#include <libguile.h>
#include <libudev.h>

#include "common.h"
#include "udev-type.h"

static void _finalizer(void* data) {
    struct udev* ud = (struct udev *) data;
    udev_unref(ud);
}

SCM_DEFINE_1(gudev_to_pointer, "udev->pointer", (SCM udev),
             "Convert an Guile UDEV instance to an Udev pointer.")
{
    gudev_t* ud = gudev_from_scm(udev);
    return scm_from_pointer(ud->udev, _finalizer);
}

SCM_DEFINE_1(gudev_from_pointer, "pointer->udev", (SCM udev_pointer),
             "Convert an UDEV instance to a pointer.")
{
    struct udev* ud = scm_to_pointer(udev_pointer);
    return udev_to_scm(ud);
}
