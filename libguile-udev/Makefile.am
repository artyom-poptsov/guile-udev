## Copyright (C) 2020-2021 Artyom V. Poptsov <poptsov.artyom@gmail.com>
##
## This file is part of Guile-Udev.
##
## Guile-Udev is free software: you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation, either version 3 of the
## License, or (at your option) any later version.
##
## Guile-Udev is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Guile-Udev.  If not, see <http://www.gnu.org/licenses/>.

lib_LTLIBRARIES = libguile-udev.la

libguile_udev_la_SOURCES = \
	common.h		\
	common.c		\
	error.c			\
	error.h			\
	udev-device-func.c	\
	udev-device-func.h	\
	udev-device-main.c	\
	udev-device-type.c	\
	udev-device-type.h	\
	udev-func.c		\
	udev-func.h		\
	udev-main.c		\
	udev-monitor-func.c	\
	udev-monitor-func.h	\
	udev-monitor-main.c	\
	udev-monitor-type.c	\
	udev-monitor-type.h	\
	udev-type.c		\
	udev-type.h		\
	udev-hwdb-type.c	\
	udev-hwdb-type.h	\
	udev-hwdb-func.c	\
	udev-hwdb-func.h	\
	udev-hwdb-main.c

BUILT_SOURCES = \
	udev-device-func.x	\
	udev-device-type.x	\
	udev-func.x		\
	udev-monitor-func.x	\
	udev-monitor-type.x	\
	udev-type.x		\
	udev-hwdb-type.x	\
	udev-hwdb-func.x

libguile_udev_la_CPPFLAGS = $(CFLAGS) $(GUILE_CFLAGS)

UDEV_LD_FLAGS = -ludev

libguile_udev_la_LDFLAGS = -module -no-undefined $(UDEV_LD_FLAGS)  \
	-version-info $(LIBGUILE_UDEV_INTERFACE) $(GUILE_LDFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/libguile-udev -I$(top_builddir)/libguile-udev

snarfcppopts = $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(GUILE_CFLAGS) \
	$(AM_CPPFLAGS)

SUFFIXES = .x
.c.x:
	$(AM_V_GEN)$(guile_snarf) -o $@ $< $(snarfcppopts)

CLEANFILES = *.x

## Makefile.am ends here
