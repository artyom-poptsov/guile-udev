#include "udev-device-type.h"
#include "udev-monitor-func.h"

void init_udev_device()
{
    init_udev_device_type();
    init_udev_device_func();
}
