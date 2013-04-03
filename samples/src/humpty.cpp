// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
//
// humpty.cpp -- humpty dll
//

#include <dgd.h>

#include "humpty.h"

HUMPTY_EXPORT void humpty_init() {
   dgd_scope;

   dgd_logger << "Hello! This is Humpty Init!" << std::endl;
}

HUMPTY_EXPORT void humpty_do_work() {
   dgd_scope;

   dgd_logger << "Humpty does some work!" << std::endl;
}
   

// 
// Local Variables:
// compile-command: "make humpty.obj"
// End:
//


