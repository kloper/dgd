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
// dgOptionFilter.h -- simple class which can separate command line 
//                     options into sets
//

#ifndef _dgOptionFilter_h_
#define _dgOptionFilter_h_

#include <vector>

namespace DGD {

class option_filter {
   public:
      class option_set_type {
	 public:

	    int argc;
	    char** argv;

	    option_set_type() : argc(0), argv(NULL) {}
	    
	    void allocate( int n ) {
	       argc = n;
	       argv = new char*[ argc ];
	       std::fill( argv, argv + argc, (char*)NULL );
	    }

	    ~option_set_type() {
	       if( argv ) delete [] argv;
	    }
      };
	    
      typedef std::vector<option_set_type> option_set_container;

      /**
       * This operator takes <argc,argv> as received by main function,
       * then it takes a set of regular expressions defined by <filtc, filtv>
       * and returns a set (vector) of option subsets. First set is generated
       * by the first filter, second set by the second filter and so on, 
       * the last set contains options which were not selected by any filter.
       * so the following is always true result->size() == filtc + 1
       */
      option_set_container* operator () ( int argc, char** argv,
					  int filtc, char** filtv );
};

}; // end of namespace DGD

#endif /* _dgOptionFilter_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgOptionFilter.obj"
 * End:
 */


