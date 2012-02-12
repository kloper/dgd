// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_filter.h -- filter traits type used for filtering out scope messages
// 

#ifndef _dg_filter_h_
#define _dg_filter_h_

namespace dgd {

class filter_traits {
   public:
      typedef unsigned int filter_type;
      typedef unsigned int value_type;

   public:

      static bool match(const filter_type &filter, const value_type& value) {
         if(is_any(value)) {
            return true;
         }
         return ((filter & value) != 0);
      }

      static bool is_any(const value_type& value) {
         return (value == (value_type)-1);
      }

      static value_type value_any() {
         return value_type(-1);
      }

      static filter_type allow_all() {
         return (filter_type)-1;
      }

      static filter_type parse(const std::string& str) {
         filter_type filter = allow_all();

         std::istringstream istr(str);
         istr >> filter;

         return filter;
      }
};

}; // end of namespace dgd

#endif /* _dg_filter_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_filter.obj"
 * End:
 */


