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
// dgChannel.h -- DGD channel implementation
//

#ifndef _dgChannel_h_
#define _dgChannel_h_

#include <iostream>
#include <string>

#include "dgChannelBuf.h"

namespace DGD {

class channel: public std::ostream {
   public:
      typedef std::ostream Parent;
      
   public:
      channel( const std::string& name );
      void               open();
      void               close();
      bool               is_open() const;
      operator           bool () const;
      const std::string& name() const;
      channelbuf*      rdbuf() const;
      channelbuf&      rdbuf();
      
      void         indent_step( unsigned int step );
      unsigned int indent_step() const;
      void         incr_indent();
      void         decr_indent();
      void         indent( unsigned int val );
      unsigned int indent() const;
      
      void         min_width( unsigned int width );
      unsigned int min_width() const;
      void         max_width( unsigned int width );
      unsigned int max_width() const;
      
      void         wrap( bool allow );
      bool         wrap() const;
      void         word_wrap( bool allow );
      bool         word_wrap() const;
      
      void         space_chars(const char* spc = " \t");
      std::string  space_chars() const;

   private:
      bool m_is_open;
      std::string m_name;
      channelbuf m_buffer;
};

void assoc( stream s, channel& channel );
void assoc( stream s, const std::string& name );

}; // end of namespace DGD

#endif /* _dgChannel_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgChannel.obj"
 * End:
 */


