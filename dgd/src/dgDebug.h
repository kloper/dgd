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
// dgDebug.h -- channel factory
//

#ifndef _dgDebug_h_
#define _dgDebug_h_

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "dgChannel.h"
#include "dgChannelManip.h"
#include "dgDebugOpt.h"

namespace DGD {

class Debug {
   public:
      typedef channel& channel_ref;

   public:
      Debug( int argc, char** argv );
      ~Debug();

      stream create_file( const std::string& name );
      channel& create_channel( const std::string& name );
      operator channel_ref () const;
      channel& operator[] ( const std::string& name );
      void current( const std::string& name );
      channel& current() const;
      
   protected:
      typedef boost::shared_ptr<channel> Channel_ptr;
      typedef std::list< Channel_ptr > Channel_list;
      typedef Channel_list::iterator Channel_iterator;
      typedef std::list< stream > File_list;

   private:
      Channel_list     m_channels;
      Channel_iterator m_current_channel;
      File_list        m_files;

      gengetopt_args_info m_args_info;    
};

}; // end of namespace DGD

#endif /* _dgDebug_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgDebug.obj"
 * End:
 */


