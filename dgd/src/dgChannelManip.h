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
// dgChannelManip.h -- manipulators for the channel
//

#ifndef _dgChannelManip_h_
#define _dgChannelManip_h_

#include "dgChannel.h"

namespace DGD {

template <class Arg_type>
class channel_manip {
   public:
      typedef void (channel::*method_type)( Arg_type );
   public:
      method_type m_method;
      Arg_type    m_argument;
      
      explicit channel_manip( method_type method, Arg_type arg ):
	 m_method(method), m_argument(arg) {}
};

class channel_manip_void {
   public:
      typedef void (channel::*method_type)();

   public:
      method_type m_method;

      explicit channel_manip_void( method_type m ):
	 m_method(m) {}
};

typedef channel_manip<bool> channel_manip_bool;
typedef channel_manip<unsigned int>  channel_manip_uint;

channel& operator <<( channel& cnl, const channel_manip_void& manip ) {
   (cnl.*(manip.m_method))();
   return cnl;
}

channel& operator <<( channel& cnl, const channel_manip_bool& manip ) {
   (cnl.*(manip.m_method))( manip.m_argument );
   return cnl;
}

channel& operator <<( channel& cnl, const channel_manip_uint& manip ) {
   (cnl.*(manip.m_method))( manip.m_argument );
   return cnl;
}

const channel_manip_void incr = channel_manip_void( &channel::incr_indent );
const channel_manip_void decr = channel_manip_void( &channel::decr_indent );

channel_manip_uint step( unsigned int s ) {
   return channel_manip_uint( &channel::indent_step, s );
}

channel_manip_uint indent( unsigned int s ) {
   return channel_manip_uint( &channel::indent, s );
}

channel_manip_uint minwidth( unsigned int s ) {
   return channel_manip_uint( &channel::min_width, s );
}

channel_manip_uint maxwidth( unsigned int s ) {
   return channel_manip_uint( &channel::max_width, s );
}

channel_manip_bool wrap( bool s ) {
   return channel_manip_bool( &channel::wrap, s );
}

channel_manip_bool word_wrap( bool s ) {
   return channel_manip_bool( &channel::word_wrap, s );
}

}; // end of namespace DGD

#endif /* _dgChannelManip_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgChannelManip.obj"
 * End:
 */


