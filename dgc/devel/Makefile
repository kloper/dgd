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
# Copyright (c) 2002. Dimitry Kloper <kloper@users.sf.net>
#
# Makefile.in -- template for a dgc library Makefile 
#

SHELL = /bin/sh


srcdir     = .
top_srcdir = ../..
BUILDDIR   = $(srcdir)/Debug

VPATH += :$(srcdir)::$(srcdir)/src:$(BUILDDIR)
GPATH += :$(srcdir)/src

RM  = rm -f
MKDIR = mkdir -p
CC = gcc
LD  = ld
AR  = ar
SED = sed
CAT = cat
RANLIB = ranlib
YACC = byacc
MAKEDEPEND = makedepend

CFLAGS =  -Wall -g -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DCYGWIN=1 -D_CYGWIN=1 -D_DEBUG=1 
LDFLAGS  =   -g
LDOUTOPT = -o
COUTOPT  = -o
INCLUDES = -I. -I.. -I../src -I$(top_srcdir)

LIBPREFIX = lib
LIBEXT = a
OBJEXT = o

SUBDIRS =
TARGETS = $(LIBPREFIX)dgc-devel.$(LIBEXT) $
PDBFILE = dgc-devel.pdb
PCHFILE = dgc-devel.pch

HEADERS = \
dgd_dgc_debug.h 
CFILES  = \
dgd_dgc_debug.c 

OBJECTS = ${patsubst %.c, %.$(OBJEXT), $(CFILES)}

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
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)} $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(AR) cr $(BUILDDIR)/$@ ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)}
	$(RANLIB) $(BUILDDIR)/$@
endif

%.$(OBJEXT): %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $(COUTOPT)$(BUILDDIR)/$@ $<

depend: .depend $(SUBDIRS)

.depend:${patsubst %.c, src/%.c, $(CFILES)}
	$(MAKEDEPEND) $(LOCALINCLUDES) $(DEFINES) -f- -o.$(OBJEXT) -p$(BUILDDIR)/ $^ 2> $@.err | $(SED) 's%$(BUILDDIR)/src/%%' > $@ 

include .depend

