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
// dgFunnel.cpp -- implementation for dgFunnel.cpp
//

#include <ctime>

#include "dgFunnel.h"

namespace DGD {

funnelbuf::funnelbuf() :
   m_callback(NULL)
{
}

funnelbuf::~funnelbuf() {
}

void funnelbuf::post_process() 
{
   if( m_callback != NULL ) {
      position_type pos = position();
      unsigned long bytes = bytes_written();
      m_callback->operator()( pos.first, pos.second, bytes );
   }
}

void funnelbuf::assoc( std::ostream* str )
{
   channelbuf::assoc( str );
}

void funnelbuf::callback( Callback* cb ) 
{
   m_callback = cb;
}

const funnelbuf::Callback* funnelbuf::callback() const 
{
   return m_callback;
}


funnel::funnel( std::ostream& physical_stream ) :
   Parent( NULL )
{
   init( &m_buffer );
   m_buffer.assoc( &physical_stream );
   m_buffer.callback( this );
   m_buffer.pubsetbuf( NULL, 100 );
   m_buffer.wrap(false);
}

funnelbuf& funnel::rdbuf() {
   return m_buffer;
}

void funnel::header() {
   funnel& self = *this;
   time_t local_time;

   time( &local_time );

   self << "Content-type: text/plain" << std::endl << std::endl
	<< "Time: " << ctime( &local_time ) << std::endl 
	<< std::endl;
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgFunnel.obj"
// End:
//


