// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
//
// dumpty.cpp -- dumpty dll
//

#include <dgd.h>

#include "dumpty.h"

DUMPTY_EXPORT void dumpty_init() {
   dgd_scope;

   dgd_logger << "Hello! This is Dumpty Init!" << std::endl;
}

DUMPTY_EXPORT void dumpty_do_work() {
   dgd_scope;

   dgd_logger << "Dumpty does some work!" << std::endl;
}
   

// 
// Local Variables:
// compile-command: "make dumpty.obj"
// End:
//


