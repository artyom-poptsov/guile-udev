## Config file for GNU Automake. 
##
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

ACLOCAL_AMFLAGS = -I build-aux/m4 --install

SUBDIRS = . libguile-udev build-aux modules doc tests

EXTRA_DIST =					\
	build-aux/config.rpath			\
	build-aux/compile			\
	.dir-locals.el

gen-ChangeLog:
	if test -d .git; then				\
	  $(top_srcdir)/build-aux/gitlog-to-changelog	\
	    14a843c..HEAD > $(distdir)/cl-t;		\
	  rm -f $(distdir)/ChangeLog;			\
	  mv $(distdir)/cl-t $(distdir)/ChangeLog;	\
	fi

dist-hook: gen-ChangeLog
.PHONY: gen-ChangeLog

clean-go:
	@cd modules; make clean-go
.PHONY: clean-go

## Makefile.am ends here
