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
// dgChannelBuf.cpp -- implementation for dgChannelBuf.h
//

/**
 * @file dgChannelBuf.cpp
 *
 * implementation of DGD::channelbuf
 */

#include "dgChannelBuf.h"

namespace DGD {

/** 
 * Sets or resets channelbuf internal buffer.
 * @param ptr - if NULL causes automatic buffer allocation.
 * @param size - if zero DefaultBufferSize is used. Otherwise
 * defines the allocated buffer size.
 */
channelbuf::Parent* 
channelbuf::setbuf( char_type* ptr , std::streamsize size) {
   if( pbase() != NULL ) {
      delete pbase();
   }
   
   char_type* buffer = NULL;
   std::streamsize bsize = 0;

   if( size == 0 ) {
      bsize = DefaultBufferSize;
   } else {
      bsize = size;
   }

   if( ptr == NULL ) {
      buffer = new char_type[ bsize ];
   } else {
      buffer = ptr;
   }

   setp( buffer, buffer + bsize );
   return this;
}

/** 
 * Synchronizes (i.e. flushes) the buffer.  
 * @return always 0
 */

int channelbuf::sync() {
   overflow( traits_type::eof() );

   if( pptr() != pbase() ) {
      propagate( pbase(), pptr() );
      setp( pbase(), epptr() );
      m_word_pos = pbase();
   }

   return 0;
}

/**
 * Called when there is no write position.  This function overrides
 * the same function in std::streambuf. This function is called
 * every time there is no enough space in the internal buffer. 
 *
 * Once called, this function processes the content of the buffer,
 * makes formatting decisions and then propagates the formatted output
 * to associated streams.
 *
 * @param ch - character which failed to be pushed into stream. could
 * be set to char_traits::eof value to indicate empty value.
 *
 * @return always 0
 * @see DGD::channel
 */
channelbuf::int_type channelbuf::overflow( int_type ch ) {   
   const char_type* pos = pbase();
   bool run_flag = true;

   do {
      unsigned int indent = std::min( m_indent, m_max_width - m_min_width );
      if( m_column < indent && pos < pptr() ) {
	 propagate( ' ', indent - m_column );
	 m_column = indent;	 
      }

      const char_type* nearest = NULL;
      const char_type* space_pos = find_one_of( pos, pptr()-pos, 
						m_spaces.c_str() );
      const char_type* nl_pos    = find_one_of( pos, pptr()-pos, "\r\n" );
      
      bool space_nearest = ( 
	 ( space_pos != NULL ) &&
	 ( nl_pos == NULL || space_pos < nl_pos ) );
      if( space_nearest ) nearest = space_pos;
      
      bool nl_nearest = ( 
	 ( nl_pos != NULL ) &&
	 ( space_pos == NULL || nl_pos < space_pos ) );
      if( nl_nearest ) nearest = nl_pos;
      
      if( nearest != NULL ) {
	 if( m_column + (nearest-pos+1) < m_max_width ) {
	    propagate( pos, nearest+1 );
	    if( nl_nearest ) {
	       m_column = 0;
	       m_line++;
	    } else {
	       m_word_pos = nearest+1;
	       m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
	    }
	    pos = nearest + (is_dos_eol( nearest )?2:1);
	 } else if( m_column + (nearest-pos+1) == m_max_width ) {
	    propagate( pos, nearest );
	    if( m_wrap ) {	       
	       pos = nearest;
	       propagate( '\n', 1 );
	       m_column = 0;
	       m_line++;
	    } else {
	       if( nl_nearest ) {
		  propagate( '\n', 1 );
		  m_column = 0;
		  m_line++;
	       } else {
		  m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
	       }
	       pos = nearest + (is_dos_eol( nearest )?2:1);
	    }
	    m_word_pos = pos;
	 } else { // m_column + (nearest-pos+1) > m_max_width
	    if( m_wrap ) {
	       if( m_column >= m_max_width ) {
		  propagate( '\n', 1 );
		  m_column = 0;
		  m_line++;
	       } else if( m_word_wrap ) {
		  unsigned int prop_size =
		     std::min( (unsigned)(m_word_pos-pos), 
			       m_max_width-m_column );
		  propagate( pos, pos + prop_size );
		  propagate( '\n', 1 );
		  pos += prop_size;
		  m_column = 0;
		  m_line++;
		  m_word_pos = nearest+1;
	       } else {
		  propagate( pos, pos + m_max_width-m_column );
		  propagate( '\n', 1 );
		  pos += m_max_width-m_column;
		  m_column = 0;
		  m_line++;
		  m_word_pos = nearest+1;
	       }
	    } else {
	       propagate( pos, nearest+1 );
	       if( nl_nearest ) {
		  m_column = 0;
		  m_line++;
	       } else {
		  m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
	       }
	       m_word_pos = nearest+1;
	       pos = nearest + (is_dos_eol( nearest )?2:1);
	    }
	 }
      } else {
	 unsigned int remaining_len = pptr()-pos;
	 if( remaining_len < (unsigned)(epptr()-pbase()) ) {
	    if( remaining_len > 0 ) {
	       if( pos > pbase() )
		  traits_type::move( pbase(), pos, remaining_len );
	       setp( pbase(), epptr() );
	       pbump( remaining_len );
	    } else {
	       setp( pbase(), epptr() );
	    }
	    m_word_pos = pbase();
	    if( ch != traits_type::eof() )
	       sputc( ch );
	    run_flag = false; // break the main loop
	 } else {
	    if( m_column >= m_max_width ) {
	       propagate( '\n', 1 );
	       m_column = 0;
	       m_line++;
	    } else if( m_wrap ) {
	       propagate( pos, pos + (m_max_width - m_column) );
	       propagate( '\n', 1 );
	       m_column = 0;
	       m_line++;
	       pos += m_max_width - m_column;
	    } else {
	       propagate( pos, pos + remaining_len );
	       m_column += remaining_len;
	       pos += remaining_len;
	    }
	    m_word_pos = pbase();
	 }	 
      }

   } while( run_flag );

   return 0;
}
 
/**
 * Propagate character string to associated streams. 
 * @param begin - pointer to beginning of propagated string
 * @param end   - pointer to the end of the propagated string (one
 * character after the end!)
 */
void channelbuf::propagate( const char_type* begin, const char_type* end ) {
   for( Assoc_list::iterator i = m_assoc.begin(); i != m_assoc.end(); ++i ) {
      (*i)->write( begin, end-begin );
   }
}

/**
 * Propagate given character to associated streams. 
 * @param ch - character to propagate
 * @param size - number of duplications.
 */
void channelbuf::propagate( const char_type ch, std::streamsize size ) {
   for( Assoc_list::iterator i = m_assoc.begin(); i != m_assoc.end(); ++i ) {
      for( int x = 0; x < size; ++x )
	 (*i)->write( &ch, 1 );
   }
}

/**
 * Default constructor 
 */
channelbuf::channelbuf() :
   Parent(),
   m_indent_step( DefaultIndentStep ),
   m_indent(0),
   m_line(0),
   m_column(0),
   m_min_width( DefaultMinWidth ),
   m_max_width( DefaultMaxWidth ),
   m_wrap( true ),
   m_word_wrap( true ),
   m_word_pos(NULL),
   m_spaces( " \t" ) {
}

/**
 * Destructor. Note that channelbuf tries to sync itself before being 
 * destructed.
 */
channelbuf::~channelbuf() {
   sync();
}

/**
 * Append the given stream to the association list.
 * @see DGD::channel
 */
void channelbuf::assoc( const stream& dgs ) {
   m_assoc.push_back( dgs );
}

/**
 * Return current channel line
 */
unsigned int channelbuf::line() const {
   return m_line;
}

/**
 * Return current channel column
 */
unsigned int channelbuf::column() const {
   return m_column;
}

/**
 * Change indentation step. @note sync() is called before the value is
 * actually changed. So this operation affects only future input.
 * @see channel::indent_step(unsigned int)
 */
void channelbuf::indent_step( unsigned int step ) {
   sync();
   m_indent_step = step;
}

/**
 * Return indentation step value.
 * @see channel::indent_step() const
 */
unsigned int channelbuf::indent_step() const {
   return m_indent_step;
}

/**
 * Increment indentation level by value of indentation step. @note
 * sync() is called before the value is actually changed. So this
 * operation affects only future input.
 * @see channel::incr_indent()
 */
void channelbuf::incr_indent() {
   sync();
   m_indent += m_indent_step;
}

/**
 * Decrement indentation level by value of indentation step. @note
 * sync() is called before the value is actually changed. So this
 * operation affects only future input.
 * @see channel::decr::indent()
 */
void channelbuf::decr_indent() {
   sync();
   m_indent -= m_indent_step;
}

/**
 * Change indentation level directly. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::indent(const unsigned int)
 */
void channelbuf::indent( const unsigned int val ) {
   sync();
   m_indent = val;
}

/**
 * Return indentation level value.
 * @see channel::indent() const
 */
unsigned int channelbuf::indent() const {
   return m_indent;
}

/**
 * Change minimum line width. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::min_width(unsigned int)
 */
void channelbuf::min_width( unsigned int width ) {
   sync();
   m_min_width = width;
}

/**
 * Return minimum line width value.
 * @see channel::min_width() const
 */
unsigned int channelbuf::min_width() const {
   return m_min_width;
}

/**
 * Change maximum line width. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::max_width(unsigned int)
 */

void channelbuf::max_width( unsigned int width ) {
   sync();
   m_max_width = width;
}

/**
 * Return maximum line width value.
 * @see channel::max_width() const
 */
unsigned int channelbuf::max_width() const {
   return m_max_width;
}

/**
 * Set character wrapping. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::wrap(bool)
 */
void channelbuf::wrap( bool allow ) {
   sync();
   m_wrap = allow;
}

/**
 * Return character wrapping state.
 * @see channel::wrap() const
 */
bool channelbuf::wrap() const {
   return m_wrap;
}


/**
 * Set word wrapping. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::word_wrap(bool)
 */
void channelbuf::word_wrap( bool allow ) {
   sync();
   m_word_wrap = allow;
}

/**
 * Return word wrapping state.
 * @see channel::word_wrap() const
 */
bool channelbuf::word_wrap() const {
   return m_word_wrap;
}


/**
 * Set space character set. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::space_chars(const char*)
 */

void channelbuf::space_chars(const char* spc ) {
   sync();
   m_spaces = spc;
}

/**
 * Return space characters set.
 * @see channel::space_chars() const
 */
std::string channelbuf::space_chars() const {
   return m_spaces;
}

/**
 * Return current channel line and column.
 */
channelbuf::position_type channelbuf::position() const {
   return position_type( m_line, m_column );
}

/**
 * strchr() equivalent. Given string s with length n, finds a first
 * occurrence of any character found in null-terminated string c_set.
 */
channelbuf::char_type* channelbuf::find_one_of( const char_type* s, 
						const unsigned int n,
						const char_type* c_set ) const
{
   const char_type* res = NULL;

   while( *c_set != '\0' ) {
      const char_type* local_res = traits_type::find( s, n, *c_set );
      if( res == NULL )
	 res = local_res;
      else if( local_res != NULL )
	 res = std::min( res, local_res );
      c_set++;
   }
   
   return (char_type*)res;
}

bool channelbuf::is_dos_eol( const char_type* p ) const {
   if( (*p == '\n' && *(p+1) == '\r') || (*p == '\r' && *(p+1) == '\n') )
      return true;
   return false;
}

}; // end of namespace DGD

// 
// Local Variables:
// compile-command: "make dgChannelBuf.obj"
// End:
//


