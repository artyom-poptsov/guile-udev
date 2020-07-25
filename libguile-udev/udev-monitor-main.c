#include "udev-monitor-type.h"
#include "udev-monitor-func.h"

void init_udev_monitor()
{
    init_udev_monitor_type();
    init_udev_monitor_func();
}
