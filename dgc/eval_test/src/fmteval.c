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

int my_string_callback( dgd_action_t *action, 
			str_bounded_range_t *str,
			void* argv ) {
   typedef struct {
	 unsigned int argn;
	 char        *curr;
   } persistent_t;

   persistent_t *p;
   ext_args_t *args = (ext_args_t*)argv;
   unsigned int i, size;

   if( args == NULL || 
       args->argc == 0 || args->argc > DGD_MAX_EXT_ARGS ||
       args->argv == NULL ) {
      action->error = EVAL_ERR_BAD_ARG;
      return EVAL_RES_ERROR;
   }

   if( action->data_size < sizeof(persistent_t) || action->data == NULL ) {
      action->error = EVAL_ERR_SMALLDATA;
      return EVAL_RES_ERROR;
   }
   
   p = (persistent_t*)action->data;

   if( action->state == EVAL_STATE_NORMAL ) {
      p->argn = 0;
      p->curr = ((ext_arg_t*)(args->argv))->value.begin;
   }

   for( i = p->argn; i < args->argc; i++ ) {
      ext_arg_t *pair = (ext_arg_t*)(args->argv) + i;      
      size = min( pair->value.end - p->curr, 
		  (str->high_bound - str->end) );
      if(size > 0 ) {
	 memcpy( (char*)str->end, p->curr, size );
	 str->end += size;
	 p->curr += size;
      }

      if( p->curr != pair->value.end ) {
	 action->state = EVAL_STATE_RANGED;
	 return EVAL_RES_RANGE;
      } else {
	 p->argn++;
	 p->curr = ((ext_arg_t*)(args->argv) + i + 1)->value.begin;
      }
   } 

   return EVAL_RES_DONE;
}

void my_init_callbacks() {
   dgd_action_lookup_t *curr;
   
   for( curr = dgd_action_lookup_table;
	curr->name != NULL;
	curr++ ) {
      if( curr == dgd_action_lookup_table + EVAL_ACTION_LOOKUP_SIZE - 1 )
	 return;
   }
   
   memset( (char*)curr, '\0', sizeof( *curr ) );
   curr->name = "s";   
   curr->callback = my_string_callback;
   (curr+1)->name = NULL;
}

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
   chain = dgd_format_parse( format, PARS_FLAG_DEFAULT );
   dgd_init_bounded_range( &str, buffer, sizeof(buffer) );
   
   if( chain == NULL )
      return;

   dgd_dump_parser_bytecode( stderr, chain->value.chain.ring, 0 );
   printf( "free cache: %u\n", dgd_ring_size( cache->free_ring ) );

   dgd_eval_init( &eval, EVAL_FLAG_DEFAULT, chain, NULL );

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
   unsigned int n = -1;

   dgd_generic_callback_init( dgd_action_lookup_table );
   my_init_callbacks();

   dgd_printf( "{%d}\n", 101 );
   dgd_printf( "{%10d}\n", 102 );
   dgd_printf( "{%*d}\n", 20, 103 );
   dgd_printf( "{%*.*d}\n", 20, 15, 104 );
   dgd_printf( "{%-*.*d}\n", 20, 15, 105 );
   dgd_printf( "{%4$*.*d}{%3$d}\n", 20, 15, 106, 107 );
   dgd_printf( "{%4$*2$.*1$d}{%3$-*1$d}\n", 10, 15, 108, 109 );
   dgd_printf( "{%*.*d}{%3$#*1$.*2$d}\n", 10, 15, 108, 109 );
   dgd_printf( "{%hhd}\n", 0x7fffffff );
   dgd_printf( "{%hhq}{%d}{%3$d}\n", 110 );
   dgd_printf( "{%1$d}{%1$X}{%1$u}{%1$o}\n", 0xfafafafa );
   dgd_printf( "{%1$e}{%1$f}{%1$E}{%1$a}{%1$g}{%1$G}\n", 123.456 );
   dgd_printf( "{%1$c}{%1$d}{%1$x}{%1$10.5c}\n", 'q' );
   dgd_printf( "{%1$c}{%1$d}{%1$x}{%1$10.5c}\n", 0x7172 );
   dgd_printf( "{%1$s}{%1$10s}{%1$10.5s}\n", "hello world" );
   dgd_printf( "{%1$p}{%1$x}\n", "hello" );
   dgd_printf( "{%s}{%n}\n", "zz", &n ); 
   dgd_printf( "{%d}\n", n );
   dgd_printf( "{%{s:*}}{%d}\n", "hello",15 );
   return 0;
}

/* 
 * Local Variables:
 * compile-command: "make fmteval.obj"
 * End:
 */

