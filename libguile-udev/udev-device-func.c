#include <libguile.h>
#include <libudev.h>

#include "udev-device-type.h"

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


void init_udev_device_func()
{
#include "udev-device-func.x"
}
