// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_expand.h -- utility functions for easier tracing
// 

#ifndef _dg_expand_h_
#define _dg_expand_h_

#define dgd_expand(v) dgd_quote(v) "=" << (v)
#define dgd_expandr(ref) dgd_quote(ref) "=" << dgd::deref(ref)
#define dgd_echo(v) { dgd_logger << dgd_expand(v) << std::endl; }
#define dgd_echor(v) { dgd_logger << dgd_expandr(v) << std::endl; }

namespace dgd
{

template<typename T>
class deref_impl;

template<typename T>
class deref_val;

template<typename T>
deref_val<T> deref(const T& val) {
   return deref_impl<T>::invoke(val);
}


template<typename T>
class deref_val
{
private:
   const T* const m_val;
   bool m_is_pointer;
public:
   deref_val(const T& val) :
      m_val(&val),
      m_is_pointer(false)
   {}

   const T* const get() const { return m_val; }
   bool is_pointer() const { return m_is_pointer; }
};

template<typename T>
class deref_val<T*>
{
private:
   const T* const m_val;
   bool m_is_pointer;
public:
   deref_val(const T* const val) :
      m_val(val),
      m_is_pointer(true)
   {}

   const T* const get() const { return m_val; }
   bool is_pointer() const { return m_is_pointer; }
};

template<typename T>
class deref_impl
{
public:
   static deref_val<T> invoke(const T& val) { 
      return deref_val<T>(val); }
};

} // end of namespace dgd

template<typename T, typename char_type>
std::basic_ostream<char_type> &operator<<(std::basic_ostream<char_type> &ostr, 
                                          const dgd::deref_val<T> &val) 
{
   dgd::channel<char_type> *ch = dynamic_cast< dgd::channel<char_type>* >(&ostr);
   if(val.is_pointer()) {
      ostr << val.get();
      if(val.get() == NULL) {
         return ostr;
      }
      ostr << "->";
   }

   if(ch != NULL) {
      (*ch) << *val.get();
   } else {
      ostr << *val.get();
   }

   return ostr;
}
#endif /* _dg_expand_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_expand.obj"
 * End:
 */


