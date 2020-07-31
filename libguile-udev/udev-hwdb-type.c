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

#include "udev-type.h"
#include "udev-hwdb-type.h"

scm_t_bits udev_hwdb_tag;

static SCM mark_udev_hwdb(SCM udev_hwdb)
{
     struct udev_hwdb_data* uhd = _scm_to_udev_hwdb_data(udev_hwdb);
     return uhd->udev;
}

static size_t free_udev_hwdb(SCM udev_hwdb)
{
     struct udev_hwdb_data* uhd = _scm_to_udev_hwdb_data(udev_hwdb);
     udev_hwdb_unref(uhd->udev_hwdb);
     return 0;
}

static int print_udev_hwdb(SCM udev_hwdb, SCM port, scm_print_state* pstate)
{
     (void) udev_hwdb;
     (void) pstate;
     scm_puts("#<udev-hwdb>", port);
     return 1;
}

static SCM equalp_udev_hwdb(SCM x1, SCM x2)
{
     struct udev_hwdb_data* d1 = _scm_to_udev_hwdb_data(x1);
     struct udev_hwdb_data* d2 = _scm_to_udev_hwdb_data(x2);
     if ((! d1) || (! d2)) {
          return SCM_BOOL_F;
     } else if (d1 != d2) {
          return SCM_BOOL_F;
     } else {
          return SCM_BOOL_T;
     }
}

SCM_DEFINE(gudev_is_udev_hwdb_p, "udev-hwdb?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev HWDB object, #f otherwise.")
{
     return scm_from_bool(SCM_SMOB_PREDICATE(udev_hwdb_tag, x));
}

struct udev_hwdb_data* _allocate_udev_hwdb()
{
     return (struct udev_hwdb_data *) scm_gc_malloc(
          sizeof(struct udev_hwdb_data),
          "udev-hwdb");
}

/**
 * @brief _scm_from_udev -- Convert an Udev handle to a Guile SMOB.
 * @param udev      -- A parent Udev Scheme object.
 * @param udev_hwdb -- A pointer to an Udev HWDB handle.
 * @return A new udev HWDB SMOB.
 */
SCM _scm_from_udev_hwdb(SCM udev, struct udev_hwdb *udev_hwdb)
{
     SCM smob;
     struct udev_hwdb_data* uhd = _allocate_udev_hwdb();
     uhd->udev      = udev;
     uhd->udev_hwdb = udev_hwdb;
     SCM_NEWSMOB(smob, udev_hwdb_tag, uhd);
     return smob;
}

struct udev_hwdb_data* _scm_to_udev_hwdb_data(SCM x)
{
     scm_assert_smob_type(udev_hwdb_tag, x);
     return (struct udev_hwdb_data *) SCM_SMOB_DATA(x);
}

SCM_DEFINE(udev_make_udev_hwdb,
           "make-udev-hwdb", 1, 0, 0,
           (SCM udev),
           "Make an Udev HWDB handle.")
{
     struct udev_data* ud = _scm_to_udev_data(udev);
     struct udev_hwdb* hwdb = udev_hwdb_new(ud->udev);
     return _scm_from_udev_hwdb(udev, hwdb);
}

void init_udev_hwdb_type()
{
     udev_hwdb_tag = scm_make_smob_type("udev-hwdb",
                                        sizeof(struct udev_hwdb_data));
     scm_set_smob_mark(udev_hwdb_tag, mark_udev_hwdb);
     scm_set_smob_free(udev_hwdb_tag, free_udev_hwdb);
     scm_set_smob_print(udev_hwdb_tag, print_udev_hwdb);
     scm_set_smob_equalp(udev_hwdb_tag, equalp_udev_hwdb);

#include "udev-hwdb-type.x"
}
