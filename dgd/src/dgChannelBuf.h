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
// Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// dgChannelBuf.h -- streambuf which acts as a channel
//

#ifndef _dgChannelBuf_h_
#define _dgChannelBuf_h_

#include <iostream>
#include <list>
#include <string>

#include "dgStream.h"

namespace DGD {

class channelbuf: public std::streambuf {
   public:
      typedef std::streambuf Parent;

   protected:
      typedef std::list< stream > Assoc_list;

   protected:
      /*@{*/
      /**
       * std::streambuf interface
       */
      /**
       * Called when there is no write position.  All subclasses are
       * expected to override this virtual member function.
       */
      virtual int_type overflow(int_type = traits_type::eof());
      /*@}*/

      virtual void propagate( const char_type* begin, 
			      const char_type* end );

   public:
      enum DefaultValues { 
	 DefaultMaxWidth   = 79,
	 DefaultMinWidth   = 20,
	 DefaultIndentStep = 8 
      };
      
   public:
      channelbuf();

      void         assoc( const stream& );
      unsigned int line() const;
      unsigned int column() const;

      void         indent_step( unsigned int step );
      unsigned int indent_step() const;
      void         incr_indent();
      void         decr_indent();
      void         indent(const unsigned int);
      unsigned int indent() const;
      
      void         min_width( unsigned int width );
      unsigned int min_width() const;
      void         max_width( unsigned int width );
      unsigned int max_width() const;
      
      void         wrap( bool allow );
      bool         wrap() const;
      void         word_wrap( bool allow );
      bool         word_wrap() const;
      
      void         space_chars(const char* spc = " \t");
      std::string  space_chars() const;

   private:
      Assoc_list m_assoc;
      unsigned int m_line;
      unsigned int m_column;
      unsigned int m_indent;
      unsigned int m_indent_step;
      unsigned int m_min_width;
      unsigned int m_max_width;
      bool         m_wrap;
      bool         m_word_wrap;
      const char_type* m_word_pos;
      std::string  m_spaces;
};

};

#endif /* _dgChannelBuf_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgChannelBuf.obj"
 * End:
 */


