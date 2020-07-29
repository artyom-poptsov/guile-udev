/* error.h -- Error reporting procedures.
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

#ifndef __ERROR_H__
#define __ERROR_H__

#include <libguile.h>

#define GUILE_UDEV_ERROR "guile-udev-error"

void guile_udev_error(const char *proc, const char *msg, SCM args, SCM rest);
void guile_udev_error1(const char *proc, const char *msg, SCM args);

#endif  /* ifndef __ERROR_H__ */
