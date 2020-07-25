#ifndef UDEV_DEVICE_H
#define UDEV_DEVICE_H

#include <libguile.h>
#include <libudev.h>

/**
 * @brief The udev_device_type struct -- Contents of a Guile SMOB object that
 *     that represents an Udev device handle.
 */
struct udev_device_data {
    struct udev_device* udev_device;
    SCM udev;
};

void init_udev_device_type();

struct udev_device_data* _allocate_udev_device();
struct udev_device_data* _scm_to_udev_device_data(SCM x);
SCM _scm_from_udev_device(SCM udev, struct udev_device *udev_device);

#endif // UDEV_DEVICE_H
