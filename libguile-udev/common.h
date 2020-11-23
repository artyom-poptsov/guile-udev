/* common.h -- Common procedures.
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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <libguile.h>
#include <libudev.h>

#include "common.h"

/* GC callbacks. */
typedef SCM    (*gc_mark_callback_t  )(SCM obj);
typedef size_t (*gc_free_callback_t  )(SCM obj);
typedef SCM    (*gc_equalp_callback_t)(SCM x1, SCM x2);
typedef int    (*gc_print_callback_t )(SCM obj, SCM port, scm_print_state* ps);


typedef void* (*converter_t)(SCM x);


SCM _scm_alist_from_udev_list(struct udev_list_entry* entry);

SCM compare_objects(SCM x1, SCM x2, converter_t converter);

void set_smob_callbacks(scm_t_bits tag,
                        gc_mark_callback_t   mark_cb,
                        gc_free_callback_t   free_cb,
                        gc_equalp_callback_t equalp_cb,
                        gc_print_callback_t  print_cb);

#endif  /* ifndef __COMMON_H__ */
