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
BUILDDIR   = $(srcdir)/Release

RM = rm -f
MV = mv
MKDIR = mkdir -p
CXX = cl
LD  = link
MAKEDEPEND = makedepend
SED = sed

CXXFLAGS =   -nologo -Zm300 -W3 -GR -GX -TP -MTd -O2 -Fd$(BUILDDIR)/$(PDBFILE) -Fp$(BUILDDIR)/$(PCHFILE)
LDFLAGS  =   /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT 
EXTRA_LDFLAGS =  -libpath:$(top_srcdir)/./../STLport-4.5/lib -libpath:$(top_srcdir)/dgd/Release -libpath:$(top_srcdir)/regex/Release -libpath:$(top_srcdir)/getopt/Release 
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

DEFINES = -DWIN32=1 -D_WINDOWS=1 -DNODEBUG=1  -D_TRACE $(BOOSTDEFINES)

LIBEXT = lib
OBJEXT = obj
EXEEXT = .exe

BOOSTDEFINES =

CPPFILES = \
stand_alone_channelbuf.cpp \
formatting.cpp

OBJECTS = ${patsubst %.cpp, %.$(OBJEXT), $(CPPFILES)}

PDBFILE  = samples.pdb 
PCHFILE  = samples.pch

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
	$(MAKEDEPEND) $(LOCALINCLUDES) $(DEFINES) -f- -o.$(OBJEXT) -p$(BUILDDIR)/ $^ 2> $@.err | $(SED) 's%$(BUILDDIR)/src%$(BUILDDIR)%' > $@ 

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status


sa_chbuf$(EXEEXT): stand_alone_channelbuf.$(OBJEXT) ../dgd/$(BUILDDIR)/dgd.$(LIBEXT)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) /pdb:$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) $(BUILDDIR)/stand_alone_channelbuf.$(OBJEXT)  $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(CXX) $(LDFLAGS) $(BUILDDIR)/stand_alone_channelbuf.$(OBJEXT) $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

fmt$(EXEEXT): formatting.$(OBJEXT) ../dgd/$(BUILDDIR)/dgd.$(LIBEXT)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) /pdb:$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) $(BUILDDIR)/formatting.$(OBJEXT)  $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(CXX) $(LDFLAGS) $(BUILDDIR)/stand_alone_channelbuf.$(OBJEXT) $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $(COUTOPT)$(BUILDDIR)/$@ $<

include .depend







