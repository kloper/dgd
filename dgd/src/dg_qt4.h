// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_qt4.h -- compatibility with Qt4
// 

#ifndef _dg_qt4_h_
#define _dg_qt4_h_

inline std::ostream &operator<<(std::ostream &ostr, const QString &str) {
   ostr << qPrintable(str);
   return ostr;
}

#endif /* _dg_qt4_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_qt4.obj"
 * End:
 */


