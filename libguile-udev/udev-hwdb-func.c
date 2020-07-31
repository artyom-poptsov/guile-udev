/* udev-hwdb-func.c -- Udev HWDB procedures.
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

#include "udev-hwdb-type.h"
#include "common.h"
#include "error.h"

SCM_DEFINE(gudev_hwdb_lookup,
           "udev-hwdb-lookup", 2, 0, 0,
           (SCM udev_hwdb, SCM mod_alias),
           "Lookup a matching device in the hardware database.")
#define FUNC_NAME s_gudev_hwdb_lookup
{
     struct udev_hwdb_data* uhd = _scm_to_udev_hwdb_data(udev_hwdb);
     struct udev_list_entry* entry = NULL;
     int flags = 0;
     char* c_mod_alias = NULL;

     scm_dynwind_begin(0);
     c_mod_alias = scm_to_locale_string(mod_alias);
     scm_dynwind_free(c_mod_alias);

     entry = udev_hwdb_get_properties_list_entry(uhd->udev_hwdb,
                                                 c_mod_alias,
                                                 flags); /* unused */
     if (! entry) {
          guile_udev_error1(FUNC_NAME,
                            "Could not lookup a device",
                            scm_list_2(udev_hwdb, mod_alias));
     }

     scm_dynwind_end();

     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME


void init_udev_hwdb_func()
{
#include "udev-hwdb-func.x"
}
