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
// dgOptionFilter.cpp -- implementation for dgOptionFilter.h
//

#include <list>
#include <vector>

#include <boost/regex.hpp>

#include "dgOptionFilter.h"

namespace DGD {

option_filter::option_set_container* 
option_filter::operator () ( int argc, char** argv, int filtc, char** filtv ) {
   unsigned int i, j, k;
   typedef std::list<char*> opt_list;
   typedef std::vector<bool> matched_bitset;

   matched_bitset matched_flags( argc-1 );
   option_set_container* result = new option_set_container[ filtc + 1 ];
   
   matched_flags.assign( argc-1, false );
   result->assign( filtc + 1, option_set_type() );

   for( j = 0; j < (unsigned)filtc; ++j ) {
      opt_list matched_options;
      for( i = 1; i < (unsigned)argc; ++i ) {
	 boost::regex filter( filtv[j] );
	 if( boost::regex_match( argv[i], filter ) ) {
	    matched_options.push_back( argv[i] );
	    matched_flags[i-1] = true;
	 }
      }

      result->at(j).allocate( matched_options.size() + 1 );
      result->at(j).argv[0] = argv[0];

      opt_list::iterator opti = matched_options.begin();
      for( k = 1; k < matched_options.size() + 1; ++k )
	 result->at(j).argv[k] = *opti++;
   }	 

   int unmatched_size = 
      std::count( matched_flags.begin(), matched_flags.end(), false );

   result->at(filtc).allocate( unmatched_size + 1 );
   result->at(filtc).argv[0] = argv[0];
   

   k = 1;
   for( i = 1; i < (unsigned)argc; ++i )
      if( !matched_flags[i-1] )
	 result->at(filtc).argv[k++] = argv[i];

   return result;
}


}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgOptionFilter.obj"
// End:
//


