// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_exception.h -- exceptions that are thrown by DGD
// 

#ifndef _dg_exception_h_
#define _dg_exception_h_

namespace dgd {

class exception: public std::exception {
   public:
      exception() throw() : 
         std::exception(),
         m_message("dgd exception")
      { }

      virtual ~exception() throw() {}

      exception(const std::string &msg) throw() : 
         std::exception(),
         m_message(msg)
      { }

      virtual const char* what() const throw() {
         return m_message.c_str();
      }

   private:
      std::string m_message;
};

}; // end of namespace dgd

#endif /* _dg_exception_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_exception.obj"
 * End:
 */


