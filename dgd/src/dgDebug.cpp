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

#include <regular_expression.h>

#include "dgDebug.h"

namespace DGD {

Debug* Debug::debug_factory = NULL;

class bad_params: std::exception {
      const char *m_what;
   public:

      bad_params() : m_what( "bad command line argument" ) {}
      bad_params( const char* s ) : m_what( s ) {}

      const char* what() const {
	 return m_what;
      }
};

class debug_disabled: public std::exception {
   public:
      const char* what() const {
	 return "debug is being disabled";
      }
};

Debug::Debug() :
   m_main_file( NULL ) 
{
   std::fill( (char*)&m_args_info, 
	      (char*)&m_args_info + sizeof(m_args_info), 
	      0 );

   channel& main_channel = create_channel( "main" );
   main_channel.open();   
   m_current_channel = m_channels.begin();
}

void Debug::process_options( int argc, char** argv ) {
   if (dgd_cmdline_parser (argc, argv, &m_args_info) != 0) {
      throw bad_params();
   }

   if( m_args_info.debug_main_file_given ) {
      m_main_file = create_file( m_args_info.debug_main_file_arg );
      assoc( m_main_file, **m_current_channel );
   }

   if( !m_args_info.debug_enable_flag )
      throw debug_disabled();

   // apply options on existing channels
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      apply_options( *i );
}

void Debug::apply_options( channel_ptr& chnl ) {
   if( m_args_info.debug_min_width_given ) 
      chnl->min_width( m_args_info.debug_min_width_arg );
   
   if( m_args_info.debug_max_width_given ) 
      chnl->max_width( m_args_info.debug_max_width_arg );
   
   if( m_args_info.debug_indent_step_given ) 
      chnl->indent_step( m_args_info.debug_indent_step_arg );

   if( m_args_info.debug_allow_wrap_given )
      chnl->wrap( ((m_args_info.debug_allow_wrap_flag == 0)? false: true) );
   if( m_args_info.debug_allow_word_wrap_flag )
      chnl->word_wrap( ((m_args_info.debug_allow_word_wrap_flag == 0)? 
			false:true) );
   
   if( m_args_info.debug_space_characters_given ) 
      chnl->space_chars( m_args_info.debug_space_characters_arg );

   if( m_args_info.debug_turn_on_given ) {
      try {
	 regex e( m_args_info.debug_turn_on_arg );
	 if( regex_match( e, chnl->name().c_str() ) ) {
	    chnl->open();
	 }
      } catch( bad_regex&  ) {
	 throw bad_params( "bad regular expression" );
      }
   }

   if( m_args_info.debug_turn_off_given ) {
      try {
	 regex e( m_args_info.debug_turn_off_arg );
	 if( regex_match( e, chnl->name().c_str() ) ) {
	    chnl->close();
	 }
      } catch( bad_regex& ) {
	 throw bad_params( "bad regular expression" );
      }
   }

   if( m_args_info.debug_space_characters_given ) 
      chnl->space_chars( m_args_info.debug_space_characters_arg );
}

Debug::~Debug() {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i ) 
      (*i)->flush();
   for( File_list::iterator f = m_files.begin(); f != m_files.end(); ++f )
      (*f)->flush();
}

channel& Debug::create_channel( const std::string& name ) {
   channel_ptr chnl;

   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i ) 
      if( (*i)->name() == name )
	 return **i;

   chnl.reset( new channel( name ) );

   apply_options( chnl );

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

stream Debug::main_file() const {
   return m_main_file;
}

Debug::operator Debug::channel_ref () const {
   return **m_current_channel;
}

Debug::channel_ptr Debug::operator[] ( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 return *i;
   return channel_ptr();
}

void Debug::current( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 m_current_channel = i;
}

Debug::channel_ptr Debug::current() const {
   return *m_current_channel;
}

Debug::debug_factory_ref Debug::create_factory( int argc, char** argv ) {
   if( debug_factory != NULL )
      return debug_factory_ref( debug_factory );
   
   debug_factory_ref df;
   try {
      debug_factory = new Debug();
      df.reset( debug_factory );
      df->process_options( argc, argv );
   } catch( bad_params& ) {
      debug_factory = NULL;
      df.reset( NULL );
   } catch( debug_disabled& ) {
      debug_factory = NULL;
      df.reset( NULL );
   }

   return df;
}

Debug* Debug::factory() {
   return Debug::debug_factory;
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgDebug.obj"
// End:
//


