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
   m_is_open(false),
   m_name( name ) {
   init( &m_tmp_buffer );
   m_buffer.pubsetbuf( NULL, 100 );
   m_tmp_buffer.pubsetbuf( NULL, 100 );
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
   init( &m_tmp_buffer );
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
 * @warning DGD provides no limit on number of channels associated with the
 * same physical stream (like std::cout). But it is not recommended to
 * associate more then one channel with the same physical stream. In
 * fact, unexpected formatting results can occur. Instead you can
 * associate multiple channels with a single DGD::channel.
 * @see DGD::channelbuf
 * @see channelbuf::assoc(std::ostream*)
 */
void assoc( std::ostream* s, channel& channel ) {
   if( s != NULL ) {
      channelbuf& buf = channel.rdbuf();
      buf.assoc( s );
      channel.header();
   }
}

/**
 * Associate physical stream with a channel. See DGD::channel
 * documentation for more information. This one is a simple alias to
 * assoc(std::ostream*,channel& channel).
 * @warning DGD provides no limit on number of channels associated with the
 * same physical stream (like std::cout). But it is not recommended to
 * associate more then one channel with the same physical stream. In
 * fact, unexpected formatting results can occur. Instead you can
 * associate multiple channels with a single DGD::channel.
 * @see DGD::channelbuf
 * @see channelbuf::assoc(std::ostream*)
 * @see assoc(std::ostream*,channel& channel)
 */
void assoc( std::ostream& s, channel& cnl ) {
   assoc( &s, cnl );
}

/**
 * Associate physical stream with a channel. This function acts as
 * void assoc(std::ostream*,channel&), but takes name of the channel
 * instead. The DGD::Debug factory is searched for the channel with
 * the given name. See DGD::channel documentation for more
 * information.
 * @warning DGD provides no limit on number of channels associated with the
 * same physical stream (like std::cout). But it is not recommended to
 * associate more then one channel with the same physical stream. In
 * fact, unexpected formatting results can occur. Instead you can
 * associate multiple channels with a single DGD::channel.
 * @see DGD::channelbuf
 * @see channelbuf::assoc(std::ostream*)
 */
void assoc( std::ostream* s, const std::string& name ) {
   Debug* factory = Debug::factory();
   if( factory != NULL ) {
      Debug::channel_ptr c = factory->operator[]( name );
      if( c.get() != NULL && s != NULL ) {
	 channelbuf& buf = c->rdbuf();
	 buf.assoc( s );	 
	 c->header();
      }
   }
}

/**
 * @page tutor_channel DGD Channels - Things you need to know
 *
 * DGD::channelbuf can't be used to make output. After all, it is
 * derived from std::streambuf which is a kind of memory buffer. There
 * must be a stream. The DGD::channel is such a stream.
 *
 * Everything which @ref tutor_formatting "was told about formatting" is
 * also true for DGD::channel. If fact, it has all formatting-related
 * methods as DGD::channelbuf has.
 *
 * There is a number of things which differ DGD::channel from
 * std::ostream:
 * <ul>
 * <li> Channels are named. There is no default constructor. You must
 * pass a name of the channel when creating it. 
 * <li> Channels can be opened and closed. Closed channel makes no
 * output. By default it is closed. Use DGD::channel::open() method to
 * open the channel. There is also DGD::channel::is_open() and
 * DGD::channel::operator bool() which return the status of the
 * channel.
 * <li> DGD::channel makes no physical output, just formatting. To
 * achieve the output it must be associated with a physical stream
 * such as std::cout. Use DGD::assoc() functions to 
 * @ref tutor_assoc "make the association".
 * </ul>
 */

/**
 * @page tutor_assoc DGD Channels - Ensuring real output
 *
 * Here we go. DGD::channel makes no physical output, but makes
 * formatting. To get the actual output we need to associate the
 * channel with a physical stream e.g std::cout, sort of std::ofstream
 * or any other object which is derived from std::ostream. 
 * There are three assoc() functions for making the association:
 * @code
 * void assoc( std::ostream* s, channel& channel );
 * void assoc( std::ostream& s, channel& channel );
 * void assoc( std::ostream* s, const std::string& name );
 * @endcode
 *
 * All functions make the same job, but the later one assumes
 * existence of the DGD::Debug factory and queries the channel object
 * from the factory by the given name.
 *
 * The only remaining tricky question about associations is: what if I
 * make more then one association of the same channel, or more then
 * one with the same physical stream?
 * In fact DGD makes no restrictions on what do you associate, how and
 * when. So, for example, it is legal to associate the same channel with
 * std::cout and std::cerr, the channel will replicate its output into
 * both streams. It is legal to register the same channel twice with
 * the same stream, its output will be duplicated in this stream. 
 * 
 * There are two additional association schemes worth to be mentioned
 * here. 
 *
 * It is possible to associate two different channels with the same
 * physical stream. But in this case DGD makes no garanties on
 * validity of the resulting stream formatting. Surely, there is other
 * simple way to avoid the problem and allow output from multiple
 * channels into single physical buffer.
 *
 * To avoid the problem mentioned above lets remind that assoc()
 * functions accept any std::ostream as a physical stream and
 * DGD::chnnel itself derives from std::ostream. So, it is possible to
 * associate DGD::channel with another DGD::channel. Te problem
 * could be solved by associating a single channel with a
 * physical stream and then associating multiple other channels with
 * that first channel. The example below demonstrates the soulution.
 * @include mult_assoc.cpp 
 */

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgChannel.obj"
// End:
//


