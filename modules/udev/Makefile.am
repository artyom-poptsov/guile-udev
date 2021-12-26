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

include $(top_srcdir)/build-aux/am/guilec

SCM_SOURCES = \
	device.scm		\
	monitor.scm		\
	udev.scm		\
	hwdb.scm

pkgguilesitedir = $(guilesitedir)/udev

nobase_dist_pkgguilesite_DATA = $(SCM_SOURCES)

ETAGS_ARGS = \
	$(SCM_SOURCES)

GOBJECTS = $(SCM_SOURCES:%.scm=%.go)
$(GOBJECTS): $(lib_LTLIBRARIES)	# Build the library first

ccachedir=$(libdir)/guile/$(GUILE_EFFECTIVE_VERSION)/site-ccache/udev
nobase_dist_ccache_DATA = $(GOBJECTS)

# Make sure source files are installed first, so that the mtime of
# installed compiled files is greater than that of installed source
# files.  See
# <http://lists.gnu.org/archive/html/guile-devel/2010-07/msg00125.html>
# for details.
guile_udev_install_go_files = install-nobase_dist_ccacheDATA
$(guile_udev_install_go_files): install-nobase_dist_pkgguilesiteDATA

guilec_warnings =				\
  -Wunbound-variable -Warity-mismatch		\
  -Wunused-variable -Wunused-toplevel

guilec_opts = 					\
	--load-path=$(abs_srcdir)/modules	\
	--load-path=$(abs_builddir)/modules	\
	--target=$(host)			\
	$(guilec_warnings)

if CROSS_COMPILING
CROSS_COMPILING_VARIABLE = GUILE_UDEV_CROSS_COMPILING=yes
else
CROSS_COMPILING_VARIABLE =
endif

# TODO: Move environment setup to a separate file.
guilec_env  = 									\
	GUILE_AUTO_COMPILE=0 							\
	$(CROSS_COMPILING_VARIABLE)                                      	\
	GUILE_SYSTEM_EXTENSIONS_PATH="$(abs_top_builddir)/libguile-udev/.libs/:${GUILE_SYSTEM_EXTENSIONS_PATH}"	\
	GUILE_LOAD_PATH="$(abs_top_srcdir)/modules"				\
	GUILE_LOAD_COMPILED_PATH="$(builddir)/udev:$$GUILE_LOAD_COMPILED_PATH"

.scm.go:
	$(AM_V_GUILEC)$(guilec_env) $(GUILEC) $(guilec_opts) \
	--output=$@ $<

CLEANFILES = $(GOBJECTS)

# Handy way to remove the .go files without removing all the rest.
clean-go:
	@cd dist; make clean-go
	-$(RM) -f $(GOBJECTS)
.PHONY: clean-go

## Makefile.am ends here.
