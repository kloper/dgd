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
# Makefile.in -- template for a getopt library Makefile 
#

SHELL = /bin/sh


srcdir     = .
top_srcdir = ..
BUILDDIR   = $(srcdir)/Debug

VPATH += :$(srcdir)::$(srcdir)/src:$(BUILDDIR)

RM  = rm -f
MKDIR = mkdir -p
CC = cl
LD  = link
CFLAGS =  -nologo -W3 -GR -GX -MTd -Gm  -Zi -Od -Fd$(BUILDDIR)/$(PDBFILE) -Fp$(BUILDDIR)/$(PCHFILE)
LDFLAGS  = /lib  /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT /debugtype:cv
LDOUTOPT = /out:
INCLUDES = -I. -I.. -I$(top_srcdir)
PERL = /d/perl/bin/perl.exe
DOXYGEN = /c/Progra~1/doxygen/bin/doxygen.exe

SUBDIRS =
TARGETS = all

all:  doxygen docs subdirs

.PHONY: subdirs $(SUBDIRS) clean depend builddir doxygen webmake

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

subdirs: $(SUBDIRS)

clean: $(SUBDIRS)
	-$(RM) -r $(BUILDDIR)

depend: $(SUBDIRS)

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

doxygen/dgd.doxy: doxygen/dgd.doxy.in
	cd $(top_srcdir) && ./config.status

docsys/dgd/config: docsys/dgd/config.in
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status

doxygen:: doxygen/dgd.doxy
	cd doxygen && $(DOXYGEN) dgd.doxy

docs:: docsys/dgd/config
	cd docsys && $(PERL) ./docsys dgd && $(PERL) ./docsys doxy

