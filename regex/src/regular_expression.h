// -*- c++ -*-
//
// $Id$
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// This file is part of Depression Glass library.
//
// Copyright (c) 2002. Dimitry Kloper <kloper@users.sf.net> . 
//
// regular_expression.h -- simple c++ wrap around for regexp
//

#ifndef _regular_expression_h_
#define _regular_expression_h_

#include <iostream>

extern "C" {
#if defined(__STDC__)
#include <sys/types.h>
#include <string.h>
#endif 

#include "regex.h"
}

#if defined(__STDC__) && !defined(_STLPORT)
#include <exception>
#include <algorithm>
#endif

namespace DGD {

class bad_expression: public std::exception {
   public:
      bad_expression() : m_what( "bad regular expression" ) {}
      bad_expression( const char* s ) : m_what( s ) {}

      const char* what() const DGD_THROW { 
	 return m_what;
      }
   private:
      const char* m_what;
};



class regex {
   public:
      friend bool regex_match( regex& rx, const char* str );

      regex( const char* rx ) {
	 if( rx == NULL ) 
	    throw bad_expression( "null string given" );

	 std::fill( (char*)&m_regexp, (char*)&m_regexp + sizeof(m_regexp), 0 );
	 const char* reason = 
	    re_compile_pattern( rx, strlen( rx ), &m_regexp );
	 if( reason != NULL )
	    throw bad_expression( reason );
      };
      
   private:
      re_pattern_buffer m_regexp;
};

inline bool regex_match( const char* str, regex& rx ) {
   int res = re_match( &(rx.m_regexp), str, strlen(str), 0, NULL );
   switch( res ) {
      case 0:
      case -1:
	 return false;
      case -2:
	 throw bad_expression( "internal error" );
   }
   return true;
}

}; // end of namespace DGD

#endif /* _regular_expression_h_ */

/* 
 * Local Variables:
 * compile-command: "make regular_expression.obj"
 * End:
 */


