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
// dgChannel.cpp -- implementation for dgChannel.h
//

#include "dgChannel.h"

namespace DGD {

channel::channel( const std::string& name ) :
   Parent( NULL ),
   m_name( name ),
   m_is_open(false) {
   init( NULL );
   m_buffer.pubsetbuf( NULL, 100 );
}

void channel::open() {
   m_is_open = true;
   init( &m_buffer );
}

void channel::close() {
   m_is_open = false;
   init( NULL );
}

bool channel::is_open() const {
   return m_is_open;
}

channel::operator bool () const {
   return m_is_open;
}

const std::string& channel::name() const {
   return m_name;
}

channelbuf* channel::rdbuf() const {
   return (m_is_open? (channelbuf*)&m_buffer : NULL );
}

void channel::indent_step( unsigned int step ) {
   m_buffer.indent_step( step );
}

unsigned int channel::indent_step() const {
   return m_buffer.indent_step();
}

void channel::incr_indent() {
   m_buffer.incr_indent();
}

void channel::decr_indent() {
   m_buffer.decr_indent();
}

void channel::indent( const unsigned int val ) {
   m_buffer.indent( val );
}

unsigned int channel::indent() const {
   return m_buffer.indent();
}

void channel::min_width( unsigned int width ) {
   m_buffer.min_width( width );
}

unsigned int channel::min_width() const {
   return m_buffer.min_width();
}

void channel::max_width( unsigned int width ) {
   m_buffer.max_width( width );
}

unsigned int channel::max_width() const {
   return m_buffer.max_width();
}

void channel::wrap( bool allow ) {
   m_buffer.wrap( allow );
}

bool channel::wrap() const {
   return m_buffer.wrap();
}

void channel::word_wrap( bool allow ) {
   m_buffer.word_wrap( allow );
}

bool channel::word_wrap() const {
   return m_buffer.word_wrap();

}

void channel::space_chars(const char* spc ) {
   m_buffer.space_chars( spc );
}

std::string channel::space_chars() const {
   return m_buffer.space_chars();
}

void assoc( stream s, channel& channel ) {
   channel.open();

   channelbuf *buf = channel.rdbuf();
   buf->assoc( s );
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgChannel.obj"
// End:
//


