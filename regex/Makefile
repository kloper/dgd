#
# $Id$
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
# Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il>
#
# Makefile.in -- template for a regex library Makefile 
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
CFLAGS =  -nologo -W3 -GR -GX -MTd -DWIN32=1 -D_WINDOWS=1 -D_DEBUG=1 
LDFLAGS  = /lib  /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT /debugtype:cv
LDOUTOPT = /out:
INCLUDES = -I. -I.. -I$(top_srcdir)

SUBDIRS =
TARGETS = regex.lib
PDBFILE = regex.pdb
PCHFILE = regex.pch

HEADERS = regex.h
CFILES  = regex.c 
OBJECTS = ${patsubst %.c, %.obj, $(CFILES)}

all:  builddir $(TARGETS) subdirs

.PHONY: subdirs $(SUBDIRS) clean depend builddir

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

builddir:
	-$(MKDIR) $(BUILDDIR)

subdirs: $(SUBDIRS)

clean: $(SUBDIRS)
	-$(RM) -r $(BUILDDIR)

depend: $(SUBDIRS)

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status

$(TARGETS): $(OBJECTS)
	$(LD) $(LDFLAGS) ${patsubst %.obj, $(BUILDDIR)/%.obj, $(OBJECTS)} $(LDOUTOPT)$(BUILDDIR)/$@

%.obj: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -Fo$(BUILDDIR)/$@  -Fd$(BUILDDIR)/$(PDBFILE) -Fp$(BUILDDIR)/$(PCHFILE) $<



