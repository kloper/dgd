// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dumpty.h -- dumpty dll
// 

#ifndef _dumpty_h_
#define _dumpty_h_

#ifndef DUMPTY_DLL
#define DUMPTY_EXPORT __declspec(dllimport)
#else
#define DUMPTY_EXPORT __declspec(dllexport)
#endif

DUMPTY_EXPORT void dumpty_init();
DUMPTY_EXPORT void dumpty_do_work();

#endif /* _dumpty_h_ */

/* 
 * Local Variables:
 * compile-command: "make dumpty.obj"
 * End:
 */


