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
STLDIR     = $(top_srcdir)/.
BOOSTDIR   = $(top_srcdir)/./../boost_1_26_0
BUILDDIR   = $(srcdir)/Debug

RM = rm -f
MV = mv
MKDIR = mkdir -p
CXX = g++
LD  = ld
MAKEDEPEND = makedepend
SED = sed

CXXFLAGS =   -Wall -g
LDFLAGS  =   -g
EXTRA_LDFLAGS =  
LDLIBS = ../dgd/$(BUILDDIR)/libdgd.a ../getopt/$(BUILDDIR)/libgetopt.a ../regex/$(BUILDDIR)/libregex.a
LDOUTOPT = -o
COUTOPT  = -o
INCLUDES = $(LOCALINCLUDES) $(STLINCLUDES) \
$(BOOSTINCLUDES) $(GETOPTINCLUDES) $(DGDINCLUDES)
LOCALINCLUDES = -I. -I.. -I../.. -I$(top_srcdir) 
STLINCLUDES = -I $(STLDIR)/stlport
BOOSTINCLUDES = -I $(BOOSTDIR)
GETOPTINCLUDES = -I $(top_srcdir)/getopt/src
DGDINCLUDES    = -I $(top_srcdir)/dgd/src

DEFINES = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DCYGWIN=1 -D_CYGWIN=1 -D_DEBUG=1  -D_TRACE $(BOOSTDEFINES)

LIBEXT = a
OBJEXT = o
EXEEXT = 

BOOSTDEFINES =

CPPFILES = \
stand_alone_channelbuf.cpp \
formatting.cpp \
mult_assoc.cpp \
list_of_lists.cpp

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
	$(CXX) $(LDFLAGS) $(BUILDDIR)/formatting.$(OBJEXT) $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

mult_assoc$(EXEEXT): mult_assoc.$(OBJEXT) ../dgd/$(BUILDDIR)/dgd.$(LIBEXT)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) /pdb:$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) $(BUILDDIR)/mult_assoc.$(OBJEXT)  $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(CXX) $(LDFLAGS) $(BUILDDIR)/mult_assoc.$(OBJEXT) $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

lofl$(EXEEXT): list_of_lists.$(OBJEXT) ../dgd/$(BUILDDIR)/dgd.$(LIBEXT)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS) /pdb:$(PDBFILE) /debug $(EXTRA_LDFLAGS) $(LDLIBS) $(BUILDDIR)/list_of_lists.$(OBJEXT)  $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(CXX) $(LDFLAGS) $(BUILDDIR)/list_of_lists.$(OBJEXT) $(EXTRA_LDFLAGS) $(LDLIBS) $(LDOUTOPT)$(BUILDDIR)/$@
endif

%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $(COUTOPT)$(BUILDDIR)/$@ $<

include .depend







