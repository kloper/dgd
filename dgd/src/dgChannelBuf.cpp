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

#include "dgChannelBuf.h"

namespace DGD {

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

int channelbuf::sync() {
   overflow( traits_type::eof() );

   if( pptr() != pbase() ) {
      propagate( pbase(), pptr() );
      setp( pbase(), epptr() );
      m_word_pos = pbase();
   }

   return 0;
}

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
   
void channelbuf::propagate( const char_type* begin, const char_type* end ) {
   for( Assoc_list::iterator i = m_assoc.begin(); i != m_assoc.end(); ++i ) {
      (*i)->write( begin, end-begin );
   }
}

void channelbuf::propagate( const char_type ch, std::streamsize size ) {
   for( Assoc_list::iterator i = m_assoc.begin(); i != m_assoc.end(); ++i ) {
      for( int x = 0; x < size; ++x )
	 (*i)->write( &ch, 1 );
   }
}

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

channelbuf::~channelbuf() {
   sync();
}

void channelbuf::assoc( const stream& dgs ) {
   m_assoc.push_back( dgs );
}

unsigned int channelbuf::line() const {
   return m_line;
}

unsigned int channelbuf::column() const {
   return m_column;
}

void channelbuf::indent_step( unsigned int step ) {
   sync();
   m_indent_step = step;
}

unsigned int channelbuf::indent_step() const {
   return m_indent_step;
}

void channelbuf::incr_indent() {
   sync();
   m_indent += m_indent_step;
}

void channelbuf::decr_indent() {
   sync();
   m_indent -= m_indent_step;
}

void channelbuf::indent( const unsigned int val ) {
   sync();
   m_indent = val;
}

unsigned int channelbuf::indent() const {
   return m_indent;
}

void channelbuf::min_width( unsigned int width ) {
   sync();
   m_min_width = width;
}

unsigned int channelbuf::min_width() const {
   return m_min_width;
}

void channelbuf::max_width( unsigned int width ) {
   sync();
   m_max_width = width;
}

unsigned int channelbuf::max_width() const {
   return m_max_width;
}

void channelbuf::wrap( bool allow ) {
   sync();
   m_wrap = allow;
}

bool channelbuf::wrap() const {
   return m_wrap;
}

void channelbuf::word_wrap( bool allow ) {
   sync();
   m_word_wrap = allow;
}

bool channelbuf::word_wrap() const {
   return m_word_wrap;
}

void channelbuf::space_chars(const char* spc ) {
   sync();
   m_spaces = spc;
}

std::string channelbuf::space_chars() const {
   return m_spaces;
}

channelbuf::position_type channelbuf::position() const {
   return position_type( m_line, m_column );
}

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


