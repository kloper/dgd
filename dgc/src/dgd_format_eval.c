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
 * dgd_format_eval.c -- eval compile cache
 *
 */

#include "dgd_format_eval.h"

#define EVAL_ACTION_LOOKUP_SIZE 512

#define DGD_INT_ACTION 0
#define DGD_OCT_ACTION 1
#define DGD_HEX_ACTION 2
#define DGD_FREE_ACTION 2

typedef struct _dgd_action_lookup_t {
      char *name;
      dgd_action_callback_t action;
} dgd_action_lookup_t;

dgd_action_lookup_t dgd_action_lookup_table[EVAL_ACTION_LOOKUP_SIZE] = {
   { NULL, NULL }
};

static
dgd_action_callback_t* lookup( char* name ) {
   for( dgd_action_lookup_t *curr = dgd_action_lookup_table;
	curr->name != NULL &&
	   curr < dgd_action_lookup_table + EVAL_ACTION_LOOKUP_SIZE;
	curr++ ) {
      if( strcmp( name, curr->name ) == 0 )
	 return curr->action;
   }
   return NULL;
}

void 
dgd_eval_init( dgd_eval_t   *eval,       
	       unsigned int  flags,
	       cache_item_t *chain,
	       void         *user_data,
	       unsigned int  flags) {
   memset( (char*)eval, '\0', sizeof( dgd_eval_t ) );
   eval->state     = EVAL_STATE_INIT;
   eval->flags     = flags;
   eval->chain     = chain;
   eval->next_item = chain->value.chain.ring;
   eval->user_data = user_data;   
}

int 
dgd_format_eval( dgd_eval_t *eval, str_bounded_range_t *str, va_list arg ) {
   cache_item_t *eval_item;
   int res = EVAL_RES_DONE;
   str_range_t *lexeme;
   unsigned int size;
   unsigned int rc;

   if( eval == NULL || str == NULL ) {
      if( eval != NULL ) 
	 eval->error = EVAL_ERR_BAD_ARG;
      return EVAL_RES_ERROR;
   }

   if( dgd_out_of_range(str) ) {
      eval->error = EVAL_ERR_BAD_ARG;
      return EVAL_RES_ERROR;
   }


   eval->state = EVAL_STATE_NORMAL;

   eval_item = eval->next_item;

   do {
      switch( eval_item->type ) {
	 case PARS_T_WORD:
	    if( eval->state == EVAL_STATE_RANGED ) 
	       lexeme = (lexeme*)data;
	    else 
	       lexeme = &eval_item->value.lexeme;
	    
	    size = min( lexeme->end - lexeme->begin,
			str->high_bound - str->end );

	    if(size > 0 ) {
	       memcpy( (char*)str->end, lexeme->begin, size );
	       str->end += size;
	       lexeme->begin += size;
	    }

	    if( lexeme->begin != lexeme->end ) {
	       *(lexeme*)data = lexeme;
	       res = EVAL_RES_RANGE;
	       goto finish;
	    } 
	    eval->state = EVAL_STATE_NORMAL;

	    break;
	 case PARS_T_BACKSLASH:
	    size = min( 1, str->high_bound - str->end );

	    if(size > 0 ) {
	       *str->end = eval_item->value.ch;
	       str->end++;
	       eval->state = EVAL_STATE_NORMAL;
	    } else {
	       res = EVAL_RES_RANGE;
	       goto finish;
	    }

	    eval->state = EVAL_STATE_NORMAL;

	    break;
	 default:
	    break;
      }
      dgd_ring_forward(eval_item);
   } while( eval_item != eval->chain->hash.ring );

  finish:
   return res;
   
}

static
void dgd_ring_push_and_sort( cache_item_t **ring, cache_item_t *item ) {
   cache_item_t *prev;

   prev = (*ring)->prev;   

   if( *ring == prev )
      return;

   do {
      if( prev->value.argload.index >= item->value.argload.index )
	 break;
      prev = prev->prev;
   } while( prev != *ring );
	
   if( prev == *ring && 
       prev->value.argload.index < item->value.argload.index ) {
      dgd_ring_push_front( ring, item );
   } else {
      dgd_ring_push_back( &prev, item );
   }
}

cache_item_t*
dgd_format_settle_args( cache_t *cache, cache_item_t *parse_ring ) {
   cache_item_t *eval_item;
   cache_item_t *parse_prefix = NULL;
   cache_item_t *ring = NULL;
   unsigned argn = 0;

   eval_item = parse_ring;

   do {
      switch( eval_item->type ) {
	 case PARS_T_SET_ARG:
	    argn = eval_item->value.set_arg.num;
	    break;
	 case PARS_T_NEXT_ARG:
	    if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
	       goto error;
	    
	    ring->type = EVAL_T_PTR;
	    ring->value.argload.index = argn++;
	    ring->value.argload.attr.valid_mask = 0;

	    dgd_ring_push_and_sort( &parse_prefix, ring );
	    
	    eval_item->value.next_arg.arg = ring;
	    break;
	 case PARS_T_DEC:
	 case PARS_T_OCT:
	 case PARS_T_UNSIGNED:
	 case PARS_T_HEX:
	 case PARS_T_CHAR:

	 case PARS_T_SCI:
	 case PARS_T_FLOAT:
	 case PARS_T_SCIORFLOAT:
	 case PARS_T_SCIHEX:

	 case PARS_T_STR:
	 case PARS_T_PTR:
	 case PARS_T_REP:
	    break;

	    if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
	       goto error;
	    
	    switch( eval_item->type ) {
	       	 case PARS_T_SCI:
	       case PARS_T_FLOAT:
	       case PARS_T_SCIORFLOAT:
	       case PARS_T_SCIHEX:
		  ring->type = EVAL_T_DOUBLE;
		  break;
	       case PARS_T_STR:
	       case PARS_T_PTR:
	       case PARS_T_REP:
		  ring->type = EVAL_T_PTR;
		  break;
	       default:
		  ring->type = EVAL_T_INT;
		  break;
	    }

	    ring->value.argload.index = argn++;
	    dgd_call_attr_assign( &(ring->value.argload.attr),
				  &(eval_item->value.call.attr) );

	    dgd_ring_push_and_sort( &parse_prefix, ring );
	    
	    eval_item->value.call.arg = ring;


	    if( eval_item->value.call.attr.width <= 0 ) {
	       if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
		  goto error;
	       
	       ring->type = EVAL_T_INT;
	       if( eval_item->value.call.attr.width < 0 ) 
		  argn = -eval_item->value.call.attr.width;
	       ring->value.argload.index = argn++;
	       ring->value.argload.attr.valid_mask = 0;

	       dgd_ring_push_and_sort( &parse_prefix, ring );
	       
	       eval_item->value.call.width = ring;	       
	    }

	    if( eval_item->value.call.attr.precision <= 0 ) {
	       if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
		  goto error;
	       
	       ring->type = EVAL_T_INT;
	       if( eval_item->value.call.attr.precision < 0 ) 
		  argn = -eval_item->value.call.attr.precision;
	       ring->value.argload.index = argn++;
	       ring->value.argload.attr.valid_mask = 0;

	       dgd_ring_push_and_sort( &parse_prefix, ring );
	       
	       eval_item->value.call.precision = ring;	       
	    }	       

	    break;
	 default:
	    break;
      }

      dgd_ring_forward(eval_item);
   } while( eval_item != parse_ring );

  finish:
   dgd_ring_push_front( &parse_ring, parse_prefix );
   return parse_ring;

  error:
   dgd_cache_free( cache, &parse_prefix, -1 );
   return NULL;
}
