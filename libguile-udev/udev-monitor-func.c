#include <libguile.h>
#include <libudev.h>
#include <stdio.h> // debug

#include <pthread.h>

#include "udev-monitor-type.h"
#include "udev-device-type.h"
#include "error.h"

SCM_DEFINE(gudev_add_filter_x,
           "udev-monitor-add-filter!", 3, 0, 0,
           (SCM udev_monitor, SCM subsystem, SCM devtype),
           "Add a filter to the monitor.")
#define FUNC_NAME s_gudev_add_filter_x
{
    char* c_subsystem = NULL;
    char* c_devtype   = NULL;
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(udev_monitor);
    int result;

    SCM_ASSERT(scm_is_string(subsystem), subsystem, SCM_ARG1, FUNC_NAME);
    SCM_ASSERT(scm_is_string(devtype),   devtype,   SCM_ARG2, FUNC_NAME);

    scm_dynwind_begin(0);

    c_subsystem = scm_to_locale_string(subsystem);
    scm_dynwind_free(c_subsystem);

    c_devtype   = scm_to_locale_string(devtype);
    scm_dynwind_free(c_devtype);

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

SCM_DEFINE(gudev_monitor_set_callback_x,
           "udev-monitor-set-callback!", 2, 0, 0,
           (SCM udev_monitor, SCM callback),
           "Set a scanner callback.")
#define FUNC_NAME s_gudev_monitor_set_callback_x
{
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(udev_monitor);
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

void* udev_monitor_scanner(void* arg)
#define FUNC_NAME "udev_monitor_scanner"
{
    SCM udev_monitor = (SCM) arg;
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(udev_monitor);
    fd_set fd_set;
    struct timeval timeout;
    int result;
    int select_result;
    int monitor_fd;
    struct udev_device *dev;

    scm_init_guile();

    result = udev_monitor_enable_receiving(umd->udev_monitor);
    if (result < 0) {
         guile_udev_error1(FUNC_NAME, "Could not enable event receiving.",
                           udev_monitor);
    }

    monitor_fd = udev_monitor_get_fd(umd->udev_monitor);
    if (monitor_fd < 0) {
         guile_udev_error1(FUNC_NAME, "Could not udev monitor file descriptor.",
                           udev_monitor);
    }

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    SCM callback = umd->scanner_callback;
    SCM device;

    while (1) {
        pthread_mutex_lock(&umd->lock);
        if (! umd->is_scanning) {
            break;
        }
        pthread_mutex_unlock(&umd->lock);

        FD_ZERO(&fd_set);
        FD_SET(monitor_fd, &fd_set);
        select_result = select(monitor_fd + 1, &fd_set, NULL, NULL, &timeout);
        if (select_result == -1) {
             guile_udev_error1(FUNC_NAME, "Error during 'select' call.",
                               udev_monitor);
        }
        if (FD_ISSET(monitor_fd, &fd_set)) {
            printf("d0\n");
            dev = udev_monitor_receive_device(umd->udev_monitor);
            printf("d1\n");
            device = _scm_from_udev_device(umd->udev, dev);
            printf("d2\n");
            scm_call_1(callback, device);
            printf("d3\n");
            scm_remember_upto_here_1(device);
        }
    }
    scm_remember_upto_here_1(callback);
    scm_remember_upto_here_1(udev_monitor);

    return NULL;
}
#undef FUNC_NAME

SCM_DEFINE(gudev_monitor_start_scanning_x,
           "udev-monitor-start-scanning!", 1, 0, 0,
           (SCM udev_monitor),
           "Enable event receiving.")
#define FUNC_NAME s_gudev_monitor_start_scanning_x
{
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(udev_monitor);

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

SCM_DEFINE(gudev_monitor_stop_scanning_x,
           "udev-monitor-stop-scanning!", 1, 0, 0,
           (SCM udev_monitor),
           "Stop event receiving.")
#define FUNC_NAME s_gudev_monitor_stop_scanning_x
{
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(udev_monitor);
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
void init_udev_monitor_func()
{
#include "udev-monitor-func.x"
}
