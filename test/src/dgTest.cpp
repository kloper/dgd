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
// dgTest.cpp -- test for DGD
//

#include <dgDebug.h>

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

int main( int argc, char** argv ) {
   Debug dout( argc, argv );

   single_channel_test( dout["main"] );
   manip_test( dout["main"] );

   return 0;
}


// 
// Local Variables:
// compile-command: "make dgTest.obj"
// End:
//


