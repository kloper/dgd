//
// $Id: dgd\\examples\\Attic\\dgExample.cpp,v 1.1 2002/08/19 13:57:22 dimka Exp $
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
// dgExample.cpp -- example of using code which does not exist
//

Debug dout;

int main( int argc, char** argv ) {

   dout.init( argc, argv );

   ofstream main_channel( "debug.log" );
   ofstream secondary_channel( "alg.log" );

   dout.create_channel( "main", /* current channel = */ true );
   dout.create_channel( "alg" );

   assoc( dout[ "main" ], main_channel );
   assoc( dout[ "alg" ], secondary_channel );

   dout["main"] << "hello world" << endl;
   DGD( "main", "hello world" << endl );

   return 0;
}


// 
// Local Variables:
// compile-command: "make dgExample.obj"
// End:
//


