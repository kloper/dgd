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
// dgStream.h -- definition of dgstream
//

#ifndef _dgStream_h_
#define _dgStream_h_

#include <iostream>
#include <boost/smart_ptr.hpp>

namespace DGD {

typedef boost::shared_ptr< std::ostream > stream;

}; // end of namespace DGD

#endif /* _dgStream_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgStream.obj"
 * End:
 */


