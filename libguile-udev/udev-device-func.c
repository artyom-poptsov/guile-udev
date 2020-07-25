#include <libguile.h>
#include <libudev.h>

#include "udev-device-type.h"

SCM_DEFINE(gudev_device_get_action,
           "udev-device-get-action", 1, 0, 0,
           (SCM device),
           "Get action.")
#define FUNC_NAME s_gudev_device_get_action
{
    struct udev_device_data* udd = _scm_to_udev_device_data(device);
    const char* action = udev_device_get_action(udd->udev_device);
    return scm_from_locale_string(action);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_sysname,
           "udev-device-get-sysname", 1, 0, 0,
           (SCM device),
           "Get sysname.")
#define FUNC_NAME s_gudev_device_get_sysname
{
    struct udev_device_data* udd = _scm_to_udev_device_data(device);
    const char* sysname = udev_device_get_sysname(udd->udev_device);
    return scm_from_locale_string(sysname);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devpath,
           "udev-device-get-devpath", 1, 0, 0,
           (SCM device),
           "Get devpath.")
#define FUNC_NAME s_gudev_device_get_devpath
{
    struct udev_device_data* udd = _scm_to_udev_device_data(device);
    const char* devpath = udev_device_get_devpath(udd->udev_device);
    return scm_from_locale_string(devpath);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devnode,
           "udev-device-get-devnode", 1, 0, 0,
           (SCM device),
           "Get devnode.")
#define FUNC_NAME s_gudev_device_get_devnode
{
    struct udev_device_data* udd = _scm_to_udev_device_data(device);
    const char* devnode = udev_device_get_devnode(udd->udev_device);
    return scm_from_locale_string(devnode);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devtype,
           "udev-device-get-devtype", 1, 0, 0,
           (SCM device),
           "Get the device type.")
#define FUNC_NAME s_gudev_device_get_devtype
{
    struct udev_device_data* udd = _scm_to_udev_device_data(device);
    const char* devtype = udev_device_get_devtype(udd->udev_device);
    return scm_from_locale_string(devtype);
}
#undef FUNC_NAME

void init_udev_device_func()
{
#include "udev-device-func.x"
}
