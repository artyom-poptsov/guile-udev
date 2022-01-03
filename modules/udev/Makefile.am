## Copyright (C) 2020-2022 Artyom V. Poptsov <poptsov.artyom@gmail.com>
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

include $(top_srcdir)/build-aux/am/guile.am

SOURCES = \
	device.scm		\
	monitor.scm		\
	udev.scm		\
	hwdb.scm

ETAGS_ARGS = $(SOURCES)

moddir=$(prefix)/share/guile/site/$(GUILE_EFFECTIVE_VERSION)/udev
godir=$(libdir)/guile/$(GUILE_EFFECTIVE_VERSION)/site-ccache/udev

## Makefile.am ends here.
