dnl -*- m4 -*-
dnl
dnl $Id$
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
dnl
dnl This file is part of Depression Glass library. 
dnl
dnl Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il>
dnl
dnl configure.ac -- templeate for configure script for autoconf
dnl

AC_INIT(dgd, 1.0, dimka@cs.technion.ac.il)
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR(dgd/src/dgConfig.h.in)

win32=0
case "$target" in
*cygwin* | *CYGWIN* | *cygnus* | *CYGNUS*) 
  win32=1 ;;
*)
  AC_ERROR([No support for UNIX yet.]) ;;
esac

AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [compile debuggable version]),,
	enable_debug=yes)

AC_ARG_WITH(cvs,
            AC_HELP_STRING([--with-cvs],
                           [use CVS to retrieve version info] ),
if test "X$withval" = "X"; then
   AC_MSG_ERROR([--with-cvs must have cvs exec as a parameter])
fi
CVS="$withval"
,
AC_CHECK_PROG(CVS, cvs, cvs,,,)
)

DG_VERSION_FROM_CVS

AC_ARG_WITH(stlport,
	    AC_HELP_STRING([--with-stlport],
	                   [search for STLPort, arg is optional]),
if test "X$with_stlport" != "Xno"; then
   if test "X$withval" != "X"; then
      STLDIR=$withval
   fi
else 
   STLDIR=.
fi
,
)

if test $win32 = 1; then 
  AC_ARG_WITH(msvc, 
              AC_HELP_STRING([--with-msvc],
                             [use MS VC++ as compiler/linker]),,
	with_msvc=yes)
  if test "x$with_msvc" = "xno"; then 
     AC_ERROR([Compilers other then MS VC++ are not supported yet.])
  fi

  AC_ARG_WITH(pch, 
              AC_HELP_STRING([--with-pch],
                             [use MS VC++ precompiled headers]),,
	with_pch=no)

  AC_DEFINE(WIN32)
  AC_DEFINE(_WINDOWS)
    
  CXXFLAGS="$CXXFLAGS -nologo -Zm300 -W3 -GR -GX -TP -MTd" 
  CFLAGS="$CFLAGS -nologo -W3 -GR -GX -MTd" 
  if test "x$with_pch" = "xyes"; then
     CXXFLAGS="$CXXFLAGS -YX"
     CFLAGS="$CFLAGS -YX"
  fi
  LDFLAGS="$LDFLAGS /nologo /machine:I386 /NODEFAULTLIB:LIBC /NODEFAULTLIB:LIBCMT"
  if test "x$enable_debug" = "xyes"; then 
     AC_DEFINE(_DEBUG)
     CXXFLAGS="$CXXFLAGS -Gm  -Zi -Od"
     LDFLAGS="$LDFLAGS /debugtype:cv"
     BUILDDIR=Debug
  else
     AC_DEFINE(NODEBUG)
     CXXFLAGS="$CXXFLAGS -O2"
     LDFLAGS="$LDFLAGS /opt:ref"
     BUILDDIR=Release
  fi

  LD_STATIC_FLAGS="/lib"

  LDOUTOPT="/out:"

  AC_SUBST(BUILDDIR)
  AC_SUBST(TARGETS)
  AC_SUBST(CXX_DYNAMIC_FLAGS)
  AC_SUBST(LD_DYNAMIC_FLAGS)
  AC_SUBST(LD_STATIC_FLAGS)
  AC_SUBST(LDOUTOPT)

  AC_LANG(C++)
  AC_PROG_CXX(cl)
  AC_PROG_CC(cl)
  AC_PROG_CXXCPP
  AC_PROG_CPP
  AC_CHECK_PROG(LD, link, link, $CXX) 
fi

DG_GUESS_LOCAL_LIB(STLport, STLDIR)
if test "X$STLDIR" = "X"; then
   AC_ERROR([Can't find STLPort])
fi
AC_SUBST(STLDIR)

DG_GUESS_LOCAL_LIB(boost, BOOSTDIR)
if test "X$BOOSTDIR" = "X"; then
   AC_ERROR([Can't find BOOST])
fi
AC_SUBST(BOOSTDIR)

if test $win32 = 1; then 
   EXTRA_LDFLAGS="$EXTRA_LDFLAGS \
-libpath:\$(top_srcdir)/$STLDIR/lib \
-libpath:\$(top_srcdir)/dgd/$BUILDDIR \
-libpath:\$(top_srcdir)/regex/$BUILDDIR \
-libpath:\$(top_srcdir)/getopt/$BUILDDIR"
   LDLIBS="getopt.lib regex.lib dgd.lib"

   AC_SUBST(EXTRA_LDFLAGS)
   AC_SUBST(LDLIBS)
fi


AC_PROG_MAKE_SET
AC_CHECK_PROG(RM, rm, [rm -f], del,,)
AC_CHECK_PROG(MV, mv, mv, move,, )
AC_CHECK_PROG(CP, cp, [cp -f], copy,, )
AC_CHECK_PROG(CAT, cat, cat, type,, )
AC_CHECK_PROG(SED, sed, sed, [cat --],, )
AC_CHECK_PROG(MKDIR, mkdir, [mkdir -p], md,, )
AC_CHECK_PROG(MAKEDEPEND, makedepend, makedepend, echo,, )
AC_CHECK_PROG(GENGETOPT, gengetopt, gengetopt, echo,, )
AC_CHECK_PROG(RC, rc, rc, echo,, )
AC_CHECK_PROG(ISCC, ISCC, ISCC, ISCC,, )
AC_CHECK_PROG(ETAGS, etags, etags, echo,, )
AC_CHECK_PROG(PERL, perl, perl, perl,, )
AC_CHECK_PROG(WEBMAKE, webmake, webmake, webmake,, )
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, doxygen,, )

AC_CONFIG_FILES(Makefile doc/Makefile doc/html/dgd.wmk doc/doxygen/dgd.doxy getopt/Makefile regex/Makefile dgd/Makefile dgd/src/dgConfig.h dgd/src/dgDebugOpt.gge test/Makefile util/bin/mkdis util/bin/copytree util/bin/html2wmk) 

AC_OUTPUT


