/* udev-device-func.c -- Udev device procedures.
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

#include "udev-device-type.h"
#include "udev-device-func.h"
#include "common.h"

/**
 * Get a string device property from an SCM device object.
 */
static SCM _get_string_property(SCM device,
                                const char* (*func)(struct udev_device *))
#define FUNC_NAME "_get_string_property"
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     const char* result = func(udd->udev_device);
     return scm_from_locale_string(result);
}
#undef FUNC_NAME


SCM_DEFINE(gudev_device_get_action,
           "udev-device-get-action", 1, 0, 0,
           (SCM device),
           "Get action.")
#define FUNC_NAME s_gudev_device_get_action
{
    return _get_string_property(device, udev_device_get_action);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_sysname,
           "udev-device-get-sysname", 1, 0, 0,
           (SCM device),
           "Get sysname.")
#define FUNC_NAME s_gudev_device_get_sysname
{
    return _get_string_property(device, udev_device_get_sysname);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devpath,
           "udev-device-get-devpath", 1, 0, 0,
           (SCM device),
           "Get devpath.")
#define FUNC_NAME s_gudev_device_get_devpath
{
    return _get_string_property(device, udev_device_get_devpath);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devnode,
           "udev-device-get-devnode", 1, 0, 0,
           (SCM device),
           "Get devnode.")
#define FUNC_NAME s_gudev_device_get_devnode
{
    return _get_string_property(device, udev_device_get_devnode);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devtype,
           "udev-device-get-devtype", 1, 0, 0,
           (SCM device),
           "Get the device type.")
#define FUNC_NAME s_gudev_device_get_devtype
{
    return _get_string_property(device, udev_device_get_devtype);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_driver,
           "udev-device-get-driver", 1, 0, 0,
           (SCM device),
           "Get the device driver.")
#define FUNC_NAME s_gudev_device_get_driver
{
     return _get_string_property(device, udev_device_get_driver);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_udev,
           "udev-device-get-udev", 1, 0, 0,
           (SCM device),
           "Get the parent udev object.")
#define FUNC_NAME s_gudev_device_get_udev
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     return udd->udev;
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_property_value,
           "udev-device-get-property-value", 2, 0, 0,
           (SCM device, SCM property),
           "Get the value of given PROPERTY for the DEVICE.")
#define FUNC_NAME s_gudev_device_get_property
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     char* c_property = NULL;

     scm_dynwind_begin(0);
     c_property = scm_to_locale_string(property);
     scm_dynwind_free(c_property);

     const char* value = udev_device_get_property_value(udd->udev_device,
                                                        c_property);

     scm_dynwind_end();

     return scm_from_locale_string(value);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_links,
           "udev-device-get-links", 1, 0, 0,
           (SCM device),
           "Get device links.")
#define FUNC_NAME s_gudev_device_get_links
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     struct udev_list_entry* entry
          = udev_device_get_devlinks_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_properties,
           "udev-device-get-properties", 1, 0, 0,
           (SCM device),
           "Get device properties as an AList.")
#define FUNC_NAME s_gudev_device_get_properties
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     struct udev_list_entry* entry
          = udev_device_get_properties_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_tags,
           "udev-device-get-tags", 1, 0, 0,
           (SCM device),
           "Get device tags as an AList.")
#define FUNC_NAME s_gudev_device_get_tags
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     struct udev_list_entry* entry
          = udev_device_get_tags_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME


void init_udev_device_func()
{
#include "udev-device-func.x"
}
