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
 * fmteval.c -- test format eval
 *
 */

#include <stdio.h>

#include <dgd_dgc_debug.h>
#include <dgd_format_parser.h>
#include <dgd_format_eval.h>
#include <dgd_format_generic.h>

void dgd_printf( char* format, ... ) {
   va_list arg;
   cache_t *cache;
   cache_item_t *chain;
   dgd_eval_t eval;
   unsigned int rc;
   str_bounded_range_t str;
   char buffer[5];

   va_start( arg, format );

   cache = dgd_format_parser_cache();
   chain = dgd_format_parse( format );
   dgd_init_bounded_range( &str, buffer, sizeof(buffer) );
   
   if( chain == NULL )
      return;

   dgd_dump_parser_bytecode( stderr, chain->value.chain.ring, 0 );
   printf( "free cache: %u\n", dgd_ring_size( cache->free_ring ) );

   dgd_eval_init( &eval, 0, chain, NULL );

   do {
      rc = dgd_format_eval( &eval, &str, arg );
      printf( "%.*s", str.end-str.begin, str.begin );
      str.begin = str.end;
      if( str.end == str.high_bound ) 
	 str.begin = str.end = str.low_bound;
   } while( rc == EVAL_RES_RANGE );

   va_end( arg );
}

int main( int argc, char** argv ) {

   dgd_generic_callback_init( dgd_action_lookup_table );

   dgd_printf( "{%d}\n", 101 );
   dgd_printf( "{%10d}\n", 102 );
   dgd_printf( "{%*d}\n", 20, 103 );
   dgd_printf( "{%*.*d}\n", 20, 15, 104 );
   dgd_printf( "{%-*.*d}\n", 20, 15, 105 );
   dgd_printf( "{%4$*.*d}{%3$d}\n", 20, 15, 106, 107 );
   dgd_printf( "{%4$*2$.*1$d}{%3$-*1$d}\n", 10, 15, 108, 109 );
   dgd_printf( "{%*.*d}{%3$#*1$.*2$d}\n", 10, 15, 108, 109 );
   dgd_printf( "{%hhd}\n", 0x7fffffff );
   return 0;
}

/* 
 * Local Variables:
 * compile-command: "make fmteval.obj"
 * End:
 */

