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

/**
 * @file dgChannel.cpp
 *
 * implementation of DGD::channel
 */

#include <ctime>

#include "dgDebug.h"
#include "dgChannel.h"

namespace DGD {

/**
 * channel constructor. This one solves the chicken and egg problem of
 * stream and streambuf construction. It calls std::ostream::init()
 * with NULL pointer, effectively disabling the channel. Use open()
 * method to enable it.
 */
channel::channel( const std::string& name ) :
   Parent( NULL ),
   m_name( name ),
   m_is_open(false) {
   init( NULL );
   m_buffer.pubsetbuf( NULL, 100 );
}

/**
 * Open and effectively initialize the channel. This method calls
 * std::ostream::init() with pointer to m_buffer. Note that m_buffer
 * is not affected by close(), this enables multiple close() and
 * open() operations on the channel.
 */
void channel::open() {
   m_is_open = true;
   init( &m_buffer );   
}

/**
 * Close and effectively disable the channel.
 */
void channel::close() {
   m_is_open = false;
   init( NULL );
}

/**
 * Return channel open/close status.
 */
bool channel::is_open() const {
   return m_is_open;
}

/**
 * Return channel open/close status. This operator is used for 
 * @code
 * if( channel ) { .... }
 * @endcode
 * expressions.
 */
channel::operator bool () const {
   return m_is_open;
}

/**
 * Return channel name. 
 */
const std::string& channel::name() const {
   return m_name;
}

/**
 * Return pointer to channel buffer. See DGD::channelbuf for more
 * info. This method is const so preventing non-const methods on the
 * channel buffer. Use channel::rdbuf() for changing channel buffer
 * directly.
 */
channelbuf* channel::rdbuf() const {
   return (m_is_open? (channelbuf*)&m_buffer : NULL );
}

/**
 * Return the channel buffer. This method allows direct access to the
 * channel buffer. See DGD::channelbuf for more info. @note This
 * method must be used carefully since direct access to the channel
 * buffer can cause unexpected results.
 */
channelbuf& channel::rdbuf() {
   return m_buffer;
}

/**
 * Change indentation step. @note This method does not affect the
 * indentation level itself.
 * @see channelbuf::indent_step(unsigned int)
 */
void channel::indent_step( unsigned int step ) {
   m_buffer.indent_step( step );
}

/**
 * Return indentation step.
 * @see channelbuf::indent_step() const
 */
unsigned int channel::indent_step() const {
   return m_buffer.indent_step();
}

/**
 * Increment indentation level by indentation step.
 * @see channelbuf::incr_indent()
 */
void channel::incr_indent() {
   m_buffer.incr_indent();
}

/**
 * Decrement indentation level by indentation step.
 * @see channelbuf::decr_indent()
 */
void channel::decr_indent() {
   m_buffer.decr_indent();
}

/**
 * Change indentation level. @note The parameter need not to be
 * multiple of the indentation level.
 * @see channelbuf::indent(unsigned int)
 */
void channel::indent( unsigned int val ) {
   m_buffer.indent( val );
}

/**
 * Return indentation level.
 * @see channelbuf::indent() const
 */
unsigned int channel::indent() const {
   return m_buffer.indent();
}

/**
 * Change minimum line width. The effective indentation level is
 * determined as 
 * @verbatim
 * elevel = min( indentation level, max_width()-min_width() );
 * @endverbatim
 * @see channelbuf::min_width(unsigned int)
 */
void channel::min_width( unsigned int width ) {
   m_buffer.min_width( width );
}

/**
 * Return minimum line width.
 * @see channelbuf::min_width() const
 */
unsigned int channel::min_width() const {
   return m_buffer.min_width();
}

/**
 * Change maximum line width. 
 * @see channelbuf::max_width(unsigned int)
 */
void channel::max_width( unsigned int width ) {
   m_buffer.max_width( width );
}

/**
 * Return maximum line width.
 * @see channelbuf::max_width() const
 */
unsigned int channel::max_width() const {
   return m_buffer.max_width();
}

/**
 * Disable or enable character wrapping.
 * @see channelbuf::wrap(bool)
 */
void channel::wrap( bool allow ) {
   m_buffer.wrap( allow );
}

/**
 * Return character wrapping flag.
 * @see channelbuf::wrap() const
 */
bool channel::wrap() const {
   return m_buffer.wrap();
}

/**
 * Disable or enable word wrapping.
 * @see channelbuf::word_wrap(bool)
 */
void channel::word_wrap( bool allow ) {
   m_buffer.word_wrap( allow );
}

/**
 * Return character wrapping flag.
 * @see channelbuf::word_wrap() const
 */
bool channel::word_wrap() const {
   return m_buffer.word_wrap();

}

/**
 * Change space characters. This method accepts a null-terminated
 * character string. Each character in the string will be considering
 * like a space between words during word wrapping.
 * @see channelbuf::space_chars(const char*)
 */
void channel::space_chars(const char* spc ) {
   m_buffer.space_chars( spc );
}

/**
 * Return space characters.
 * @see channelbuf::space_chars() const
 */
std::string channel::space_chars() const {
   return m_buffer.space_chars();
}

/**
 * Dump header information into the channel.
 * Normally this method is invoked by assoc(), but it can be called in
 * any other places.
 */
void channel::header() {
   channel& self = *this;
   time_t local_time;

   time( &local_time );

   self << "Content-type: text/plain" << std::endl << std::endl
	<< "Channel: " << m_name << std::endl
	<< "Time: " << ctime( &local_time ) << std::endl 
	<< std::endl;
   
}

/**
 * Associate physical stream with a channel. See DGD::channel
 * documentation for more information.
 * @see DGD::channelbuf
 * @see channelbuf::assoc(const stream&)
 */
void assoc( stream s, channel& channel ) {
   if( s.get() != NULL ) {
      channelbuf& buf = channel.rdbuf();
      buf.assoc( s );
      channel.header();
   }
}

/**
 * Associate physical stream with a channel. This function acts as
 * void assoc(stream,channel&), but takes name of the channel
 * instead. The DGD::Debug factory is searched for the channel with
 * the given name. See DGD::channel documentation for more information.
 * @see DGD::channelbuf
 * @see channelbuf::assoc(const stream&)
 */
void assoc( stream s, const std::string& name ) {
   Debug* factory = Debug::factory();
   if( factory != NULL ) {
      Debug::channel_ptr c = factory->operator[]( name );
      if( c.get() != NULL && s.get() != NULL ) {
	 channelbuf& buf = c->rdbuf();
	 buf.assoc( s );	 
	 c->header();
      }
   }
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgChannel.obj"
// End:
//


