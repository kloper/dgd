/* 
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This file is part of Depression Glass library.
 *
 * Copyright (c) 2003. Dimitry Kloper <dimka@cs.technion.ac.il> . 
 *
 * parser.c -- test format parser
 *
 */

#include <dgd_format_parser.h>
#include <dgd_dgc_debug.h>

extern char* dgd_format_string;
extern void yyparse();

int main( int argc, char** argv ) {
   int i;
   cache_item_t *result;
   cache_t *cache;

   cache = dgd_format_parser_cache();

   for( i = 1; i < argc; i++ ) {
      printf( "free cache: %u\n", dgd_ring_size( cache->free_ring ) );
      printf( "> %s\n", argv[i] );
      result = dgd_format_parse( argv[i], PARS_FLAG_DEFAULT );
      
      if( result != NULL ) {
	 dgd_dump_parser_bytecode( stdout, result->value.chain.ring, 0 );
         printf( "free cache: %u\n", dgd_ring_size( cache->free_ring ) );
      }
   }

   for( i = 1; i < argc; i++ ) {
      dgd_cache_delete( cache, argv[i] );
   }
   printf( "free cache: %u\n", dgd_ring_size( cache->free_ring ) );

   return 0;
}

/* 
 * Local Variables:
 * compile-command: "make parser.obj"
 * End:
 */

