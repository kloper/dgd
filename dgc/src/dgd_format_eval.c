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

#include "dgd_config.h"
#include "dgd_types.h"
#include "dgd_format_eval.h"
#include "dgd_format_parser.h"

dgd_action_lookup_t dgd_action_lookup_table[EVAL_ACTION_LOOKUP_SIZE] = {
   { "error",    NULL, { 0, 0, 0, 0 } },
   { "dec",      NULL, { 0, 0, 0, 0 } },
   { "oct",      NULL, { 0, 0, 0, 0 } },
   { "hex",      NULL, { 0, 0, 0, 0 } },
   { "unsigned", NULL, { 0, 0, 0, 0 } },
   { "report",   NULL, { 0, 0, 0, 0 } },
   { "sci",      NULL, { 0, 0, 0, 0 } },
   { "float",    NULL, { 0, 0, 0, 0 } },
   { "scifloat", NULL, { 0, 0, 0, 0 } },
   { "scihex",   NULL, { 0, 0, 0, 0 } },
   { "char",     NULL, { 0, 0, 0, 0 } },
   { "str",      NULL, { 0, 0, 0, 0 } },
   { "ptr",      NULL, { 0, 0, 0, 0 } },
   { NULL,       NULL, { 0, 0, 0, 0 } }
};

static
dgd_action_callback_t lookup( char* name ) {
   dgd_action_lookup_t *curr;

   for( curr = dgd_action_lookup_table;
	curr->name != NULL &&
	   curr < dgd_action_lookup_table + EVAL_ACTION_LOOKUP_SIZE;
	curr++ ) {
      if( strcmp( name, curr->name ) == 0 )
	 return curr->callback;
   }
   return NULL;
}

void 
dgd_eval_init( dgd_eval_t   *eval,       
	       unsigned int  flags,
	       cache_item_t *chain,
	       void         *user_data ) {
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
   int index;
   unsigned int rc;
   dgd_action_callback_t callback;

   if( eval == NULL || str == NULL ) {
      if( eval != NULL ) 
	 eval->error = EVAL_ERR_BAD_ARG;
      return EVAL_RES_ERROR;
   }

   if( dgd_out_of_range(str) ) {
      eval->error = EVAL_ERR_BAD_ARG;
      return EVAL_RES_ERROR;
   }

   eval_item = eval->next_item;

   do {
      index = -1;

      switch( eval_item->type ) {
	 case EVAL_T_INT:
	    eval_item->value.argload.num = va_arg( arg, int );
	    eval->state = EVAL_STATE_NORMAL;
	    break;
	 case EVAL_T_DOUBLE:
	    eval_item->value.argload.dnum = va_arg( arg, double );
	    eval->state = EVAL_STATE_NORMAL;
	    break;
	 case EVAL_T_PTR:
	    eval_item->value.argload.ptr = va_arg( arg, void* );
	    eval->state = EVAL_STATE_NORMAL;
	    break;
	 case PARS_T_WORD:
	    if( eval->state == EVAL_STATE_RANGED ) 
	       lexeme = (str_range_t*)eval->data;
	    else 
	       lexeme = &(eval_item->value.word.lexeme);
	    
	    size = min( lexeme->end - lexeme->begin,
			str->high_bound - str->end );

	    if(size > 0 ) {
	       memcpy( (char*)str->end, lexeme->begin, size );
	       str->end += size;
	       lexeme->begin += size;
	    }

	    if( lexeme->begin != lexeme->end ) {
	       *(str_range_t*)eval->data = *lexeme;
	       res = EVAL_RES_RANGE;
	       goto finish;
	    } 
	    eval->state = EVAL_STATE_NORMAL;

	    break;
	 case PARS_T_BACKSLASH:
	    size = min( 1, str->high_bound - str->end );

	    if(size > 0 ) {
	       *str->end = eval_item->value.backslash.ch;
	       str->end++;
	       eval->state = EVAL_STATE_NORMAL;
	    } else {
	       res = EVAL_RES_RANGE;
	       goto finish;
	    }

	    eval->state = EVAL_STATE_NORMAL;

	    break;
	 case PARS_T_DEC:
	    index = EVAL_ACTION_DEC;

	    callback = dgd_action_lookup_table[index].callback;
	    if( callback != NULL ) {
	       dgd_action_t *action;

	       if( eval->state == EVAL_STATE_NORMAL ) {
		  action = &(dgd_action_lookup_table[index].action);
		  action->state     = EVAL_STATE_NORMAL;
		  action->flags     = eval->flags;
		  action->error     = 0;
		  action->user_data = eval->user_data;
		  action->data      = eval->data;
		  action->data_size = sizeof(eval->data);
		  dgd_call_attr_assign( &(action->attr), 
					&(eval_item->value.call.attr) );
		  if( (action->attr.valid_mask & CALL_ATTR_WIDTH) &&
		      action->attr.width <= 0 && 
		      eval_item->value.call.width != NULL ) {
		     action->attr.width = 
			eval_item->value.call.width->value.argload.num;
		  }
		  if( (action->attr.valid_mask & CALL_ATTR_PRECISION) &&
		      action->attr.precision <= 0 && 
		      eval_item->value.call.precision != NULL ) {
		     action->attr.precision = 
			eval_item->value.call.precision->value.argload.num;
		  }
	       }
	       
	       rc = EVAL_RES_DONE;
	       if( eval_item->value.call.arg != NULL ) {
		  rc = callback( action, str, 
				 &(eval_item->
				   value.call.arg->
				   value.argload.num) );
	       }

	       switch( rc ) {
		  case EVAL_RES_DONE:
		     break;
		  case EVAL_RES_ERROR:
		     eval->error = EVAL_ERR_CALLBACK;
		     eval->state = EVAL_STATE_NORMAL;
		     res = rc;
		     goto finish;
		  case EVAL_RES_RANGE:
		     eval->state = EVAL_STATE_RANGED;
		     res = rc;
		     goto finish;
	       }			      
	    }
	    break;
	 default:
	    break;
      }
      dgd_ring_forward(eval_item);
   } while( eval_item != eval->chain->value.chain.ring );

  finish:
   eval->next_item = eval_item;
   return res;
   
}


