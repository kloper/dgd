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

#include <boost/smart_ptr.hpp>

#include "dgConfig.h"
#include "dgChannel.h"
#include "dgChannelManip.h"
#include "dgDebugExtra.h"
#include "dgDebugStd.h"
#include "dgDebugOpt.h"

namespace DGD {

class Debug {
   public:
      typedef channel& channel_ref;
      typedef boost::shared_ptr<Debug> debug_factory_ref;
      typedef boost::shared_ptr<channel> channel_ptr;

   public:
      Debug();
      ~Debug();

      void process_options( int argc, char** argv );
      stream create_file( const std::string& name );
      stream main_file() const;
      channel& create_channel( const std::string& name );
      operator channel_ref () const;
      channel_ptr operator[] ( const std::string& name );
      void current( const std::string& name );
      channel_ptr current() const;

   public:
      static Debug* debug_factory;
      static Debug* factory();
      static debug_factory_ref create_factory( int argc, char** argv );

   protected:
      typedef std::list< channel_ptr > Channel_list;
      typedef Channel_list::iterator Channel_iterator;
      typedef std::list< stream > File_list;

   private:
      Channel_list     m_channels;
      Channel_iterator m_current_channel;
      File_list        m_files;
      stream           m_main_file;

      dgd_gengetopt_args_info m_args_info;    
};

}; // end of namespace DGD

#endif /* _dgDebug_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgDebug.obj"
 * End:
 */


