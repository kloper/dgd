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

static unsigned int dgd_eval_argc( cache_item_t *eval_item );

dgd_action_lookup_t dgd_action_lookup_table[EVAL_ACTION_LOOKUP_SIZE] = {
   { "error",    NULL },
   { "dec",      NULL },
   { "oct",      NULL },
   { "hex",      NULL },
   { "unsigned", NULL },
   { "report",   NULL },
   { "sci",      NULL },
   { "float",    NULL },
   { "scifloat", NULL },
   { "scihex",   NULL },
   { "char",     NULL },
   { "str",      NULL },
   { "ptr",      NULL },
   { NULL,       NULL }
};

static
int lookup( str_range_t *name ) {
   dgd_action_lookup_t *curr;

   for( curr = dgd_action_lookup_table;
	curr->name != NULL &&
	   curr < dgd_action_lookup_table + EVAL_ACTION_LOOKUP_SIZE;
	curr++ ) {
      if( strncmp( name->begin, curr->name, name->end - name->begin ) == 0 &&
	  *(curr->name + (name->end - name->begin)) == '\0' )
	 return curr-dgd_action_lookup_table;
   }
   return -1;
}

void 
dgd_eval_init( dgd_eval_t   *eval,       
	       unsigned int  flags,
	       cache_item_t *chain,
	       void         *user_data ) {
   memset( (char*)eval, '\0', sizeof( dgd_eval_t ) );
   eval->state     = EVAL_STATE_NORMAL;
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
   unsigned int size, i;
   int index;
   unsigned int rc;
   dgd_action_callback_t callback;
   cache_item_t tmp;
   ext_arg_t ext_args[DGD_MAX_EXT_ARGS];

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
	    eval_item->value.argload.v.num = va_arg( arg, int );
	    eval->state = EVAL_STATE_NORMAL;
	    break;
	 case EVAL_T_DOUBLE:
	    eval_item->value.argload.v.dnum = va_arg( arg, double );
	    eval->state = EVAL_STATE_NORMAL;
	    break;
	 case EVAL_T_PTR:
	    eval_item->value.argload.v.ptr = va_arg( arg, void* );
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
	       eval->state = EVAL_STATE_RANGED;
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
	       eval->state = EVAL_STATE_RANGED;
	       res = EVAL_RES_RANGE;
	       goto finish;
	    }

	    eval->state = EVAL_STATE_NORMAL;

	    break;
	 case PARS_T_ERROR:
	    index = EVAL_ACTION_ERROR;
	    callback = dgd_action_lookup_table[index].callback;
	    if( callback != NULL ) {
	       dgd_action_t *action;
	       dgd_error_arg_t err_arg;

	       action = &(dgd_action_lookup_table[index].action);
	       if( eval->state == EVAL_STATE_NORMAL ) {
		  action->state           = EVAL_STATE_NORMAL;
		  action->flags           = eval->flags;
		  action->error           = 0;
		  action->user_data       = eval->user_data;
		  action->data            = eval->data;
		  action->data_size       = sizeof(eval->data);
		  action->attr.valid_mask = 0;		  
	       }

	       err_arg.error   = eval_item->value.error.error;
	       err_arg.num     = eval_item->value.error.num;
	       err_arg.token   = eval_item->value.error.lexeme;
	       
	       rc = callback( action, str, &err_arg );

	       switch( rc ) {
		  case EVAL_RES_DONE:
		  case EVAL_RES_ERROR:
		     action->attr.valid_mask = 0;
		     if( action->error != 0 ) 
			eval->error = action->error;
		     else 
			eval->error = EVAL_ERR_CALLBACK;
		     eval->state = EVAL_STATE_NORMAL;
		     res = rc;
		     if( eval->flags & EVAL_FLAG_IGNORE_ERRORS ) 
			break;
		     else
			goto finish;
		  case EVAL_RES_RANGE:
		     eval->state = EVAL_STATE_RANGED;
		     res = rc;
		     goto finish;
	       }			       
	    }
	    break;
	 case PARS_T_CALL_BY_NAME:
	    index = eval_item->value.call.index;

	    if( index < 0 ) 
	       index = lookup( &(eval_item->value.call.name) );

	    if( index < 0 ) {
	       eval->error = EVAL_ERR_LOOKUP;
	       res = EVAL_RES_ERROR;
	       eval->state = EVAL_STATE_NORMAL;
	       goto finish;
	    }

	    eval_item->value.call.index = index;
	    eval_item->value.call.arg   = &tmp;

	    if( eval_item->value.call.num_param < 0 ) 
	       tmp.value.argload.v.args.argc = 
		  eval_item->value.call.num_param =
		  dgd_eval_argc( eval_item->next );

	    if( tmp.value.argload.v.args.argc <= DGD_MAX_EXT_ARGS ) {
	       cache_item_t *iterator = eval_item;
	       tmp.value.argload.v.args.argv = ext_args;
	       
	       dgd_ring_forward(iterator);
	       for( i = 0; 
		    i < tmp.value.argload.v.args.argc && 
		       i < DGD_MAX_EXT_ARGS;
		    i++ ) {
		  rc = dgd_iterate_ext_arg( &iterator,  ext_args + i );
		  if( rc == EVAL_RES_ERROR ) {
		     eval->error = EVAL_ERR_EXTARG;
		     res = EVAL_RES_ERROR;
		     eval->state = EVAL_STATE_NORMAL;
		     if( eval->flags & EVAL_FLAG_IGNORE_ERRORS ) 
			break;
		     else 
			goto finish;
		     
		  }
	       }
	    } else {
	       tmp.value.argload.v.args.argv = eval_item;
	    }

	    /* fall through */
	 case PARS_T_OCT:
	    if( index < 0 ) 
	       index = EVAL_ACTION_OCT;
	    /* fall through */
	 case PARS_T_HEX:
	    if( index < 0 ) 
	       index = EVAL_ACTION_HEX;
	    /* fall through */
	 case PARS_T_UNSIGNED:
	    if( index < 0 ) 
	       index = EVAL_ACTION_UNSIGNED;
	    /* fall through */
	 case PARS_T_SCI:
	    if( index < 0 ) 
	       index = EVAL_ACTION_SCI;
	    /* fall through */
	 case PARS_T_FLOAT:
	    if( index < 0 ) 
	       index = EVAL_ACTION_FLOAT;
	    /* fall through */
	 case PARS_T_SCIORFLOAT:
	    if( index < 0 ) 
	       index = EVAL_ACTION_SCIFLOAT;
	    /* fall through */
	 case PARS_T_SCIHEX:
	    if( index < 0 ) 
	       index = EVAL_ACTION_SCIHEX;
	    /* fall through */
	 case PARS_T_CHAR:
	    if( index < 0 ) 
	       index = EVAL_ACTION_CHAR;
	    /* fall through */
	 case PARS_T_STR:
	    if( index < 0 ) 
	       index = EVAL_ACTION_STR;
	    /* fall through */
	 case PARS_T_PTR:
	    if( index < 0 ) 
	       index = EVAL_ACTION_PTR;
	    /* fall through */
	 case PARS_T_REP:
	    if( index < 0 ) 
	       index = EVAL_ACTION_REPORT;
	    /* fall through */
	 case PARS_T_DEC:
	    if( index < 0 ) 
	       index = EVAL_ACTION_DEC;

	    callback = dgd_action_lookup_table[index].callback;
	    if( callback != NULL ) {
	       dgd_action_t *action;

	       action = &(dgd_action_lookup_table[index].action);

	       if( eval->state == EVAL_STATE_NORMAL ) {
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
			eval_item->value.call.width->value.argload.v.num;
		  }
		  if( (action->attr.valid_mask & CALL_ATTR_PRECISION) &&
		      action->attr.precision <= 0 && 
		      eval_item->value.call.precision != NULL ) {
		     action->attr.precision = 
			eval_item->value.call.precision->value.argload.v.num;
		  }
	       }
	       
	       rc = EVAL_RES_DONE;
	       if( eval_item->value.call.arg != NULL ) {
		  rc = callback( action, str, 
				 &(eval_item->
				   value.call.arg->
				   value.argload.v) );
	       } 

	       switch( rc ) {
		  case EVAL_RES_DONE:
		     action->attr.valid_mask = 0;
		     eval->state = EVAL_STATE_NORMAL;
		     break;
		  case EVAL_RES_ERROR:
		     action->attr.valid_mask = 0;
		     if( action->error != 0 ) 
			eval->error = action->error;
		     else 
			eval->error = EVAL_ERR_CALLBACK;
		     eval->state = EVAL_STATE_NORMAL;
		     res = rc;
		     if( eval->flags & EVAL_FLAG_IGNORE_ERRORS ) 
			break;
		     else 
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

#define EXTARG_STATE_NORMAL     0
#define EXTARG_STATE_PAIR_NAME  1
#define EXTARG_STATE_PAIR_VALUE 2
#define EXTARG_STATE_DONE       3

int dgd_iterate_ext_arg( void *iterator,  ext_arg_t *arg ) {
   cache_item_t **eval_item = (cache_item_t**) iterator;
   unsigned int res = EVAL_RES_DONE;
   unsigned int state = EXTARG_STATE_NORMAL;
   str_range_t *fill_range;

   arg->name.begin = arg->name.end = NULL;
   arg->value.begin = arg->value.end = NULL;

   while( state != EXTARG_STATE_DONE ) {
      switch( state ) {
	 case EXTARG_STATE_PAIR_VALUE:
	 case EXTARG_STATE_NORMAL:
	    state = EXTARG_STATE_DONE;
	    fill_range = &(arg->value);
	    break;
	 case EXTARG_STATE_PAIR_NAME:
	    state = EXTARG_STATE_PAIR_VALUE;
	    fill_range = &(arg->name);
	    break;
      }

      switch( (*eval_item)->type ) {
	 case PARS_T_LEXEME:
	    *fill_range = (*eval_item)->value.lexeme.lexeme;
	    break;
	 case PARS_T_NEXT_ARG:
	    if( (*eval_item)->value.next_arg.arg != NULL ) {
	       fill_range->begin = 
		  (*eval_item)->value.next_arg.arg->value.argload.v.ptr;
	       fill_range->end = 
		  fill_range->begin + strlen( fill_range->begin );
	    }
	    break;
	 case PARS_T_PAIR:
	    state = EXTARG_STATE_PAIR_NAME;
	    break;
	 default:
	    res = EVAL_RES_ERROR;
	    goto finish;
      }
      dgd_ring_forward( *eval_item );
   }

  finish:
   return res;
}

static
unsigned int dgd_eval_argc( cache_item_t *eval_item ) {
   unsigned int count = 0;
   cache_item_t *first = eval_item;

   do {
      switch( eval_item->type ) {
	 case PARS_T_LEXEME:
	 case PARS_T_NEXT_ARG:
	    count++;
	    break;
	 case PARS_T_PAIR:
	    count++;
	    dgd_ring_forward( eval_item );
	    dgd_ring_forward( eval_item );
	    break;
	 default:
	    return count;
      }
      dgd_ring_forward( eval_item );
   } while( eval_item != first );

   return count;
}
