// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_manip.h -- manipulators for the channel
// 

#ifndef _dg_manip_h_
#define _dg_manip_h_

namespace dgd {

struct wrapper_manip_nop {
};

struct wrapper_manip_pos {};
struct wrapper_manip_indent_step { 
      wrapper_manip_indent_step(unsigned int val) : value(val) {}      
      unsigned int value;
};
struct wrapper_manip_incr_indent {};
struct wrapper_manip_decr_indent {};
struct wrapper_manip_indent {
      wrapper_manip_indent(unsigned int val) : value(val) {}      
      unsigned int value;
};
struct wrapper_manip_min_width {
      wrapper_manip_min_width(unsigned int val) : value(val) {}      
      unsigned int value;
};
struct wrapper_manip_max_width {
      wrapper_manip_max_width(unsigned int val) : value(val) {}      
      unsigned int value;
};
struct wrapper_manip_wrap {
      wrapper_manip_wrap(bool val) : value(val) {}      
      bool value;
};
struct wrapper_manip_word_wrap {
      wrapper_manip_word_wrap(bool val) : value(val) {}      
      bool value;
};
struct wrapper_manip_spaces {
      wrapper_manip_spaces(const char* val) : value(val) {}      
      const char* value;
};

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             typename wrapper_filter<char_type>::wrapper_config& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      wrapper_filter<char_type> *wrapper = ch.wrapper();
      if(wrapper != NULL) {
         manip.indent_step(wrapper->indent_step());
         manip.indent(wrapper->indent());
         manip.min_width(wrapper->min_width());
         manip.max_width(wrapper->max_width());
         manip.wrap(wrapper->wrap());
         manip.word_wrap(wrapper->word_wrap());
      }
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const typename wrapper_filter<char_type>::wrapper_config& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      wrapper_filter<char_type> *wrapper = ch.wrapper();
      if(wrapper != NULL) {
         wrapper->indent_step(manip.indent_step());
         wrapper->indent(manip.indent());
         wrapper->min_width(manip.min_width());
         wrapper->max_width(manip.max_width());
         wrapper->wrap(manip.wrap());
         wrapper->word_wrap(manip.word_wrap());
      }
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_pos& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch << "[" << ch.wrapper()->line() << "," << ch.wrapper()->column() << "]";
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_indent_step& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->indent_step(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_incr_indent& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->incr_indent();
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_decr_indent& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->decr_indent();
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_indent& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->indent(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_min_width& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->min_width(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_max_width& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->max_width(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_wrap& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->wrap(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_word_wrap& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->word_wrap(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
inline std::basic_ostream<char_type>& 
operator <<( std::basic_ostream<char_type>& cnl, 
             const wrapper_manip_spaces& manip ) {
   try 
   {
      channel<char_type>& ch = dynamic_cast<channel<char_type>&>(cnl);
      ch.flush();
      ch.wrapper()->spaces(manip.value);
   } catch( std::bad_cast& ) {
   }
   return cnl;
}

template <typename char_type>
typename wrapper_filter<char_type>::wrapper_config&
save(typename wrapper_filter<char_type>::wrapper_config& state) 
{
   return state;
}

template <typename char_type>
const typename wrapper_filter<char_type>::wrapper_config&
restore(const typename wrapper_filter<char_type>::wrapper_config& state) 
{
   return state;
}

const wrapper_manip_pos position = wrapper_manip_pos();

wrapper_manip_indent_step indentstep(unsigned int step) {
   return wrapper_manip_indent_step(step);
}

const wrapper_manip_incr_indent incr = wrapper_manip_incr_indent();

const wrapper_manip_decr_indent decr = wrapper_manip_decr_indent();

wrapper_manip_indent indent(unsigned int value) {
   return wrapper_manip_indent(value);
}

wrapper_manip_min_width minwidth(unsigned int value) {
   return wrapper_manip_min_width(value);
}

wrapper_manip_max_width maxwidth(unsigned int value) {
   return wrapper_manip_max_width(value);
}

wrapper_manip_wrap wrap(bool value) {
   return wrapper_manip_wrap(value);
}

wrapper_manip_word_wrap wordwrap(bool value) {
   return wrapper_manip_word_wrap(value);
}

wrapper_manip_spaces spaces(const char* value) {
   return wrapper_manip_spaces(value);
}

}; // end of namespace DGD

#endif /* _dgChannelManip_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgChannelManip.obj"
 * End:
 */


