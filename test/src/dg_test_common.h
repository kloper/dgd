// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_test_common.h -- common utilities for tests
// 

#ifndef _dg_test_common_h_
#define _dg_test_common_h_

/*
 * This dummy template method is required to satisfy gcc and make it accept
 * filtering_stream::component<T>() method. For some reason without this trick
 * gcc complains when trying to use it.
 */
template<typename T>
T* component(int n) {
   throw std::exception("dummy component");
}

std::string wstr2str ( const std::basic_string<char> &str) {
   return str;
}

std::string wstr2str ( const std::basic_string<wchar_t> &str) {
   std::string rc;
   for(unsigned int i = 0; i < str.length(); i++) 
   {
      if(str[i] <= 0x7f) {
         rc += (char)str[i];
      } else {
         std::ostringstream ostr;
         ostr << "{0x" << std::hex << (int)str[i] << "}";
         rc += ostr.str();
      }
   }
   return rc;
}

template<typename char_type>
class literal {
   public:
      std::string source;
      std::basic_string<char_type> target;

      literal( std::string str ) :
         source(str)
      {
         target.resize(str.length(), ' ');
         std::copy( str.begin(), str.end(), target.begin());
      }
};

template <typename char_type>
std::basic_ostream<char_type>& 
operator <<(std::basic_ostream<char_type>& ostr,
           const literal<char_type>& inst) {
   ostr << inst.target;
   return ostr;
}

#endif /* _dg_test_common_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_test_common.obj"
 * End:
 */


