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

# Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il>
#
# Makefile.in -- template for a VMC comand line Makefile 
#

SHELL = /bin/sh


srcdir = .
top_srcdir = ..
STLDIR     = $(top_srcdir)/./../STLport-4.5
BOOSTDIR   = $(top_srcdir)/./../boost
BUILDDIR   = $(srcdir)/Debug

RM = rm -f
MV = mv
MKDIR = mkdir -p
CXX = cl
LD  = link
MAKEDEPEND = makedepend
SED = sed

CXXFLAGS =   -nologo -Zm300 -W3 -GR -GX -TP -MTd -Gm  -Zi -Od
LDFLAGS  =   /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT /debugtype:cv
EXTRA_LDFLAGS =  -libpath:$(top_srcdir)/./../STLport-4.5/lib -libpath:$(top_srcdir)/dgd/Debug -libpath:$(top_srcdir)/getopt/Debug 
LDLIBS = getopt.lib dgd.lib
LDOUTOPT = /out:
INCLUDES = $(LOCALINCLUDES) $(STLINCLUDES) \
$(BOOSTINCLUDES) $(GETOPTINCLUDES) $(DGDINCLUDES)
LOCALINCLUDES = -I. -I.. -I../.. -I$(top_srcdir) 
STLINCLUDES = -I $(STLDIR)/stlport
BOOSTINCLUDES = -I $(BOOSTDIR)
GETOPTINCLUDES = -I $(top_srcdir)/getopt/src
DGDINCLUDES    = -I $(top_srcdir)/dgd/src

DEFINES = -DWIN32=1 -D_WINDOWS=1 -D_DEBUG=1  -D_TRACE $(BOOSTDEFINES)

BOOSTDEFINES =

TARGET   = $(BUILDDIR)/dgdtest.exe
CPPFILES = \
dgTest.cpp 

OBJECTS = ${patsubst %.cpp, $(BUILDDIR)/%.obj, $(CPPFILES)}

PDBFILE  = $(BUILDDIR)/dgdtest.pdb 
PCHFILE  = $(BUILDDIR)/dgdtest.pch

VPATH += :$(srcdir):$(srcdir)/src:$(BUILDDIR)

.PHONY: all subdirs $(SUBDIRS) clean depend builddir

all: builddir $(TARGET) subdirs

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

builddir:
	$(MKDIR) $(BUILDDIR)

subdirs: $(SUBDIRS)

clean: $(SUBDIRS)
	$(RM) -r $(BUILDDIR) .depend .depend.err

depend: .depend $(SUBDIRS)

.depend: ${patsubst %.cpp, src/%.cpp, $(CPPFILES)}
	$(MAKEDEPEND) $(LOCALINCLUDES) $(DEFINES) -f- -o.obj -p$(BUILDDIR)/ $^ 2> $@.err | $(SED) 's%$(BUILDDIR)/src%$(BUILDDIR)%' > $@ 

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status


$(TARGET): $(OBJECTS) ../dgd/$(BUILDDIR)/dgd.lib
	$(LD) $(LDFLAGS) /pdb:$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) $(OBJECTS) $(LDOUTOPT)$@

$(BUILDDIR)/%.obj: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c -Fo$@ -Fd$(PDBFILE) -Fp$(PCHFILE) $<

include .depend







