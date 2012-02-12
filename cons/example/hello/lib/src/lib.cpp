// Copyright (c) 2002-2012. Dimitry Kloper <kloper at users dot sf dot net>
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
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// lib.cpp -- this file contains some lib functions
//

#include "lib.h"

namespace lib  {

std::ostream& operator << ( std::ostream& ostr, const my_type& s ) {
   ostr << "[" << s.name << "]";
   return ostr;
}

std::ostream& operator << ( std::ostream& ostr, const my_type::value_type& s ) 
{
   ostr << "->" << s.c_str();
   return ostr;
}

}; // end of namespace lib
