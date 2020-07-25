#ifndef UDEV_TYPE_H
#define UDEV_TYPE_H

#include <libguile.h>
#include <libudev.h>

/**
 * @brief The udev_data struct -- Contents of a Guile SMOB object that
 *     that represents an Udev handle.
 */
struct udev_data {
    struct udev* udev;
};

void init_udev_type();

struct udev_data* _scm_to_udev_data(SCM x);
SCM _scm_from_udev_data(struct udev* udev);

#endif // UDEV_TYPE_H
