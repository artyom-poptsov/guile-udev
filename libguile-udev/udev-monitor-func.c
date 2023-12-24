/* udev-monitor-func.c -- Udev monitor functions.
 *
 * Copyright (C) 2020-2023 Artyom V. Poptsov <poptsov.artyom@gmail.com>
 * Copyright (C) 2023 Maxim Cournoyer <maxim.cournoyer@gmail.com>
 *
 * This file is part of Guile-Udev.
 *
 * Guile-Udev is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Guile-Udev is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Guile-Udev. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libguile.h>
#include <libudev.h>
#include <unistd.h>             /* usleep */

#include <pthread.h>

#include "common.h"
#include "udev-monitor-type.h"
#include "udev-monitor-func.h"
#include "udev-device-type.h"
#include "error.h"

SCM_DEFINE(gudev_add_filter_x, "udev-monitor-add-filter!", 2, 1, 0,
           (SCM udev_monitor, SCM subsystem, SCM devtype),
           "Add a filter to the monitor.  @var{subsystem} is the subsystem\n"
	   "associated with a device while @var{devtype} is its device type.\n"
	   "The @var{devtype} argument is optional.")
#define FUNC_NAME s_gudev_add_filter_x
{
    char* c_subsystem = NULL;
    char* c_devtype   = NULL;
    gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
    int result;

    SCM_ASSERT(scm_is_string(subsystem), subsystem, SCM_ARG1, FUNC_NAME);
    SCM_ASSERT(scm_is_string(devtype) || SCM_UNBNDP(devtype), devtype,
	       SCM_ARG2, FUNC_NAME);

    scm_dynwind_begin(0);

    c_subsystem = scm_to_locale_string(subsystem);
    scm_dynwind_free(c_subsystem);

    if (! SCM_UNBNDP(devtype)) {
        c_devtype = scm_to_locale_string(devtype);
        scm_dynwind_free(c_devtype);
    }

    result = udev_monitor_filter_add_match_subsystem_devtype(umd->udev_monitor,
                                                             c_subsystem,
                                                             c_devtype);
    if (result < 0) {
         guile_udev_error1(FUNC_NAME, "Could not add a filter.",
                           udev_monitor);
    }

    scm_dynwind_end();

    return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_monitor_remove_filters_x, "udev-monitor-remove-filters!",
             (SCM udev_monitor),
             "Remove all the filters from a monitor.")
#define FUNC_NAME s_gudev_monitor_remove_filters_x
{
     gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
     int result = udev_monitor_filter_remove(umd->udev_monitor);
     if (result < 0) {
          guile_udev_error1(FUNC_NAME,
                            "Could not remove filters from the monitor.",
                            udev_monitor);
     }

     return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE_N(gudev_monitor_set_callback_x, "udev-monitor-set-callback!", 2,
             (SCM udev_monitor, SCM callback),
             "Set a scanner callback.")
#define FUNC_NAME s_gudev_monitor_set_callback_x
{
    gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
    SCM_ASSERT(scm_to_bool(scm_procedure_p(callback)), callback,
               SCM_ARG2, FUNC_NAME);

    pthread_mutex_lock(&umd->lock);
    if (umd->is_scanning) {
        pthread_mutex_unlock(&umd->lock);
        guile_udev_error1(FUNC_NAME,
                          "Stop the monitor before setting the callback.",
                          udev_monitor);
    }

    umd->scanner_callback = callback;
    scm_gc_protect_object(umd->scanner_callback);

    pthread_mutex_unlock(&umd->lock);

    return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE_N(gudev_monitor_set_error_callback_x, "udev-monitor-set-error-callback!", 2,
             (SCM udev_monitor, SCM callback),
             "Set an error callback.")
#define FUNC_NAME s_gudev_monitor_set_error_callback_x
{
     gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
     SCM_ASSERT(scm_to_bool(scm_procedure_p(callback)), callback,
                SCM_ARG2, FUNC_NAME);

     pthread_mutex_lock(&umd->lock);
     if (umd->is_scanning) {
          pthread_mutex_unlock(&umd->lock);
          guile_udev_error1(FUNC_NAME,
                            "Stop the monitor before setting the callback.",
                            udev_monitor);
     }

     umd->error_callback = callback;
     scm_gc_protect_object(umd->error_callback);

     pthread_mutex_unlock(&umd->lock);

     return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE(gudev_monitor_set_timeout_x, "udev-monitor-set-timeout!", 1, 2, 0,
	   (SCM udev_monitor, SCM secs, SCM usecs),
	   "Set monitor event poll @var{seconds} and @var{microseconds}\n"
	   "timeout non-negative numbers in seconds and microseconds,\n"
	   "respectively.\n\n"

	   "@var{secs} and @var{usecs} are optional;\n"
	   "They share the same semantic as the corresponding arguments of\n"
	   "Guile's `select' abstraction.  When left unspecified, the timeout\n"
	   "is cleared.\n\n"

	   "Throws 'guile-udev-error' on errors.")
#define FUNC_NAME s_gudev_monitor_set_timeout_x
{
     gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
     umd->secs = secs;
     umd->usecs = usecs;

     scm_remember_upto_here_1(udev_monitor);

     return SCM_UNDEFINED;
}
#undef FUNC_NAME

struct select_args_data {
     SCM reads;
     SCM secs;
     SCM usecs;
};

/* A procedure for calling scm_select via scm_internal_catch. */
SCM call_select(void *data) {
     struct select_args_data* args = data;
     return scm_select(args->reads, SCM_EOL, SCM_EOL, args->secs, args->usecs);
}

/* Dummy handler returning false. */
SCM false_on_exception(void *data, SCM key, SCM args) {
     return SCM_BOOL_F;
}

void* udev_monitor_scanner(void* arg)
#define FUNC_NAME "udev_monitor_scanner"
{
    SCM udev_monitor = (SCM) arg;
    gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
    int result;
    SCM select_result;
    int c_monitor_fd;
    SCM monitor_fd;
    struct udev_device *dev;
    SCM error_callback = umd->error_callback;

    scm_init_guile();

    result = udev_monitor_enable_receiving(umd->udev_monitor);
    if (result < 0) {
         char msg[] = "Could not enable event receiving.";
         scm_call_2(error_callback, udev_monitor,
                    scm_from_locale_string(msg));
         umd->is_scanning = 0;
         return NULL;
    }

    c_monitor_fd = udev_monitor_get_fd(umd->udev_monitor);
    monitor_fd = scm_from_int(c_monitor_fd);

    if (monitor_fd < 0) {
         char msg[] = "Could not retrieve udev monitor file descriptor.";
         scm_call_2(error_callback, udev_monitor,
                    scm_from_locale_string(msg));
         umd->is_scanning = 0;
         return NULL;
    }

    struct select_args_data select_args;
    select_args.reads = scm_list_1(monitor_fd);
    select_args.secs = umd->secs;
    select_args.usecs = umd->usecs;

    SCM callback = umd->scanner_callback;
    SCM device;
    SCM read_fdes;

    while (1) {
        pthread_mutex_lock(&umd->lock);
        if (! umd->is_scanning) {
            break;
        }
        pthread_mutex_unlock(&umd->lock);

	/* We use scm_select here instead of C select so as to benefit from
	 * the 'secs' and 'usecs' arguments validation/behavior that it
	 * provides. */
	select_result = scm_internal_catch(scm_from_utf8_symbol("system-error"),
					   call_select, &select_args,
					   false_on_exception, NULL);
	// Actual error handling is done here.
        if (scm_is_false(select_result)) {
             char msg[] = "Error during 'select' call.";
             scm_call_2(error_callback, udev_monitor,
                        scm_from_locale_string(msg));
             umd->is_scanning = 0;
             pthread_cancel(pthread_self());
             break;
        }

	read_fdes = scm_car(select_result);

	if (scm_member(monitor_fd, read_fdes)) {
            dev = udev_monitor_receive_device(umd->udev_monitor);
            device = udev_device_to_scm(umd->udev, dev);
            scm_call_1(callback, device);
            scm_remember_upto_here_1(device);
        }
    }
    scm_remember_upto_here_1(callback);
    scm_remember_upto_here_1(error_callback);
    scm_remember_upto_here_1(udev_monitor);

    return NULL;
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_monitor_start_scanning_x, "udev-monitor-start-scanning!",
             (SCM udev_monitor),
             "Enable event receiving.")
#define FUNC_NAME s_gudev_monitor_start_scanning_x
{
    gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);

    pthread_mutex_lock(&umd->lock);
    if (umd->is_scanning) {
        pthread_mutex_unlock(&umd->lock);
        guile_udev_error1(FUNC_NAME, "The monitor is already running.",
                          udev_monitor);
    }
    umd->is_scanning = 1;

    pthread_create(&umd->scanner_thread, NULL, &udev_monitor_scanner,
                   (void*) udev_monitor);

    pthread_mutex_unlock(&umd->lock);

    return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_monitor_stop_scanning_x, "udev-monitor-stop-scanning!",
             (SCM udev_monitor),
             "Stop event receiving.")
#define FUNC_NAME s_gudev_monitor_stop_scanning_x
{
    gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
    pthread_mutex_lock(&umd->lock);
    if (! umd->is_scanning) {
        pthread_mutex_unlock(&umd->lock);
        guile_udev_error1(FUNC_NAME, "The monitor is already stopped.",
                          udev_monitor);
    }
    umd->is_scanning = 0;

    pthread_join(umd->scanner_thread, NULL);

    pthread_mutex_unlock(&umd->lock);

    return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_monitor_get_udev, "udev-monitor-get-udev",
             (SCM udev_monitor),
             "Get the parent udev instance.")
#define FUNC_NAME s_gudev_monitor_get_udev
{
     gudev_monitor_t* umd = gudev_monitor_from_scm(udev_monitor);
     return umd->udev;
}
#undef FUNC_NAME

void init_udev_monitor_func()
{
#include "udev-monitor-func.x"
}
