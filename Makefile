#
# 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# This file is part of Depression Glass library.
#
# Copyright (c) 2002. Dimitry Kloper <kloper@users.sf.net>
#
# Makefile.in -- template for a root Makefile 
#

SHELL = /bin/sh


srcdir = .
top_srcdir = .

RM= rm -f
ETAGS = etags

SUBDIRS = getopt regex dgd test

all: subdirs

.PHONY: subdirs $(SUBDIRS) clean depend etags

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

subdirs: $(SUBDIRS)

clean: $(SUBDIRS)
	$(RM) vc60.*

depend: $(SUBDIRS)

etags:
	$(ETAGS) -R dgd getopt

configure: configure.ac aclocal.m4
	cd $(srcdir) && autoconf

Makefile: Makefile.in config.status
	./config.status

config.status: configure
	./config.status --recheck

