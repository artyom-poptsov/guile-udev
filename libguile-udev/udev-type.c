#include <libguile.h>

#include "udev-type.h"

scm_t_bits udev_tag;

static SCM mark_udev(SCM udev)
{
    (void) udev;
    return SCM_BOOL_F;
}

static size_t free_udev(SCM udev)
{
    struct udev_data* ud = _scm_to_udev_data(udev);
    udev_unref(ud->udev);
    return 0;
}

static int print_udev(SCM udev, SCM port, scm_print_state* pstate)
{
    (void) udev;
    (void) pstate;
    scm_puts("#<udev>", port);
    return 1;
}

static SCM equalp_udev(SCM x1, SCM x2)
{
    struct udev_data* d1 = _scm_to_udev_data(x1);
    struct udev_data* d2 = _scm_to_udev_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

SCM_DEFINE(gudev_is_udev_p, "udev?", 1, 0, 0, (SCM x),
           "Return #t if X is an udev object, #f otherwise.")
{
    return scm_from_bool(SCM_SMOB_PREDICATE(udev_tag, x));
}

/**
 * @brief _scm_from_udev -- Convert an Udev handle to a Guile SMOB.
 * @param udev -- A pointer to an Udev handle.
 * @return A new udev SMOB.
 */
SCM _scm_from_udev(struct udev *udev)
{
    SCM smob;
    struct udev_data* ud
            = (struct udev_data *) scm_gc_malloc(sizeof(struct udev_data),
                                                 "udev");
    ud->udev = udev;
    SCM_NEWSMOB(smob, udev_tag, ud);
    return smob;
}

/**
 * @brief _scm_to_udev_data -- Convert a SCM object to a Udev data structure.
 * @param x -- Source SCM object.
 * @return A pointer to the Udev data.
 */
struct udev_data* _scm_to_udev_data(SCM x)
{
    scm_assert_smob_type(udev_tag, x);
    return (struct udev_data *) SCM_SMOB_DATA(x);
}



SCM_DEFINE(udev_make_udev,
           "make-udev", 0, 0, 0,
           (),
           "Make an Udev handle.")
{
    SCM smob;
    struct udev_data* ud
            = (struct udev_data *) scm_gc_malloc(sizeof (struct udev_data),
                                                 "udev");
    ud->udev = udev_new();
    SCM_NEWSMOB(smob, udev_tag, ud);
    return smob;
}

/**
 * @brief init_udev_type -- Udev smob initialization.
 */
void init_udev_type()
{
    udev_tag = scm_make_smob_type("udev", sizeof(struct udev_data));
    scm_set_smob_mark(udev_tag, mark_udev);
    scm_set_smob_free(udev_tag, free_udev);
    scm_set_smob_print(udev_tag, print_udev);
    scm_set_smob_equalp(udev_tag, equalp_udev);

#include "udev-type.x"
}
