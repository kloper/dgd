//
// 
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
// Copyright (c) 2002. Dimitry Kloper <kloper@users.sf.net> . 
//
// dgTest.cpp -- test for DGD
//

#include <dgConfig.h>

#include <boost/smart_ptr.hpp>

#include <dgDebug.h>
#include <dgDebugStd.h>
#include <dgOptionFilter.h>
#include <dgMultifileLog.h>

using namespace std;
using namespace DGD;

void single_channel_test( channel& debug ) {
   int i;

   for(i = 0; i < 100; i++) {
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }

   debug.wrap(false);
   debug << endl;

   for(i = 0; i < 120; i++) {
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }

   debug.wrap(true);
   debug.word_wrap(false);
   debug << endl;

   for(i = 0; i < 120; i++) {
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }
   
   debug.word_wrap(true);
   debug << endl;

   for(i = 0; i < 120; i++) {
      if( i % 10 == 0) {
	 debug << endl;
	 debug.incr_indent();
      }
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }

   debug << endl;
   debug.indent( 0 );
   debug.word_wrap(false);

   for(i = 0; i < 120; i++) {
      if( i % 10 == 0) {
	 debug << endl;
	 if( i < 64 ) 
	    debug.incr_indent();
	 else
	    debug.decr_indent();
      }
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }

   debug << endl;

   debug.indent(0);
   debug.word_wrap(true);

   for(i = 0; i < 120; i++) {
      if( i % 10 == 0) {
	 if( i < 60 ) 
	    debug.incr_indent();
	 else
	    debug.decr_indent();
      }
      debug << "qwertyui opasdfghjkl ;'zxcvbnm,./ dgdgdh d dgdg dgd dgdgdgd dfdfdfdg dgdgdgd d dhdhdhdie9deiosdfj23491111111087r4hnwef dfhefjsdf sdfjsdfhjdgfdhfg fvgdfhsgfshdfgsdh sdhfgsdfhd s\n";
   }

   debug << "qwertyui opas";
   debug << std::endl;

   debug << "11111111111111111111112222222222222222222222233333333333333333333333444444444444444444444455555555555555566666666666666666666666688888888888888" << endl;
//     debug << "Hello World!!!" << ' ' 
//  	 << incr << 1 << "\n" 
//  	 << 2 << "\n" 
//  	 << incr << 3 << "\n" 
//  	 << 3 << "\n" << decr  
//  	 << 5 << "\n" << std::endl 
//  	 << indent(5) << "kuku" << std::endl;
}

void manip_test( channel& debug ) {
   int i;

   debug << "hello world" << incr;

   debug << wrap(false) << endl;

   for(i = 0; i < 120; i++) {
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }

   debug << wrap(true) << word_wrap(false) << endl;

   for(i = 0; i < 120; i++) {
      debug << "Dimka" << ' ' << i << ' ' << "Pimka";
   }
   
   debug << word_wrap(true) << endl;   
}

void bad_stream_test( ostream& ostr ) {
   try {
      ostr << incr << "Hello world!" << endl << decr;
   } catch ( std::exception& ex ) {
      ostr << "bad channel test: " << ex.what() << endl;
   }
}

void two_channels_test() {
   dgd_start_scope( dimka, "void two_channels_test()" );
   dgd_echo( "Item 1" << endl );
   std::pair<int, int> p( 10,20 );
   dgd_scope( dimka, "p = " );
   dgd_echo( p << endl );
   dgd_end_scope( dimka );
   dgd_trace( pimka, "Hey ! I am Pimka !" << endl );
   dgd_trace( pimka, "Hit me !" << endl );
   dgd_end_scope( dimka );
}

struct A {
      char str[30];
      A() {
	 std::fill(str, str+30, '-' );
      }
      virtual const char* get() const {
	 return str;
      }
};

void pointers_test( channel& debug ) {
   char* a = "Hello World";
   A b;

   debug << dgd_expand(a) << endl
	 << dgd_expand(mem_ref(a)) << endl
	 << dgd_expand(mem_ref(a, true)) << endl
	 << dgd_expand(&b) << endl
	 << dgd_expand(mem_ref(&b)) << endl
	 << incr << dgd_expand(mem(&b)) << decr << endl
	 << incr << incr << dgd_expand(mem(&b, 100)) << decr << decr << endl;
}

void empty_test( channel& debug ) {
}

void tie_test( Debug::debug_factory_ref& dout) {
   stream s = dout->create_file( "tie_test.log" );

   stream funnel( new DGD::funnel( *s ) );
   dout->append_file( funnel );

   DGD::channel& a_channel = dout->create_channel( "a_channel" );
   DGD::channel& b_channel = dout->create_channel( "b_channel" ); 

   assoc( funnel.get(), a_channel );
   assoc( funnel.get(), b_channel );

   manip_test( a_channel );
   a_channel << incr << incr << "hello hello world " ;
   a_channel.flush();
   manip_test( b_channel );
}

DGD::multifile_log::Split_by_size skip_by_size( 3000 );
DGD::multifile_log::Split_by_lines skip_by_lines( 10 );

void multifile_log_test( Debug::debug_factory_ref& dout) {   
   stream file = 
      dout->append_file( stream( new DGD::multifile_log( "multifile.log", 
							 &skip_by_lines ) ) );

   DGD::channel& m_channel = dout->create_channel( "m_channel" );

   assoc( file.get(), m_channel );

   manip_test( m_channel );
}

class My_class {
   public:
      int x;
      int y;
};

std::ostream& operator << ( std::ostream& dbg, const My_class& c ) {
   dbg << c.x << " " << c.y;
   return dbg;
}

channel& operator << ( channel& dbg,  const My_class& c ) {
   dbg << "[" << c.x << " " << c.y << "]";
   return dbg;
}

void overload_test( channel& debug ) {
   My_class c;
   c.x = 10; c.y = 15;
   cout << "c = " << c << endl;
   debug << "c = " << dgd << c << endl;
}

int main( int argc, char** argv ) {
   option_filter of;
   char* filter[] = { "--debug.*" };
   
   option_filter::option_set_container* option_sets = 
      of( argc, argv, 1, filter );

   Debug::debug_factory_ref dout = 
      Debug::create_factory( (*option_sets)[0].argc, 
			     (*option_sets)[0].argv );   
      
   if( dout.get() == NULL )
      return 1;
   
   dgd_trace( main, "Created by: " << dgd << make_pair( argc, argv ) << endl );

   stream s = dout->create_file( "dimka.log" );

   assoc( s.get(), dout->create_channel( "dimka" ) );
   assoc( s.get(), dout->create_channel( "pimka" ) );
   assoc( dout->main_file().get(), "dimka" );

   single_channel_test( *dgd_channel(main) );
   manip_test( *dgd_channel(main) );
   empty_test( *dgd_channel(main) );
   two_channels_test();
   overload_test( *dgd_channel(main) );
   pointers_test( *dgd_channel(main) );
   tie_test( dout );
   multifile_log_test( dout );
   bad_stream_test( cout );
   bad_stream_test( *dgd_channel(main) );

   return 0;
}


// 
// Local Variables:
// compile-command: "make dgTest.obj"
// End:
//


