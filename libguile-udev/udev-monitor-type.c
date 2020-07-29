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

#include "udev-type.h"
#include "udev-monitor-type.h"

scm_t_bits udev_monitor_tag;

static SCM mark_udev_monitor(SCM um)
{
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(um);
    return umd->udev;
}

static size_t free_udev_monitor(SCM um)
{
    struct udev_monitor_data* umd = _scm_to_udev_monitor_data(um);
    scm_gc_unprotect_object(umd->scanner_callback);
    udev_monitor_unref(umd->udev_monitor);
    pthread_mutex_destroy(&umd->lock);
    return 0;
}

static int print_udev_monitor(SCM um, SCM port, scm_print_state* pstate)
{
    (void) um;
    (void) pstate;
    scm_puts("#<udev-monitor>", port);
    return 1;
}

static SCM equalp_udev_monitor(SCM x1, SCM x2)
{
    struct udev_monitor_data* d1 = _scm_to_udev_monitor_data(x1);
    struct udev_monitor_data* d2 = _scm_to_udev_monitor_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

SCM_DEFINE(gudev_is_udev_monitor_p, "udev-monitor?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev monitor object, #f otherwise.")
{
    return scm_from_bool(SCM_SMOB_PREDICATE(udev_monitor_tag, x));
}

struct udev_monitor_data* _allocate_udev_monitor()
{
    return (struct udev_monitor_data *) scm_gc_malloc(
                sizeof(struct udev_monitor_data),
                "udev-monitor");
}

/**
 * @brief _scm_from_udev_data -- Convert an Udev handle to a Guile SMOB.
 * @param udev -- A pointer to an Udev monitor handle.
 * @return A new udev monitor SMOB.
 */
SCM _scm_from_udev_monitor(SCM udev, struct udev_monitor *udev_monitor)
{
    SCM smob;
    struct udev_monitor_data* umd = _allocate_udev_monitor();
    umd->udev         = udev;
    umd->udev_monitor = udev_monitor;
    umd->is_scanning  = 0;
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
struct udev_monitor_data* _scm_to_udev_monitor_data(SCM x)
{
    scm_assert_smob_type(udev_monitor_tag, x);
    return (struct udev_monitor_data *) SCM_SMOB_DATA(x);
}

SCM_DEFINE(gudev_make_udev_monitor,
           "make-udev-monitor", 1, 0, 0,
           (SCM udev),
           "Make an Udev monitor.")
{
    struct udev_data* ud = _scm_to_udev_data(udev);
    struct udev_monitor* um = udev_monitor_new_from_netlink(ud->udev, "udev");
    return _scm_from_udev_monitor(udev, um);
}

void init_udev_monitor_type()
{
    udev_monitor_tag = scm_make_smob_type("udev-monitor",
                                          sizeof(struct udev_monitor_data));
    scm_set_smob_mark(udev_monitor_tag, mark_udev_monitor);
    scm_set_smob_free(udev_monitor_tag, free_udev_monitor);
    scm_set_smob_print(udev_monitor_tag, print_udev_monitor);
    scm_set_smob_equalp(udev_monitor_tag, equalp_udev_monitor);

#include "udev-monitor-type.x"
}
