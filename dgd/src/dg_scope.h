// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_scope.h -- convenience logger
// 

#ifndef _dg_scope_h_
#define _dg_scope_h_

#define dgd_quote(x) #x
#define dgd_fileline_m(file, line) file ":" dgd_quote(line)
#define dgd_fileline dgd_fileline_m(__FILE__, __LINE__)

#ifndef dgd_logger
#define dgd_logger __dgd_logger
#endif 

#define dgd_scope dgd::scope dgd_logger(dgd_fileline,__func__,NULL)
#define dgd_scopev(text) dgd::scope dgd_logger(dgd_fileline,__func__,(text))

namespace dgd {

template <typename Ch>
class basic_scope {
   public:
      typedef controller<Ch> controller_type;
      typedef typename controller_type::channel_type channel_type;
      typedef typename channel_type::char_type char_type;
      typedef typename channel_type::string_type string_type;

   protected:
      typedef boost::iostreams::basic_null_sink<char_type> null_sink_type;
      typedef boost::iostreams::stream<null_sink_type> onull_type;

   public:
      basic_scope(const char* place,
                  const char* func,
                  const char* text, 
                  unsigned int filter_value = (unsigned int)-1) :
         m_filter_value(filter_value)
      {
         start(place, func, text);
      }

      basic_scope(const char* text, 
                  unsigned int filter_value = (unsigned int)-1) :
         m_filter_value(filter_value)
      {
         start(text);
      }

      ~basic_scope() {
         end();
      }

      void start(const char* place, 
                 const char* func,
                 const char* text) {
         controller_type *ctrl = controller_type::get();
         if(ctrl != NULL && ctrl->enabled() && 
            ctrl->filter(m_filter_value)) {
            channel_type &ch = ctrl->get_channel();
            if(ch.is_open()) {
               if(ch.wrapper()->column() > 
                     ch.wrapper()->indent() * ch.wrapper()->indent_step()) {
                  ch << std::endl;
               }
               ch << dgd::literal<char_type>("{ ").value;
               if(place != NULL) {
                  ch << place;
               } 
               if(func != NULL) {
                  if(place != NULL) 
                     ch << ":";
                  ch << func;
               }
               if(text != NULL) {
                  if(place != NULL || func != NULL) 
                     ch << ":";
                  ch << text;
               }
               ch << std::endl
                  << dgd::incr;
            }
         }
      }

      void start(const char* text) {
         start(NULL, NULL, text);
      }

      void end() {
         controller_type *ctrl = controller_type::get();
         if(ctrl != NULL && ctrl->enabled() &&
            ctrl->filter(m_filter_value)) {
            channel_type &ch = ctrl->get_channel();
            if(ch.is_open()) {
               ch.flush();
               if(ch.wrapper()->column() > 
                      ch.wrapper()->indent() * ch.wrapper()->indent_step()) {
                  ch << std::endl;
               }
               ch << dgd::decr 
                  << dgd::literal<char_type>("}").value
                  << std::endl;
            }
         }
      }

      unsigned int filter_value() const { return m_filter_value; }

   private:
      unsigned int m_filter_value;
};

typedef basic_scope<char> scope;
typedef basic_scope<wchar_t> wscope;

template<typename T, typename char_type>
basic_scope<char_type> &operator << (basic_scope<char_type> &s, const T& data) {
   typename basic_scope<char_type>::controller_type *ctrl = 
      basic_scope<char_type>::controller_type::get();
   if(ctrl == NULL || !ctrl->enabled() || 
      !ctrl->filter(s.filter_value()) ||
      !ctrl->get_channel().is_open()) {
      return s;
   }

   ctrl->get_channel() << data;
   return s;
}

template<typename char_type>
basic_scope<char_type> &operator << (basic_scope<char_type> &s, 
                                     std::basic_ostream<char_type> &(*manip)(std::basic_ostream<char_type>&)) {
   typename basic_scope<char_type>::controller_type *ctrl = 
      basic_scope<char_type>::controller_type::get();
   if(ctrl == NULL ||  !ctrl->enabled() || 
      !ctrl->filter(s.filter_value()) ||
      !ctrl->get_channel().is_open()) {
      return s;
   }

   ctrl->get_channel() << manip;
   return s;
}


}; // end of namespace dgd

#endif /* _dg_scope_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_scope.obj"
 * End:
 */


