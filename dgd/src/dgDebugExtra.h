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
// dgDebugExtra.h -- couple of useful macro definitions
//

#ifndef _dgDebugExtra_h_
#define _dgDebugExtra_h_

/**
 * @def dgd_file
 * 
 * Expands to constant string in format "filename(line)"
 */
#define dgd_file (char*)__FILE__ << "(" << __LINE__ <<") "


/** 
 * @def dgd_expand(s)
 *
 * Expands into expression in form "s = " << s. For example dgd_expand(a+b)
 * will expand into "a+b = " << (a+b)
 *
 * @param s any expression
 */
#define dgd_expand(s) #s " = " << (s)

#define dgd_channel(name) ((DGD::Debug::debug_factory->operator[](#name)))

#if defined(_TRACE)
#define dgd_trace(name, arg) \
if(DGD::Debug::debug_factory) { \
  DGD::channel& __cnl = dgd_channel(name); \
  __cnl.is_open() && (__cnl << arg ); \
}
#else 
#define dgd_trace(name, arg)
#endif 

#if defined(_TRACE)
#define dgd_start_scope(c, text) \
DGD::channel* __cnl = NULL; \
if(DGD::Debug::debug_factory) __cnl = &dgd_channel(c); \
__cnl && __cnl->is_open() && ((*__cnl) << text " {" << std::endl << DGD::incr);
#else
#define dgd_start_scope(c, text) 
#endif

#if defined(_TRACE)
#define dgd_end_scope(c) \
__cnl && __cnl->is_open() && ((*__cnl) << DGD::decr<< "}" << std::endl);
#else 
#define dgd_end_scope(c)
#endif

#if defined(_TRACE)
#define dgd_end_scope_text(c, text) \
__cnl && __cnl->is_open() && ((*__cnl) << DGD::decr<< text "}" << std::endl);
#else 
#define dgd_end_scope_text(c, text)
#endif

#endif /* _dgDebugExtra_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgDebugExtra.obj"
 * End:
 */


