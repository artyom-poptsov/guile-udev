/* udev-monitor-type.h -- Udev monitor object description.
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

#ifndef UDEV_MONITOR_TYPE_H
#define UDEV_MONITOR_TYPE_H

#include <libguile.h>
#include <libudev.h>

#include <pthread.h>

/**
 * @brief The udev_data struct -- Contents of a Guile SMOB object that
 *     that represents an Udev handle.
 */
struct gudev_monitor {
     struct udev_monitor* udev_monitor;
     SCM udev;
     SCM scanner_callback;
     SCM error_callback;

     /**
      * @brief is_scanning -- does the monitor perform the scanning
      *     at the moment?
      */
     int is_scanning;

     /**
      * @brief timeout -- 'select' timeout for the device monitor.
      */
     SCM secs;
     SCM usecs;

     pthread_mutex_t lock;
     pthread_t scanner_thread;
};

typedef struct gudev_monitor gudev_monitor_t;

void init_udev_monitor_type();

gudev_monitor_t* make_udev_monitor();
gudev_monitor_t* gudev_monitor_from_scm(SCM x);
SCM gudev_monitor_to_scm(SCM udev, struct udev_monitor *udev_monitor);

#endif // UDEV_MONITOR_TYPE_H
