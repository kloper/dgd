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
// dgDebugStd.h -- output operators for some std types
//

#ifndef _dgDebugStd_h_
#define _dgDebugStd_h_

#include <list>
#include <vector>
#include <map>
#include <typeinfo>

#include "dgDebug.h"

namespace DGD {

template <class First_type, class Second_type>
inline channel& operator << ( channel& cnl,
			      const std::pair<First_type, Second_type>& p ) {
   cnl << "[" << p.first << " " << p.second << "]";
   return cnl;
}

template <>
inline channel& operator << ( channel& cnl,
			      const std::pair<int, char**>& params ) {
   cnl.wrap(false);
   for( int i = 0; i < params.first; ++i ) {
      cnl << params.second[i] << " ";
   }
   cnl.wrap(true);
   return cnl;
}

template <class Item_type>
inline channel& operator << ( channel& cnl, 
			      const std::list<Item_type>& l ) {
   cnl << "{" << incr << std::endl;
   for( std::list<Item_type>::const_iterator i = l.begin(); 
	i != l.end(); 
	++i ) {
      cnl << (*i) << std::endl;
   }
   cnl << decr << "}";
   return cnl;
}

template <class Item_type>
inline channel& operator << ( channel& cnl, 
			      const std::vector<Item_type>& v ) {
   cnl << "{" << incr << std::endl;
   for( std::vector<Item_type>::const_iterator i = v.begin(); 
	i != v.end(); 
	++i ) {
      cnl << (*i) << std::endl;
   }
   cnl << decr << "}";
   return cnl;
}

template <class Key_type, class Value_type>
inline channel& operator << ( channel& cnl, 
			      const std::map<Key_type, Value_type>& m ) {
   cnl << "{" << incr << std::endl;
   for( std::map<Key_type, Value_type>::const_iterator i = m.begin(); 
	i != m.end(); 
	++i ) {
      cnl << (*i) << std::endl;
   }
   cnl << decr << "}";
   return cnl;
}

template <class T>
struct dgd_reference {
      const T* const m_pointer;
      const bool m_use_as_void;
      const unsigned int m_size;
      
      dgd_reference() : m_pointer(NULL), m_use_as_void(false), m_size(0) {}
      explicit dgd_reference( const T* const p,
			      const bool use_as_void = false,
			      const unsigned int size = 0 ) :
	 m_pointer( p ), m_use_as_void(use_as_void), m_size(size) {}

      dgd_reference( const dgd_reference& peer ) : 
	 m_pointer( peer.m_pointer ), 
	 m_use_as_void( peer.m_use_as_void ),
	 m_size( peer.m_size ) {}
};

template <class T> 
inline dgd_reference<T> mem_ref( const T* const p,
				 const bool use_as_void = false,
				 const unsigned int size = 0 ) {
   return dgd_reference<T>(p, use_as_void, size);
}

template <class T> 
inline dgd_reference<T> mem( const T* const p,
			     const unsigned int size = sizeof(T),
			     const bool use_as_void = false ) {
   return dgd_reference<T>(p, use_as_void, size);
}

template <class T> 
inline std::ostream& operator << ( std::ostream& cnl,
				   const dgd_reference<T>& ptr ) {
   if( !ptr.m_use_as_void ) {
      cnl << "(" << typeid(T).name() << "*)";
   }
   std::ostream::fmtflags flags = cnl.flags();
   cnl << hex << "0x" << (void*)ptr.m_pointer;

   if( ptr.m_size > 0 ) {
      channel* dgd_cnl = dynamic_cast< channel* > ( &cnl );
      unsigned int width = channelbuf::DefaultMaxWidth;
      
      if( dgd_cnl ) {
	 if( dgd_cnl->wrap() ) {
	    width = dgd_cnl->max_width() - dgd_cnl->indent();
	 }
      }

      unsigned int num_bytes_per_line = 
	 std::min( (unsigned int)16, (width - 10) / 4 );
      unsigned int bytes_written = 0, i;
      unsigned char* mem = (unsigned char*)ptr.m_pointer;
      
      cnl << dec << " " << ptr.m_size << hex << " bytes" << std::endl;
      while(bytes_written < ptr.m_size) {
	 cnl << (unsigned long)( mem + bytes_written ) << " ";
	 
	 for(i = 0; i < num_bytes_per_line; i++) {
	    if( bytes_written+i < ptr.m_size ) {
	       cnl.width(2);
	       cnl << (unsigned int)(0xff & mem[bytes_written+i]) << " ";
	    } else {
	       cnl << "   ";
	    }
	 }
	 cnl << " ";
	 for( i = 0; i < num_bytes_per_line && 
		 bytes_written+i < ptr.m_size; i++ ) {
	    char c = mem[bytes_written+i];
	    cnl << ((c >= ' ')?c:'.');
	 }
	 bytes_written += num_bytes_per_line;
	 cnl << std::endl;
      }
   }

   cnl.flags( flags );
   return cnl;
}
}; // end of namespace DGD

#endif /* _dgDebugStd_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgDebugStd.obj"
 * End:
 */


