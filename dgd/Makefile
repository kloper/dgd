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
# Makefile.in -- template for a DGPhrase Makefile 
#

SHELL = /bin/sh


srcdir     = .
top_srcdir = ..
STLDIR     = $(top_srcdir)/.
BOOSTDIR   = $(top_srcdir)/./../boost
BUILDDIR   = $(srcdir)/Debug

VPATH += :$(srcdir):$(srcdir)/src:$(BUILDDIR)

RM  = rm -f
MV  = mv
CAT = cat
SED = sed
MKDIR = mkdir -p
CXX = g++
LD  = ld
AR  = ar
RANLIB = ranlib
MAKEDEPEND = makedepend
GENGETOPT  = gengetopt

CXXFLAGS =  -Wall -g
LDFLAGS  =   -g
LDOUTOPT = -o
COUTOPT  = -o
INCLUDES = $(LOCALINCLUDES) $(STLINCLUDES) $(BOOSTINCLUDES) \
$(GETOPTINCLUDES) $(REGEXINCLUDES)
LOCALINCLUDES = -I. -I.. -I$(top_srcdir) 
STLINCLUDES   = -I $(STLDIR)/stlport
BOOSTINCLUDES = -I $(BOOSTDIR)
GETOPTINCLUDES = -I$(top_srcdir)/getopt/src
REGEXINCLUDES = -I$(top_srcdir)/regex/src
DEFINES = -DCYGWIN=1 -D_CYGWIN=1 -D_DEBUG=1  -D_DG_CPP_FILE_=1 $(BOOSTDEFINES)
BOOSTDEFINES =

LIBEXT = a
OBJEXT = o

SUBDIRS = 
TARGET = dgd.$(LIBEXT)
PDBFILE = dgd.pdb
PCHFILE = dgd.pch

HEADERS = 

CPPFILES = \
dgChannel.cpp \
dgChannelBuf.cpp \
dgDebug.cpp \
dgDebugOpt.cpp \
dgOptionFilter.cpp

GGEFILES = \
dgDebugOpt.gge

OBJECTS = ${patsubst %.cpp, %.$(OBJEXT), $(CPPFILES)}

all:  builddir $(TARGET) subdirs

.PHONY: all subdirs $(SUBDIRS) clean depend builddir

.SECONDARY: ${patsubst %.gge, %.cpp, $(GGEFILES)} \
            ${patsubst %.gge, %.h, $(GGEFILES)}

%.cpp: %.gge
	$(GENGETOPT) --no-handle-help --no-handle-version --no-handle-error --file-name=${basename $@} --func-name=dgd_cmdline_parser < $<
	$(CAT) ${basename $@}.c | $(SED) 's/gengetopt_args_info/dgd_gengetopt_args_info/' | $(SED) 's%src/%%g' > tmp.c
	$(MV) tmp.c $@
	$(RM) ${basename $@}.c
	$(CAT) ${basename $@}.h | $(SED) 's/gengetopt_args_info/dgd_gengetopt_args_info/' | $(SED) 's%src/%%g' > tmp.h
	$(MV) tmp.h ${basename $@}.h

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

builddir:
	-$(MKDIR) $(BUILDDIR)

subdirs: $(SUBDIRS)

clean: $(SUBDIRS)
	-$(RM) -r $(BUILDDIR) .depend .depend.err

depend: .depend $(SUBDIRS)

.depend:${patsubst %.cpp, src/%.cpp, $(CPPFILES)}
	$(MAKEDEPEND) $(LOCALINCLUDES) $(DEFINES) -f- -o.$(OBJEXT) -p$(BUILDDIR)/ $^ 2> $@.err | $(SED) 's%$(BUILDDIR)/src%$(BUILDDIR)%' > $@ 

Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir) && ./config.status

$(top_srcdir)/config.status: $(top_srcdir)/configure $(top_srcdir)/aclocal.m4 \
                             $(top_srcdir)/configure.ac
	cd $(top_srcdir) && $(MAKE) config.status

$(TARGET): $(OBJECTS)
ifeq ($(LIBEXT),lib)
	$(LD) $(LDFLAGS)  ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)} $(LDOUTOPT)$(BUILDDIR)/$@
else
	$(AR) cr $(BUILDDIR)/$@ ${patsubst %.$(OBJEXT), $(BUILDDIR)/%.$(OBJEXT), $(OBJECTS)}
	$(RANLIB) $(BUILDDIR)/$@
endif

%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $(COUTOPT)$(BUILDDIR)/$@ $<

include .depend
