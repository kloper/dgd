// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
//
// main_exec.cpp -- this is a main program that will use humpty and dumpty dlls
//

#include <boost/exception/diagnostic_information.hpp> 

#include <dgd.h>

#include "humpty.h"
#include "dumpty.h"

//std::vector<std::string> dgd_init(int argc, char **argv);

int main(int argc, char **argv)
{
   try {
      dgd::controller<char>::init(argc, argv);
   }
   catch(...) {
      std::cerr << "Unable to init dgd. Try --trace-help"
                << std::endl
                << boost::current_exception_diagnostic_information() << std::endl;
      return -1;
   }

   dgd_scope;

   dgd_logger << "Hello! This is a main program" << std::endl;

   humpty_init();

   dumpty_init();

   humpty_do_work();

   dumpty_do_work();

   dgd_logger << "Main is ending" << std::endl;

   return 0;
}



// 
// Local Variables:
// compile-command: "make main_exec.obj"
// End:
//


