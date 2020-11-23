/* udev-device-type.c -- Udev device type implementation.
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

#include "udev-device-type.h"

scm_t_bits udev_device_tag;

static SCM mark_udev_device(SCM udev_device)
{
    gudev_device_t* udd = _scm_to_udev_device_data(udev_device);
    return udd->udev;
}

static size_t free_udev_device(SCM udev_device)
{
    gudev_device_t* udd = _scm_to_udev_device_data(udev_device);
    udev_device_unref(udd->udev_device);
    return 0;
}

static int print_udev_device(SCM udev_device, SCM port, scm_print_state* pstate)
{
    (void) udev_device;
    (void) pstate;
    scm_puts("#<udev-device>", port);
    return 1;
}

static SCM equalp_udev_device(SCM x1, SCM x2)
{
    gudev_device_t* d1 = _scm_to_udev_device_data(x1);
    gudev_device_t* d2 = _scm_to_udev_device_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

gudev_device_t* _allocate_udev_device()
{
    return scm_gc_malloc(sizeof(gudev_device_t),
                         "udev-device");
}

/**
 * @brief _scm_from_udev_device -- Convert an Udev device handle to a
 *     Guile SMOB.
 * @param udev -- A pointer to an Udev device handle.
 * @return A new udev device SMOB.
 */
SCM _scm_from_udev_device(SCM udev, struct udev_device *udev_device)
{
    SCM smob;
    gudev_device_t* udd = _allocate_udev_device();
    udd->udev_device = udev_device;
    udd->udev        = udev;
    SCM_NEWSMOB(smob, udev_device_tag, udd);
    return smob;
}

/**
 * @brief _scm_to_udev_data -- Convert a SCM object to a Udev data structure.
 * @param x -- Source SCM object.
 * @return A pointer to the Udev data.
 */
gudev_device_t* _scm_to_udev_device_data(SCM x)
{
    scm_assert_smob_type(udev_device_tag, x);
    return (gudev_device_t *) SCM_SMOB_DATA(x);
}

/**
 * @brief init_udev_type -- Udev smob initialization.
 */
void init_udev_device_type()
{
    udev_device_tag = scm_make_smob_type("udev-device",
                                         sizeof(gudev_device_t));
    scm_set_smob_mark(udev_device_tag, mark_udev_device);
    scm_set_smob_free(udev_device_tag, free_udev_device);
    scm_set_smob_print(udev_device_tag, print_udev_device);
    scm_set_smob_equalp(udev_device_tag, equalp_udev_device);

#include "udev-device-type.x"
}
