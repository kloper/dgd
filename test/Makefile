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

# Copyright (c) 2002. Dimitry Kloper <kloper@users.sf.net>
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

CXXFLAGS =   -nologo -Zm300 -W3 -GR -GX -TP -MTd -Gm  -Zi -Od -Fd$(BUILDDIR)/$(PDBFILE) -Fp$(BUILDDIR)/$(PCHFILE)
LDFLAGS  =   /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT /debugtype:cv
EXTRA_LDFLAGS =  -libpath:$(top_srcdir)/./../STLport-4.5/lib -libpath:$(top_srcdir)/dgd/Debug -libpath:$(top_srcdir)/regex/Debug -libpath:$(top_srcdir)/getopt/Debug 
LDLIBS = getopt.lib regex.lib dgd.lib
LDOUTOPT = /out:
COUTOPT  = -Fo
INCLUDES = $(LOCALINCLUDES) $(STLINCLUDES) \
$(BOOSTINCLUDES) $(GETOPTINCLUDES) $(DGDINCLUDES)
LOCALINCLUDES = -I. -I.. -I../.. -I$(top_srcdir) 
STLINCLUDES = -I $(STLDIR)/stlport
BOOSTINCLUDES = -I $(BOOSTDIR)
GETOPTINCLUDES = -I $(top_srcdir)/getopt/src
DGDINCLUDES    = -I $(top_srcdir)/dgd/src

DEFINES = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DWIN32=1 -D_WINDOWS=1 -D_DEBUG=1  -D_TRACE $(BOOSTDEFINES)

LIBPREFIX = 
LIBEXT = lib
OBJEXT = obj
EXEEXT = .exe

BOOSTDEFINES =

TARGET   = dgdtest$(EXEEXT)
CPPFILES = \
dgTest.cpp 

OBJECTS = ${patsubst %.cpp, %.$(OBJEXT), $(CPPFILES)}

PDBFILE  = dgdtest.pdb 
PCHFILE  = dgdtest.pch

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
	$(MAKEDEPEND) $(LOCALINCLUDES) $(DGDINCLUDES) $(DEFINES) -f- -o.$(OBJEXT) -p$(BUILDDIR)/ $^ 2> $@.err | $(SED) 's%$(BUILDDIR)/src/%%' > $@ 

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status


$(TARGET): $(OBJECTS) ../dgd/$(BUILDDIR)/$(LIBPREFIX)dgd.$(LIBEXT)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) /pdb:$(BUILDDIR)/$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)}  $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(CXX) $(LDFLAGS)  ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)} $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $(COUTOPT)$(BUILDDIR)/$@ $<

include .depend







