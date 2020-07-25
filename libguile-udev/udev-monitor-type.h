#ifndef UDEV_MONITOR_TYPE_H
#define UDEV_MONITOR_TYPE_H

#include <libguile.h>
#include <libudev.h>

#include <pthread.h>

/**
 * @brief The udev_data struct -- Contents of a Guile SMOB object that
 *     that represents an Udev handle.
 */
struct udev_monitor_data {
    struct udev_monitor* udev_monitor;
    SCM udev;
    SCM scanner_callback;

    /**
     * @brief is_scanning -- does the monitor perform the scanning
     *     at the moment?
     */
    int is_scanning;

    pthread_mutex_t lock;
    pthread_t scanner_thread;
};

void init_udev_monitor_type();

struct udev_monitor_data* _allocate_udev_monitor();
struct udev_monitor_data* _scm_to_udev_monitor_data(SCM x);
SCM _scm_from_udev_monitor(SCM udev, struct udev_monitor *udev_monitor);

#endif // UDEV_MONITOR_TYPE_H
