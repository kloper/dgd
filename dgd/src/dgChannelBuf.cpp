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

channelbuf::int_type channelbuf::overflow( int_type ch ) {   
   const char_type* pos = pbase();

   do {
      const char_type* nearest = NULL;
      const char_type* space_pos = traits_type::find( pos, epptr()-pos, ' ' );
      const char_type* tab_pos   = traits_type::find( pos, epptr()-pos, '\t' );
      const char_type* nl_pos    = traits_type::find( pos, epptr()-pos, '\n' );
      const char_type* rl_pos    = traits_type::find( pos, epptr()-pos, '\r' );
      
      bool space_nearest = ( 
	 ( space_pos != NULL ) &&
	 ( tab_pos == NULL || space_pos < tab_pos ) &&
	 ( nl_pos == NULL || space_pos < nl_pos ) &&
	 ( rl_pos == NULL || space_pos < rl_pos ) );
      if( space_nearest ) nearest = space_pos;

      bool tab_nearest = ( 
	 ( tab_pos != NULL ) &&
	 ( space_pos == NULL || space_pos < tab_pos ) &&
	 ( nl_pos == NULL || tab_pos < nl_pos ) &&
	 ( rl_pos == NULL || tab_pos < rl_pos ) );
      if( tab_nearest ) nearest = tab_pos;
      
      bool nl_nearest = ( 
	 ( nl_pos != NULL ) &&
	 ( tab_pos == NULL || nl_pos < tab_pos ) &&
	 ( space_pos == NULL || nl_pos < space_pos ) &&
	 ( rl_pos == NULL || nl_pos < rl_pos ) );
      if( nl_nearest ) nearest = nl_pos;

      bool rl_nearest = ( 
	 (rl_pos != NULL) &&
	 ( tab_pos == NULL || rl_pos < tab_pos ) &&
	 ( nl_pos == NULL || rl_pos < nl_pos ) &&
	 ( space_pos == NULL || rl_pos < space_pos ) );
      if( rl_nearest ) nearest = rl_pos; 
      
      if( nearest != NULL ) {
	 if( m_column + (nearest-pos+1) < m_max_width ) {
	    if( nl_nearest || rl_nearest ) {
	       m_column = std::min( m_indent * m_indent_step,
				    m_max_width - m_min_width );
	       m_line++;
	    } else {
	       m_word_pos = nearest;
	       m_column += nearest-pos+1 + (tab_nearest?7:0);
	    }
	    pos = nearest + 1;
	 } else if( m_column + (nearest-pos+1) == m_max_width ) {
	    if( m_wrap ) {
	       if( space_nearest || tab_nearest ) {
		  propagate( pos, nearest );
		  while( *pos == ' ' || *pos == '\t' ) pos++;
	       } else if( nl_nearest || rl_nearest ) {
		  propagate( pos, nearest );
		  pos++;
	       } 
	       m_column = std::min( m_indent * m_indent_step,
				    m_max_width - m_min_width );
	       m_line++;
	    } else {
	       if( nl_nearest || rl_nearest ) {
		  m_column = std::min( m_indent * m_indent_step,
				       m_max_width - m_min_width );
		  m_line++;
	       } else {
		  m_column += nearest-pos+1 + (tab_nearest?7:0);
	       }
	       pos = nearest + 1;
	    }
	 } else { // m_column + (nearest-pos+1) > m_max_width
	    if( m_wrap ) {
	       if( space_nearest || tab_nearest ) {
		  if( m_word_wrap ) {
		     propagate( pos, m_word_pos );
		     pos = m_word_pos+1;
		     m_word_pos = nearest;
		  } else {
		     propagate( pos, pos + (m_max_width-m_column) + 1 );
		     pos += (m_max_width-m_column) + 1;
		  }
		  m_column = std::min( m_indent * m_indent_step,
				       m_max_width - m_min_width );
		  m_line++;
	       } else if( nl_nearest || rl_nearest ) {
		  propagate( pos, pos + (m_max_width-m_column) + 1 );
		  pos += (m_max_width-m_column) + 1;
	       }
	    } else {
	       if( nl_nearest || rl_nearest ) {
		  m_column = std::min( m_indent * m_indent_step,
				       m_max_width - m_min_width );
		  m_line++;
	       } else {
		  m_column += nearest-pos+1 + (tab_nearest?7:0);
	       }
	       pos = nearest + 1;
	    }
	 }
      } else {
	 while( (unsigned)(epptr()-pos) >= m_max_width-m_column ) {
	    propagate( pos, pos + (m_max_width-m_column) + 1 );
	    pos += (m_max_width-m_column) + 1;
	    m_column = std::min( m_indent * m_indent_step,
				 m_max_width - m_min_width );
	    m_line++;  
	 }
	 
	 unsigned int remaining_len = epptr()-pos;
	 if( remaining_len > 0 ) {
	    traits_type::move( pbase(), pos, remaining_len );
	    setp( pbase(), epptr() );
	    pbump( remaining_len );
	    pos = pbase();
	 } else {
	    setp( pbase(), epptr() );
	    pos = pbase();
	 }
	 if( ch != traits_type::eof() )
	    _M_xsputnc( ch, 1 );
	 break; // break the main loop
      }
   } while( 1 );

   return traits_type::eof();
}
   
void channelbuf::propagate( const char_type* begin, const char_type* end ) {
   for( Assoc_list::iterator i = m_assoc.begin(); i != m_assoc.end(); ++i ) {
      (*i)->write( begin, end-begin );
      (*i)->write( "\n", 1 );
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
   m_indent_step = step;
}

unsigned int channelbuf::indent_step() const {
   return m_indent_step;
}

void channelbuf::incr_indent() {
   m_indent += m_indent_step;
}

void channelbuf::decr_indent() {
   m_indent -= m_indent_step;
}

void channelbuf::indent( const unsigned int val ) {
   m_indent = val;
}

unsigned int channelbuf::indent() const {
   return m_indent;
}

void channelbuf::min_width( unsigned int width ) {
   m_min_width = width;
}

unsigned int channelbuf::min_width() const {
   return m_min_width;
}

void channelbuf::max_width( unsigned int width ) {
   m_max_width = width;
}

unsigned int channelbuf::max_width() const {
   return m_max_width;
}

void channelbuf::wrap( bool allow ) {
   m_wrap = allow;
}

bool channelbuf::wrap() const {
   return m_wrap;
}

void channelbuf::word_wrap( bool allow ) {
   m_word_wrap = allow;
}

bool channelbuf::word_wrap() const {
   return m_word_wrap;
}

void channelbuf::space_chars(const char* spc ) {
   m_spaces = spc;
}

std::string channelbuf::space_chars() const {
   return m_spaces;
}

}; // end of namespace DGD

// 
// Local Variables:
// compile-command: "make dgChannelBuf.obj"
// End:
//


