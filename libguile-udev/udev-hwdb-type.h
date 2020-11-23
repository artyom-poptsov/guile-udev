/* udev-hwdb-type.h -- Udev HWDB object description.
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

#ifndef UDEV_HWDB_TYPE_H
#define UDEV_HWDB_TYPE_H

#include <libguile.h>
#include <libudev.h>

/**
 * @brief The udev_data struct -- Contents of a Guile SMOB object that
 *     that represents an Udev handle.
 */
struct gudev_hwdb {
  struct udev_hwdb* udev_hwdb;

  /**
   * @brief udev -- Parent Scheme udev object.
   */
  SCM udev;
};

typedef struct gudev_hwdb gudev_hwdb_t;

void init_udev_hwdb_type();

gudev_hwdb_t* make_gudev_hwdb();
gudev_hwdb_t* _scm_to_udev_hwdb_data(SCM x);
SCM _scm_from_udev_hwdb(SCM udev, struct udev_hwdb* udev_hwdb);

#endif // UDEV_HWDB_TYPE_H
