// This file is part of Depression Glass library.
//
// Copyright (c) 2003. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// list_of_lists.cpp -- simple example of DGD usage
//

#include <list>
#include <map>
#include <string>

#include <dgDebug.h>

using namespace std;
using namespace DGD;

typedef map<string, string> Dictionary;
typedef list< pair< string, Dictionary > > List_of_dictionaries;

void init_lod( List_of_dictionaries& lod ) {
   dgd_start_scope( main, "init_lod()" );

   lod.push_back ( List_of_dictionaries::value_type( "Spanish", 
						     Dictionary() ) );
   lod.back().second.insert( Dictionary::value_type( "software", 
						     "software" ) );
   lod.back().second.insert( Dictionary::value_type( "development", 
						     "desarrollo" ) );
   lod.back().second.insert( Dictionary::value_type( "hard", 
						     "duro" ) );
   lod.back().second.insert( Dictionary::value_type( "process", 
						     "proceso" ) );

   lod.push_back ( List_of_dictionaries::value_type( "German", 
						     Dictionary() ) );
   lod.back().second.insert( Dictionary::value_type( "software", 
						     "software" ) );
   lod.back().second.insert( Dictionary::value_type( "development", 
						     "entwicklung" ) );
   lod.back().second.insert( Dictionary::value_type( "hard", 
						     "harter" ) );
   lod.back().second.insert( Dictionary::value_type( "process", 
						     "Prozes" ) );

   lod.push_back ( List_of_dictionaries::value_type( "French", 
						     Dictionary() ) );
   lod.back().second.insert( Dictionary::value_type( "software", 
						     "logiciel" ) );
   lod.back().second.insert( Dictionary::value_type( "development", 
						     "developpement" ) );
   lod.back().second.insert( Dictionary::value_type( "hard", "dur" ) );
   lod.back().second.insert( Dictionary::value_type( "process", 
						     "processus" ) );
   dgd_echo( dgd_expand(lod) << endl);

   dgd_end_scope( main);
}

int main( int argc, char** argv ) 
{  
   Debug::debug_factory_ref dout = Debug::create_factory( argc, argv );

   List_of_dictionaries lod;

   init_lod( lod );

   return 0;
}


