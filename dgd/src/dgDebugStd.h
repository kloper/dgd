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

}; // end of namespace DGD

#endif /* _dgDebugStd_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgDebugStd.obj"
 * End:
 */


