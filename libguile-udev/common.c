/* common.c -- Common procedures.
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

#include "common.h"

/**
 * Convert an Udev list to a Scheme alist.
 */
SCM _scm_alist_from_udev_list(struct udev_list_entry* entry)
{
  SCM alist = scm_make_list(scm_from_int(0), SCM_UNDEFINED);
  for (; entry != NULL; entry = udev_list_entry_get_next(entry)) {
    const char* name  = udev_list_entry_get_name(entry);
    const char* value = udev_list_entry_get_value(entry);

    alist = scm_acons(scm_from_locale_string(name),
                      scm_from_locale_string(value),
                      alist);
  }
  return alist;
}

/** Procedure that converts an SCM object to an Guile-UDev structure
 * pointer.
 */
SCM compare_objects(SCM x1, SCM x2, converter_t converter)
{
    void* d1 = converter(x1);
    void* d2 = converter(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}


void set_smob_callbacks(scm_t_bits tag,
                        gc_mark_callback_t   mark_cb,
                        gc_free_callback_t   free_cb,
                        gc_equalp_callback_t equalp_cb,
                        gc_print_callback_t  print_cb)
{
    scm_set_smob_mark(tag, mark_cb);
    scm_set_smob_free(tag, free_cb);
    scm_set_smob_print(tag, print_cb);
    scm_set_smob_equalp(tag, equalp_cb);
}
