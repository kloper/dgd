// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// humpty.h -- humpty dll
// 

#ifndef _humpty_h_
#define _humpty_h_

#ifndef HUMPTY_DLL
#define HUMPTY_EXPORT __declspec(dllimport)
#else
#define HUMPTY_EXPORT __declspec(dllexport)
#endif

HUMPTY_EXPORT void humpty_init();
HUMPTY_EXPORT void humpty_do_work();

#endif /* _humpty_h_ */

/* 
 * Local Variables:
 * compile-command: "make humpty.obj"
 * End:
 */


