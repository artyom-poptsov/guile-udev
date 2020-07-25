#ifndef __ERROR_H__
#define __ERROR_H__

#include <libguile.h>

#define GUILE_UDEV_ERROR "guile-udev-error"

void guile_udev_error(const char *proc, const char *msg, SCM args, SCM rest);
void guile_udev_error1(const char *proc, const char *msg, SCM args);

#endif  /* ifndef __ERROR_H__ */
