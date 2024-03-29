/* udev-device-func.c -- Udev device procedures.
 *
 * Copyright (C) 2020-2022 Artyom V. Poptsov <poptsov.artyom@gmail.com>
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
#include "error.h"

/**
 * Get a string device property from an SCM device object.
 */
static SCM _get_string_property(SCM device,
                                const char* (*func)(struct udev_device *))
#define FUNC_NAME "_get_string_property"
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     const char* result = func(udd->udev_device);
     if (result) {
          return scm_from_locale_string(result);
     } else {
          guile_udev_error1(FUNC_NAME, "Could not get a string", device);
          return SCM_UNDEFINED; /* Not reached. */
     }
}
#undef FUNC_NAME


SCM_DEFINE_1(gudev_device_get_action, "udev-device-get-action", (SCM device),
             "Get action.")
#define FUNC_NAME s_gudev_device_get_action
{
    return _get_string_property(device, udev_device_get_action);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_sysname, "udev-device-get-sysname", (SCM device),
           "Get sysname.")
#define FUNC_NAME s_gudev_device_get_sysname
{
    return _get_string_property(device, udev_device_get_sysname);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_devpath, "udev-device-get-devpath", (SCM device),
             "Get devpath.")
#define FUNC_NAME s_gudev_device_get_devpath
{
    return _get_string_property(device, udev_device_get_devpath);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_devnode, "udev-device-get-devnode", (SCM device),
             "Get devnode.")
#define FUNC_NAME s_gudev_device_get_devnode
{
    return _get_string_property(device, udev_device_get_devnode);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_devtype, "udev-device-get-devtype", (SCM device),
             "Get the device type.")
#define FUNC_NAME s_gudev_device_get_devtype
{
    return _get_string_property(device, udev_device_get_devtype);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_driver, "udev-device-get-driver", (SCM device),
             "Get the device driver.")
#define FUNC_NAME s_gudev_device_get_driver
{
     return _get_string_property(device, udev_device_get_driver);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_udev, "udev-device-get-udev", (SCM device),
             "Get the parent udev object.")
#define FUNC_NAME s_gudev_device_get_udev
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     return udd->udev;
}
#undef FUNC_NAME

SCM_DEFINE_N(gudev_device_get_property_value,
             "udev-device-get-property-value", 2,
             (SCM device, SCM property),
             "Get the value of given PROPERTY for the DEVICE.")
#define FUNC_NAME s_gudev_device_get_property_value
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     char* c_property = NULL;

     scm_dynwind_begin(0);
     c_property = scm_to_locale_string(property);
     scm_dynwind_free(c_property);

     const char* value = udev_device_get_property_value(udd->udev_device,
                                                        c_property);
     if (! value) {
          guile_udev_error1(FUNC_NAME,
                            "Could not get the property value",
                            scm_list_2(device, property));
     }

     scm_dynwind_end();

     return scm_from_locale_string(value);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_links, "udev-device-get-links", (SCM device),
             "Get device links.")
#define FUNC_NAME s_gudev_device_get_links
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     struct udev_list_entry* entry
          = udev_device_get_devlinks_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_properties, "udev-device-get-properties",
             (SCM device),
             "Get device properties as an AList.")
#define FUNC_NAME s_gudev_device_get_properties
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     struct udev_list_entry* entry
          = udev_device_get_properties_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME

SCM_DEFINE_1(gudev_device_get_tags, "udev-device-get-tags", (SCM device),
             "Get device tags as an AList.")
#define FUNC_NAME s_gudev_device_get_tags
{
     gudev_device_t* udd = gudev_device_from_scm(device);
     struct udev_list_entry* entry
          = udev_device_get_tags_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME


void init_udev_device_func()
{
#include "udev-device-func.x"
}
