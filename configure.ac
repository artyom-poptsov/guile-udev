dnl   configuration script for Guile-Udev
dnl   Process this file with autoconf to produce configure.
dnl

define(GUILE_UDEV_CONFIGURE_COPYRIGHT, [[
Copyright (C) 2020-2022 Artyom V. Poptsov <poptsov.artyom@gmail.com>

This file is part of Guile-Udev.

Guile-Udev is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Guile-Udev is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Guile-Udev.  If not, see <http://www.gnu.org/licenses/>.
]])

AC_INIT([Guile-Udev], [0.2.3], [poptsov.artyom@gmail.com],
                     [guile-udev],
                     [https://github.com/artyom-poptsov/guile-udev])

AC_COPYRIGHT(GUILE_UDEV_CONFIGURE_COPYRIGHT)

dnl See
dnl   <https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html>
LIBGUILE_UDEV_INTERFACE="1:3:0"
AC_SUBST(LIBGUILE_UDEV_INTERFACE)

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

AM_INIT_AUTOMAKE([color-tests])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])], [AC_SUBST([AM_DEFAULT_VERBOSITY],1)])

AC_PROG_CC
LT_INIT([disable-static])

if test "x$GCC" = "xyes"; then
  # Use compiler warnings.
  WARN_CFLAGS="-Wall"
else
  WARN_CFLAGS=""
fi
AC_SUBST([WARN_CFLAGS])

AC_ARG_WITH([guilesitedir],
            [AS_HELP_STRING([--with-guilesitedir],
                            [use the specified installation path for Guile modules])],
            [case "x$withval" in
             xyes|xno) guilesitedir="";;
             *)        guilesitedir="$withval";;
             esac],
             [guilesitedir=""])

# -------------------------------------------------------------------------------
# Check for needed libraries
# -------------------------------------------------------------------------------

PKG_CHECK_MODULES([UDEV], [udev >= 239])

AM_CONDITIONAL(HAVE_UDEV, $HAVE_UDEV)

dnl These macros must be provided by guile.m4.
m4_pattern_forbid([^GUILE_PKG$])
m4_pattern_forbid([^GUILE_PROGS$])

dnl Use this macro so that 'GUILE_EFFECTIVE_VERSION' is defined here.
dnl Try Guile 3.0, then 2.2, and finally 2.0.
GUILE_PKG([3.0 2.2 2.0])

GUILE_PROGS
GUILE_FLAGS
GUILE_SITE_DIR

AC_PATH_PROG([guile_snarf], [guile-snarf], [not-found])
if test "x$guile_snarf" = "xnot-found"; then
   AC_MSG_ERROR([`guile-snarf' not found.  Please install Guile 2.x, 3.x or later.])
fi

AC_SUBST([top_srcdir])

if test "x$GUILD" = "x"; then
   GUILD=`which guild`
   if test "x$GUILD" = "x"; then
      GUILD=`pwd`/build-aux/guild
   fi
   AC_SUBST(GUILD)
fi

dnl (srfi srfi-64) appeared in Guile 2.0.10.  Make sure we have it.
GUILE_MODULE_AVAILABLE([have_srfi64], [(srfi srfi-64)])
if test "x$have_srfi64" != "xyes"; then
  AC_MSG_ERROR([(srfi srfi-64) is missing; please install a more recent Guile.])
fi

LT_INIT()

if test "x$guilesitedir" = "x"; then
   guilesitedir="$datadir/guile/site/$GUILE_EFFECTIVE_VERSION"
fi
AC_SUBST([guilesitedir])

GUILE_EFFECTIVE_VERSION=`$GUILE -c '(display (effective-version))'`
AC_SUBST(GUILE_EFFECTIVE_VERSION)

AC_CONFIG_FILES([Makefile libguile-udev/Makefile build-aux/Makefile])
AC_CONFIG_FILES([build-aux/m4/Makefile build-aux/am/Makefile doc/Makefile])
AC_CONFIG_FILES([modules/Makefile modules/udev/Makefile])

AM_CONDITIONAL([CROSS_COMPILING], [test "x$cross_compiling" = "xyes"])

# Generate a Makefile, based on the results.
AC_OUTPUT

if test "$guilesitedir" != "$GUILE_SITE"; then
   # Guile has a different prefix than this module
   AC_MSG_WARN([]
   [The Guile modules will be installed in ${guilesitedir}.]
   [You should probably re-run `configure' with]
   [`--with-guilesitedir=$GUILE_SITE']
   [Otherwise, you will have to adjust the `GUILE_LOAD_PATH' environment]
   [variable.])
fi
