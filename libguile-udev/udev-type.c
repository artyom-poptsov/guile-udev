/* udev-type.c -- Udev object implementation.
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

#include "common.h"
#include "udev-type.h"

scm_t_bits udev_tag;

static SCM _mark(SCM udev)
{
    (void) udev;
    return SCM_BOOL_F;
}

static size_t _free(SCM udev)
{
    gudev_t* ud = _scm_to_udev_data(udev);
    udev_unref(ud->udev);
    return 0;
}

static int _print(SCM udev, SCM port, scm_print_state* pstate)
{
    (void) udev;
    (void) pstate;
    scm_puts("#<udev>", port);
    return 1;
}

static SCM _equalp(SCM x1, SCM x2)
{
    gudev_t* d1 = _scm_to_udev_data(x1);
    gudev_t* d2 = _scm_to_udev_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}


SCM_DEFINE(gudev_is_udev_p, "udev?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev object, #f otherwise.")
{
    return scm_from_bool(SCM_SMOB_PREDICATE(udev_tag, x));
}

/**
 * @brief _scm_from_udev -- Convert an Udev handle to a Guile SMOB.
 * @param udev -- A pointer to an Udev handle.
 * @return A new udev SMOB.
 */
SCM _scm_from_udev(struct udev *udev)
{
    SCM smob;
    gudev_t* ud = (gudev_t *) scm_gc_malloc(sizeof(gudev_t),
                                            "udev");
    ud->udev = udev;
    SCM_NEWSMOB(smob, udev_tag, ud);
    return smob;
}

/**
 * @brief _scm_to_udev_data -- Convert a SCM object to a Udev data structure.
 * @param x -- Source SCM object.
 * @return A pointer to the Udev data.
 */
gudev_t* _scm_to_udev_data(SCM x)
{
    scm_assert_smob_type(udev_tag, x);
    return (gudev_t *) SCM_SMOB_DATA(x);
}



SCM_DEFINE(udev_make_udev,
           "make-udev", 0, 0, 0,
           (),
           "Make an Udev handle.")
{
    SCM smob;
    gudev_t* ud = (gudev_t *) scm_gc_malloc(sizeof (gudev_t),
                                            "udev");
    ud->udev = udev_new();
    SCM_NEWSMOB(smob, udev_tag, ud);
    return smob;
}

/**
 * @brief init_udev_type -- Udev smob initialization.
 */
void init_udev_type()
{
    udev_tag = scm_make_smob_type("udev", sizeof(gudev_t));
    set_smob_callbacks(udev_tag, _mark, _free, _equalp, _print);

#include "udev-type.x"
}
