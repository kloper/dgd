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
// Copyright (c) 2003. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// dgd_dgc_debug.h -- declaration for dgd_dgc_debug.c
//

#ifndef _dgd_dgc_debug_h_
#define _dgd_dgc_debug_h_

#include <stdio.h>
#include "dgd_compile_cache.h"

void dgd_dump_parser_bytecode( FILE *stream, 
			       cache_item_t *ring, 
			       unsigned int nesting_level );

#endif /* _dgd_dgc_debug_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_dgc_debug.obj"
 * End:
 */

