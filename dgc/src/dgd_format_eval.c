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

#define DGD_CACHE_ARG_FLAG    0x00000001
#define DGD_CACHE_WIDTH_FLAG  0x00000002
#define DGD_CACHE_PREC_FLAG   0x00000004
#define DGD_CACHE_ARGP_FLAG   0x00000008
#define DGD_CACHE_WIDTHP_FLAG 0x00000010
#define DGD_CACHE_PRECP_FLAG  0x00000020

typedef struct _dgd_action_lookup_t {
      char *name;
      dgd_action_callback_t action;
} dgd_action_lookup_t;

dgd_action_lookup_t dgd_action_lookup_table[EVAL_ACTION_LOOKUP_SIZE] = {
   { "int", dgd_int },
   { "oct", dgd_oct },
   { "hex", dgd_hex }
};

dgd_action_lookup_t *dgd_free_action = 
                                 dgd_action_lookup_table + DGD_FREE_ACTION + 1;

static
dgd_action_callback_t* lookup( char* name ) {
   for( dgd_action_lookup_t *curr = dgd_action_lookup_table;
	curr < dgd_free_action;
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
	       unsigned int  flags;
	       va_arg        arg ) {
   memset( (char*)eval, '\0', sizeof( dgd_eval_t ) );
   eval->state     = EVAL_STATE_INIT;
   eval->flags     = flags;
   eval->first_arg = arg;
   eval->next_arg  = arg;
   eval->chain     = chain;
   eval->next_item = chain->value.hash.ring;
   eval->user_data = user_data;   
}

int 
dgd_format_eval( dgd_eval_t *eval, str_bounded_range_t *str ) {
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

   if( EVAL_STATE_INIT ) {
      dgd_format_setle_argn( eval );
      if( eval->argn > 0 ) {
	 rc = dgd_format_setle_argp( eval );
	 if( rc == EVAL_RES_ERROR )
	    return rc;
      }
      eval->state = EVAL_STATE_NORMAL;
   }

   eval_item = eval->next_item;

   do {
      switch( eval_item->type ) {
	 case PARS_T_SET_ARG:
	    
	    break;
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

	    break;
	 case PARS_T_CHAR:
	    size = min( 1, str->high_bound - str->end );

	    if(size > 0 ) {
	       *str->end = eval_item->value.ch;
	       str->end++;
	    } else {
	       res = EVAL_RES_RANGE;
	       goto finish;
	    }

	    break;
	 default:
	    eval->error = EVAL_ERR_BAD_CACHE;
	    res = EVAL_RES_ERROR;
	    goto finish;
      }
      dgd_ring_forward(eval_item);
   } while( eval_item != eval->chain->hash.ring );

  finish:
   return res;
   
}

static
void
dgd_format_setle_argn( dgd_eval_t *eval ) {
   cache_item_t *eval_item;

   unsigned argn = 0;

   eval_item = eval->chain->value.hash.ring;

   do {

      switch( eval_item->type ) {
	 case PARS_T_SET_ARG:
	    argn = eval_item->value.num;
	    break;
	 case PARS_T_NEXT_ARG:
	    eval_item->cache.flags |= DGD_CACHE_ARG_FLAG;
	    eval_item->cache.argn[0] = argn++;
	    break;
	 case PARS_T_READ_DEC:
	 case PARS_T_READ_OCT:
	 case PARS_T_READ_UNSIGNED:
	 case PARS_T_READ_HEX:
	 case PARS_T_WRITE_REP:
	 case PARS_T_READ_SCI:
	 case PARS_T_READ_FLOAT:
	 case PARS_T_READ_SCIORFLOAT:
	 case PARS_T_READ_SCIHEX:
	 case PARS_T_READ_CHAR:
	 case PARS_T_READ_STR:
	 case PARS_T_READ_PTR:
	    eval_item->cache.flags |= DGD_CACHE_ARG_FLAG;
	    eval_item->cache.argn[0] = argn++;

	    if( eval_item->value.hash.attr.width == 0 ) {
	       eval_item->cache.flags |= DGD_CACHE_WIDTH_FLAG;
	       eval_item->cache.argn[1] = argn++;
	    } else if(  eval_item->value.hash.attr.width < 0 ) {
	       argn = -eval_item->value.hash.attr.width;
	       eval_item->cache.flags |= DGD_CACHE_WIDTH_FLAG;
	       eval_item->cache.argn[1] = argn++;
	    }

	    if( eval_item->value.hash.attr.precision == 0 ) {
	       eval_item->cache.flags |= DGD_CACHE_PREC_FLAG;
	       eval_item->cache.argn[2] = argn++;
	    } else if(  eval_item->value.hash.attr.precision < 0 ) {
	       argn = -eval_item->value.hash.attr.precision;
	       eval_item->cache.flags |= DGD_CACHE_PREC_FLAG;
	       eval_item->cache.argn[2] = argn;
	    }	       
	 default:
	    break;
      }

      dgd_ring_forward(eval_item);
   } while( eval_item != eval->chain->hash.ring );

   eval->argn = argn;
}

static
int 
dgd_format_setle_argp( dgd_eval_t *eval ) {
   cache_item_t *eval_item, *first_item;
   int res = EVAL_RES_DONE;
   unsigned int i;
   va_list next_arg;
   bool_t found_arg;

   next_arg = eval->first_arg;
   eval_item = eval->chain->value.hash.ring;

   for( i = 0; i < eval->argn; i++ ) {
      found_arg = FALSE;
      first_item = eval_item;
      do {

	 if( (eval_item->cache.flags & DGD_CACHE_ARG_FLAG) &&
	     eval_item->cache.argn[0] == i ) {
	    eval_item->cache.flags |= DGD_CACHE_ARGP_FLAG;
	    eval_item->cache.argp[0] = next_arg;
	    switch( eval_item ) {
	       case PARS_T_READ_PTR:
	       case PARS_T_READ_STR:
	       case PARS_T_NEXT_ARG:
		  va_arg( next_arg, char* );
		  break;
	       case PARS_T_READ_DEC:
	       case PARS_T_READ_OCT:
	       case PARS_T_READ_UNSIGNED:
	       case PARS_T_READ_HEX:
	       case PARS_T_WRITE_REP:
	       case PARS_T_READ_CHAR:
		  /* bytecount does not make sense since compiler will 
		     expand short and char values to int */
		  va_arg( next_arg, int );
		  break;
	       case PARS_T_READ_SCI:
	       case PARS_T_READ_FLOAT:
	       case PARS_T_READ_SCIORFLOAT:
	       case PARS_T_READ_SCIHEX:
		  va_arg( next_arg, double );
		  break;
	       default:
		  res = EVAL_RES_ERROR;
		  eval->error = EVAL_ERR_BAD_CACHE;
		  goto finish;
	    }

	    found_arg = TRUE;
	 } else if( (eval_item->cache.flags & DGD_CACHE_WIDTH_FLAG) &&
		    eval_item->cache.argn[1] == i ) {
	    eval_item->cache.flags |= DGD_CACHE_WIDTHP_FLAG;
	    eval_item->cache.argp[1] = next_arg;
	    va_arg( next_arg, int );
	    found_arg = TRUE;
	 } else if( (eval_item->cache.flags & DGD_CACHE_PREC_FLAG) &&
		    eval_item->cache.argn[2] == i ) {
	    eval_item->cache.flags |= DGD_CACHE_PRECP_FLAG;
	    eval_item->cache.argp[2] = next_arg;
	    va_arg( next_arg, int );
	    found_arg = TRUE;
	 }

	 dgd_ring_forward(eval_item);
      } while( !found_arg && eval_item != first_item );

      if( !found_arg ) {
	 res = EVAL_RES_ERROR;
	 eval->error = EVAL_ERR_CONT_ARGS;
	 goto finish;
      }
   }
  finish:
   return res;   
}

/* 
 * Local Variables:
 * compile-command: "make dgd_format_eval.obj"
 * End:
 */

