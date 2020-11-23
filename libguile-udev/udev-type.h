/* udev-type.h -- Udev object description.
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

#ifndef UDEV_TYPE_H
#define UDEV_TYPE_H

#include <libguile.h>
#include <libudev.h>

/**
 * @brief The udev_data struct -- Contents of a Guile SMOB object that
 *     that represents an Udev handle.
 */
struct gudev {
    struct udev* udev;
};

typedef struct gudev gudev_t;

void init_udev_type();

gudev_t* gudev_from_scm(SCM x);
SCM udev_to_scm(struct udev* udev);

#endif // UDEV_TYPE_H
