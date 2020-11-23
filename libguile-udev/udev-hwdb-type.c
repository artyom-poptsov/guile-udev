/* udev-hwdb-type.c -- Udev HWDB object implementation.
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

#include "common.h"
#include "udev-type.h"
#include "udev-hwdb-type.h"

scm_t_bits udev_hwdb_tag;

static SCM _mark(SCM udev_hwdb)
{
     gudev_hwdb_t* uhd = _scm_to_udev_hwdb_data(udev_hwdb);
     return uhd->udev;
}

static size_t _free(SCM udev_hwdb)
{
     gudev_hwdb_t* uhd = _scm_to_udev_hwdb_data(udev_hwdb);
     udev_hwdb_unref(uhd->udev_hwdb);
     return 0;
}

static int _print(SCM udev_hwdb, SCM port, scm_print_state* pstate)
{
     (void) udev_hwdb;
     (void) pstate;
     scm_puts("#<udev-hwdb>", port);
     return 1;
}

static SCM _equalp(SCM x1, SCM x2)
{
     return compare_objects(x1, x2, (converter_t) _scm_to_udev_hwdb_data);
}


SCM_DEFINE(gudev_is_udev_hwdb_p, "udev-hwdb?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev HWDB object, #f otherwise.")
{
     return scm_from_bool(SCM_SMOB_PREDICATE(udev_hwdb_tag, x));
}

gudev_hwdb_t* make_gudev_hwdb()
{
     return (gudev_hwdb_t *) scm_gc_malloc(
          sizeof(gudev_hwdb_t),
          "udev-hwdb");
}

/**
 * @brief _scm_from_udev -- Convert an Udev handle to a Guile SMOB.
 * @param udev      -- A parent Udev Scheme object.
 * @param udev_hwdb -- A pointer to an Udev HWDB handle.
 * @return A new udev HWDB SMOB.
 */
SCM udev_hwdb_to_scm(SCM udev, struct udev_hwdb *udev_hwdb)
{
     SCM smob;
     gudev_hwdb_t* uhd = make_gudev_hwdb();
     uhd->udev      = udev;
     uhd->udev_hwdb = udev_hwdb;
     SCM_NEWSMOB(smob, udev_hwdb_tag, uhd);
     return smob;
}

gudev_hwdb_t* _scm_to_udev_hwdb_data(SCM x)
{
     scm_assert_smob_type(udev_hwdb_tag, x);
     return (gudev_hwdb_t *) SCM_SMOB_DATA(x);
}

SCM_DEFINE(udev_make_udev_hwdb,
           "make-udev-hwdb", 1, 0, 0,
           (SCM udev),
           "Make an Udev HWDB handle.")
{
     gudev_t* ud = gudev_from_scm(udev);
     struct udev_hwdb* hwdb = udev_hwdb_new(ud->udev);
     return udev_hwdb_to_scm(udev, hwdb);
}

void init_udev_hwdb_type()
{
     udev_hwdb_tag = scm_make_smob_type("udev-hwdb",
                                        sizeof(gudev_hwdb_t));
     set_smob_callbacks(udev_hwdb_tag, _mark, _free, _equalp, _print);

#include "udev-hwdb-type.x"
}
