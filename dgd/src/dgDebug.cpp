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
// dgDebug.cpp -- implementation for dgDebug.h
//

#include <exception>

#include <boost/bind.hpp>

#include "dgDebug.h"

namespace DGD {

class bad_params: std::exception {
   public:
      const char* what() const {
	 return "bad command line argument";
      }
};

Debug::Debug( int argc, char** argv ) {
   if (cmdline_parser (argc, argv, &m_args_info) != 0) {
      throw bad_params();
   }

   channel& main_channel = create_channel( "main" );
   main_channel.open();

   if( m_args_info.debug_file_given ) {
      stream main_file = create_file( m_args_info.debug_file_arg );
      assoc( main_file, main_channel );
   }
   
   m_current_channel = m_channels.begin();
}

channel& Debug::create_channel( const std::string& name ) {
   Channel_ptr chnl;

   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i ) 
      if( (*i)->name() == name )
	 return **i;

   chnl.reset( new channel( name ) );

   if( m_args_info.debug_min_width_given ) 
      chnl->min_width( m_args_info.debug_min_width_arg );

   if( m_args_info.debug_max_width_given ) 
      chnl->max_width( m_args_info.debug_max_width_arg );

   if( m_args_info.debug_indent_step_given ) 
      chnl->indent_step( m_args_info.debug_indent_step_arg );

   chnl->wrap( ((m_args_info.debug_allow_wrap_flag == 0)? false: true) );
   chnl->word_wrap( ((m_args_info.debug_allow_word_wrap_flag == 0)? 
			false:true) );
   
   if( m_args_info.debug_space_characters_given ) 
      chnl->space_chars( m_args_info.debug_space_characters_arg );

   m_channels.push_back( chnl );

   return *chnl;
}

stream Debug::create_file( const std::string& name ) {
   stream new_file( new std::ofstream( name.c_str() ) );
   if( new_file->fail() || new_file->bad() ) {
      new_file.reset( NULL );
      return new_file;
   }

   m_files.push_back( new_file );
   return new_file;
}

Debug::operator Debug::channel_ref () const {
   return **m_current_channel;
}

channel& Debug::operator[] ( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 return **i;
   return **m_current_channel;
}

void Debug::current( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 m_current_channel = i;
}

channel& Debug::current() const {
   return **m_current_channel;
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgDebug.obj"
// End:
//


