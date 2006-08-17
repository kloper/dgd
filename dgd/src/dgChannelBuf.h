// -*- c++ -*-
//
// 
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// This file is part of Depression Glass library.
//
// Copyright (c) 2002-2006. Dimitry Kloper <kloper@users.sf.net> . 
//
// dgChannelBuf.h -- streambuf which acts as a channel
//

#ifndef _dgChannelBuf_h_
#define _dgChannelBuf_h_

/**
 * @file dgChannelBuf.h
 *
 * Declaration of DGD::channelbuf
 */

#include <iostream>
#include <list>
#include <string>

namespace DGD {

/**
 * Simple formatting std::streambuf. This is a core class of DGD. Here
 * actual output formatting takes place. Please refer to DGD::channel
 * for general explanation on the channel formatting
 * capabilities. The discussion below is focused on technical aspects
 * of the formatting.
 *
 * DGD::channelbuf has it's own memory buffer which can be controlled
 * by channelbuf::setbuf(char_type*,std::streamsize) method. All
 * channel input (see DGD::channel for the terminology) formatted by
 * std::ostream operators is put into it. DGD::channelbuf
 * scans the buffer (see channelbuf::overflow(int_type)) and makes the
 * additional formatting.
 *
 * DGD::channelbuf acts as a stream multiplexor. It maintains a list of
 * DGD::stream objects which are "associated" with this channel  buffer 
 * (see channelbuf::assoc(std::ostream*)). If the list is empty no
 * output is produced. The channelbuf output is copied to all objects 
 * on the list when it is ready.
 * 
 * std::ostream object can be associated with DGD::channelbuf by
 * calling channelbuf::assoc(std::ostream*) method, but it is more
 * appropriate to use DGD::assoc(std::ostream*,channel&) or 
 * assoc(std::ostream*,const std::string&) functions. 
 *
 * Since channelbuf usually can't figure out whether its input
 * contains a complete line or only part of it, channelbuf does not
 * flush the entire buffer, but only part of it. To keep track of
 * current state of the output it has m_line and m_column variables which
 * can be queried using channelbuf::position() method. 
 */
class channelbuf: public std::streambuf {
   public:
      enum Assoc_type { 
	 Assoc_Append = 0,
	 Assoc_Assign,
	 Assoc_Prepend,
      };

      typedef std::streambuf Parent;
      typedef std::pair<unsigned,unsigned> position_type;
#if defined(__STDC__) && !defined(_STLPORT)
      typedef char char_type;
      typedef int  int_type;
      class traits_type {
	 public:
	    static int_type eof() { return -1; }
	    static void move( char_type* dst, 
			      const char_type* src, 
			      const int_type len ) {
	       memmove( dst, src, len );
	    }
	    static const char_type* find( const char_type* src, 
				    const int_type len,
				    const char_type ch ) {
	       return (const char_type*)memchr( src, ch, len );
	    }
      };
#endif

   protected:
      typedef std::list< std::ostream* > Assoc_list;

   protected:
      /*@{*/
      /**
       * std::streambuf interface
       */

      virtual Parent* setbuf(char_type*, std::streamsize);

      virtual int sync();

      virtual int_type overflow(int_type = traits_type::eof());
      /*@}*/

      virtual void propagate( const char_type* begin, 
			      const char_type* end );
      virtual void propagate( const char_type ch,
			      std::streamsize size );

      virtual void post_process();

   public:
      enum DefaultValues { 
	 DefaultBufferSize = 100,
	 DefaultMaxWidth   = 79,
	 DefaultMinWidth   = 20,
	 DefaultIndentStep = 8 
      };
      
   public:
      channelbuf();
      ~channelbuf();

      void         assoc( std::ostream*, 
			  const Assoc_type action = Assoc_Append );
      bool         dassoc( std::ostream* );
      void         dassoc( );

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

      position_type position() const;
      unsigned long bytes_written() const;

#if defined(_CYGWIN) && !defined(_STLPORT)
      Parent* pubsetbuf(char_type* b, std::streamsize s) {
	 return setbuf(b, s);
      }
#endif

   protected:
      char_type* find_one_of( const char_type* s, const unsigned int n,
			      const char_type* c_set ) const;
      bool is_dos_eol( const char_type* p ) const;

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
      unsigned long m_bytes;
};

};

#endif /* _dgChannelBuf_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgChannelBuf.obj"
 * End:
 */



