/* udev-monitor-type.h -- Udev monitor object implementation.
 *
 * Copyright (C) 2020 Artyom V. Poptsov <poptsov.artyom@gmail.com>
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
#include <pthread.h>

#include "common.h"
#include "udev-type.h"
#include "udev-monitor-type.h"

scm_t_bits udev_monitor_tag;

static SCM _mark(SCM um)
{
    gudev_monitor_t* umd = gudev_monitor_from_scm(um);
    return umd->udev;
}

static size_t _free(SCM um)
{
    gudev_monitor_t* umd = gudev_monitor_from_scm(um);
    scm_gc_unprotect_object(umd->scanner_callback);
    udev_monitor_unref(umd->udev_monitor);
    pthread_mutex_destroy(&umd->lock);
    return 0;
}

static int _print(SCM um, SCM port, scm_print_state* pstate)
{
    (void) um;
    (void) pstate;
    scm_puts("#<udev-monitor>", port);
    return 1;
}

static SCM _equalp(SCM x1, SCM x2)
{
    return compare_objects(x1, x2, (converter_t) gudev_monitor_from_scm);
}


SCM_DEFINE(gudev_is_udev_monitor_p, "udev-monitor?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev monitor object, #f otherwise.")
{
    return scm_from_bool(SCM_SMOB_PREDICATE(udev_monitor_tag, x));
}

gudev_monitor_t* make_udev_monitor()
{
    return (gudev_monitor_t *) scm_gc_malloc(
                sizeof(gudev_monitor_t),
                "udev-monitor");
}

/**
 * @brief _scm_from_udev_data -- Convert an Udev handle to a Guile SMOB.
 * @param udev -- A pointer to an Udev monitor handle.
 * @return A new udev monitor SMOB.
 */
SCM gudev_monitor_to_scm(SCM udev, struct udev_monitor *udev_monitor)
{
    SCM smob;
    gudev_monitor_t* umd = make_udev_monitor();
    umd->udev             = udev;
    umd->udev_monitor     = udev_monitor;
    umd->timeout.tv_sec   = 0;
    umd->timeout.tv_usec  = 0;
    umd->is_scanning      = 0;
    umd->scanner_callback = SCM_BOOL_F;
    pthread_mutex_init(&umd->lock, NULL);

    SCM_NEWSMOB(smob, udev_monitor_tag, umd);

    return smob;
}

/**
 * @brief _scm_to_udev_data -- Convert a SCM object to a Udev data structure.
 * @param x -- Source SCM object.
 * @return A pointer to the Udev data.
 */
gudev_monitor_t* gudev_monitor_from_scm(SCM x)
{
    scm_assert_smob_type(udev_monitor_tag, x);
    return (gudev_monitor_t *) SCM_SMOB_DATA(x);
}

SCM_DEFINE(gudev_make_udev_monitor,
           "%make-udev-monitor", 1, 0, 0,
           (SCM udev),
           "Make an Udev monitor.")
{
    gudev_t* ud = gudev_from_scm(udev);
    struct udev_monitor* um = udev_monitor_new_from_netlink(ud->udev, "udev");
    return gudev_monitor_to_scm(udev, um);
}

void init_udev_monitor_type()
{
    udev_monitor_tag = scm_make_smob_type("udev-monitor",
                                          sizeof(gudev_monitor_t));
    set_smob_callbacks(udev_monitor_tag, _mark, _free, _equalp, _print);

#include "udev-monitor-type.x"
}
