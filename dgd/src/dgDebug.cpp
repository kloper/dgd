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

/**
 * @file dgDebug.cpp
 * Implementation of DGD::Debug.
 */

#include "dgDebug.h"

namespace DGD {

/**
 * Global Debug factory pointer . Use this global static variable with
 * care. It is initialized by Debug::create_factory(int,char**). This
 * variable is not const, nor const pointer, but changing this
 * variable directly is not recommended.
 * @see Debug::create_factory(int,char**)
 * @see Debug::factory()
 */
Debug* Debug::debug_factory = NULL;

/**
 * Bad command line option exception. This exception is thrown when
 * Debug::process_options(int,char**) encounters invalid command line
 * option.
 * @see Debug::create_factory(int,char**)
 * @see Debug::process_options(int,char**)
 */
class bad_params: std::exception {
      const char *m_what;
   public:

      bad_params() : m_what( "bad command line argument" ) {}
      bad_params( const char* s ) : m_what( s ) {}

      const char* what() const {
	 return m_what;
      }
};

/**
 * Disabled Debug exception. This exception is thrown when
 * Debug::create_factory(int,char**) tries to create the debug
 * factory, but the debug output is diabled by command line options.
 * @see Debug::process_options(int,char**)
 */
class debug_disabled: public std::exception {
   public:
      const char* what() const {
	 return "debug is being disabled";
      }
};

/**
 * Exit application exception. This exception is thrown when
 * Debug::process_options(int,char**) encounters option which requires
 * the entire application termination.
 * @see Debug::create_factory(int,char**)
 * @see Debug::process_options(int,char
 */
class exit_required: public std::exception {
   public:
      const char* what() const {
	 return "debug required exit";
      }
};

/**
 * Default constructor. Initializes the factory and opens the main
 * channel.
 */
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

/**
 * Process command line options. This method applies the command line
 * options set on the DGD::Debug object and its channels. 
 * 
 * The following options are accepted:
 * <dl>
 * <dt>--debug-version
 * <dd>Print DGD version to std::cout and exit the
 * application (actually throw DGD::exit_required exception).
 * <dt>--debug-help
 * <dd>Print DGD help to std::cout and exit the
 * application (actually throw DGD::exit_required exception).
 * <dt>--debug-enable
 * <dd>Enable debug. By default the debug logging is disabled. This
 * command line option enables it. Debug::process_options(int,char**)
 * will throw debug_disabled exception if this option is not specified
 * on command line.
 * <dt>--debug-main-file
 * <dd>Create the main file. This option must be specified with a string
 * argument which defines the main file name. The main file is
 * created and its stream can be queried by Debug::main_file().
 * <dt>--debug-min-width
 * <dd>Set default minimum line width for all existing and future
 * channels. This option must be specified with an integer parameter
 * which defines the default minimum line width. 
 * <dt>--debug-max-width
 * <dd>Set default maximum line width for all existing and future
 * channels. This option must be specified with an integer parameter
 * which defines the default maximum line width. 
 * <dt>--debug-indent-step
 * <dd>Set default indentation step for all existing and future
 * channels. This option must be specified with an integer parameter
 * which defines the default indentation step. 
 * <dt>--debug-allow-wrap
 * <dd>Set default character wrapping policy for all existing and future
 * channels. 
 * <dt>--debug-allow-word-wrap
 * <dd>Set default word wrapping policy for all existing and future
 * channels. 
 * <dt>--debug-space-characters
 * <dd>Set default space character set for all existing and future
 * channels. This option must be specified with a string parameter
 * which defines the default space characters. 
 * <dt>--debug-turn-on
 * <dd>Turn on (open) channels. This option must be specified with a
 * string parameter which defines a GNU regular exception. This regexp
 * is applied on the current channel list. All channels with names
 * matching the regexp will be opened.
 * <dt>--debug-turn-off
 * <dd>Turn off (close) channels. This option must be specified with a
 * string parameter which defines a GNU regular exception. This regexp
 * is applied on the current channel list. All channels with names
 * matching the regexp will be closed.
 * </dl>
 */
void Debug::process_options( int argc, char** argv ) {
   if (dgd_cmdline_parser (argc, argv, &m_args_info) != 0) {
      throw bad_params();
   }

   if( m_args_info.debug_version_flag ) {
      dgd_cmdline_parser_print_version();
      throw exit_required();
   }
   
   if( m_args_info.debug_help_flag ) {
      dgd_cmdline_parser_print_help();
      throw exit_required();
   }      

   if( !m_args_info.debug_enable_flag )
      throw debug_disabled();

   if( m_args_info.debug_main_file_given ) {
      m_main_file = create_file( m_args_info.debug_main_file_arg );
      assoc( m_main_file.get(), **m_current_channel );
   }

   // apply options on existing channels
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      apply_options( *i );
}

/**
 * Apply default channel-specific options on given channel.
 * @see Debug::process_options(int,char**)
 */
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

/**
 * Destructor. All memory is deallocated automatically by smart
 * pointers. This destructor flushes all files and channels.
 * @see Debug::Debug()
 */
Debug::~Debug() {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i ) 
      (*i)->flush();
   for( File_list::iterator f = m_files.begin(); f != m_files.end(); ++f )
      (*f)->flush();
}

/**
 * Create channel. This method creates a new channel if there is no channel
 * with the given name in the channels list. If a channel with the
 * given name does already exist it is returned.
 * @see Debug::operator[]
 */
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

/**
 * Create a file stream. This method will return NULL stream if
 * the file can't be created.
 */
stream Debug::create_file( const std::string& name ) {
   stream new_file( new std::ofstream( name.c_str() ) );
   if( new_file->fail() || new_file->bad() ) {
      new_file.reset( NULL );
      return new_file;
   }

   m_files.push_back( new_file );
   return new_file;
}

/**
 * Return main file stream
 */
stream Debug::main_file() const {
   return m_main_file;
}

/**
 * @fun int Debug::operator channel_ref ( ) const 
 * DGD::Debug to DGD::channel& conversion. Returns current channel.
 */
Debug::operator Debug::channel_ref () const {
   return **m_current_channel;
}

/**
 * Get channel by name. 
 * @return Pointer to the channel with the given name or NULL.
 * @see Debug::create_channel(const std::string&)
 */
Debug::channel_ptr Debug::operator[] ( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 return *i;
   return channel_ptr();
}

/**
 * Set current channel. 
 * @see Debug::current()
 */
void Debug::current( const std::string& name ) {
   for( Channel_iterator i = m_channels.begin(); i != m_channels.end(); ++i )
      if( (*i)->name() == name )
	 m_current_channel = i;
}

/**
 * Return current channel.
 * @see Debug::current(const std::string&)
 */
Debug::channel_ptr Debug::current() const {
   return *m_current_channel;
}

/**
 * Create global Debug factory. This function must be called at most
 * once during the application lifetime. It creates the factory and
 * calls Debug::process_options(int,char**).
 * @return This method returns a smart pointer to the factory. This
 * smart pointer <b>must</b> be used to ensure proper factory
 * destruction, even when the application crashes. Usually you need
 * only a single instance of the pointer:
 * @code
 * int main( int argc, char** argv ) {
 *     using namespace DGD;
 *     debug_factory_ref factory = create_factory( argc, argv );
 *     ...
 * }
 * @endcode
 * Note that the 'factory' variable is destructed always when main()
 * does exit. It will be destructed even if application crashes on
 * unexpected exception. It is bad idea to make this variable global 
 * or static since not all compilers ensure that global variables are
 * destructed on the crash (at least msvc6.0sp5 does not).
 * @see Debug::debug_factory
 * @see Debug::factory()
 */
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
   } catch( exit_required& ) {
      exit(0);
   }

   return df;
}

/**
 * Return pointer to the global Debug factory.
 * @see Debug::create_factory(int,char**)
 */
Debug* Debug::factory() {
   return Debug::debug_factory;
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgDebug.obj"
// End:
//


