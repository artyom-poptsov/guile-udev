/* udev-device-func.h -- Udev device object description.
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

#ifndef UDEV_DEVICE_H
#define UDEV_DEVICE_H

#include <libguile.h>
#include <libudev.h>

/**
 * @brief The udev_device_type struct -- Contents of a Guile SMOB object that
 *     that represents an Udev device handle.
 */
struct gudev_device {
    struct udev_device* udev_device;
    SCM udev;
};

typedef struct gudev_device gudev_device_t;

void init_udev_device_type();

gudev_device_t* make_gudev_device();
gudev_device_t* gudev_device_from_scm(SCM x);
SCM udev_device_to_scm(SCM udev, struct udev_device *udev_device);

#endif // UDEV_DEVICE_H
