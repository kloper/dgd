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
 * dgd_format_generic.c -- generic formatting callbacks
 *
 */

#include <stdio.h>
#include <ctype.h>

#include "dgd_config.h"
#include "dgd_format_eval.h"
#include "dgd_format_parser.h"

static
char *build_format( call_attr_t *attr, char format ) {
   static char buffer[100];
   char *curr;

   curr = buffer;
   *curr++ = '%';

   if( attr->valid_mask & CALL_ATTR_ALTERNATE )   { *curr++ = '#'; }
   if( attr->valid_mask & CALL_ATTR_ZERO_PAD )    { *curr++ = '0'; }
   if( attr->valid_mask & CALL_ATTR_LEFT_ADJUST ) { *curr++ = '-'; }
   if( attr->valid_mask & CALL_ATTR_BLANK )       { *curr++ = ' '; }
   if( attr->valid_mask & CALL_ATTR_SIGN )        { *curr++ = '+'; }
   if( attr->valid_mask & CALL_ATTR_CAPITAL )     { format = toupper(format); }
   if( attr->valid_mask & CALL_ATTR_WIDTH )       {
      sprintf( curr, "%u", attr->width );
      curr += strlen(curr);
   }
   if( attr->valid_mask & CALL_ATTR_PRECISION ) {
      sprintf( curr, ".%u", attr->precision );
      curr += strlen(curr);
   }
   if( attr->valid_mask & CALL_ATTR_BYTECOUNT ) {
      switch( attr->byte_count ) {
	 case sizeof(dgd_char_t): 
	    *curr++ = 'h';
	    *curr++ = 'h';
	    break;
	 case sizeof(dgd_short_t):
	    *curr++ = 'h';
	    break;
	 case sizeof(dgd_long_t):
	    *curr++ = 'l';
	    break;
      }
   }

   *curr++ = format;
   *curr++ = '\0';
   return buffer;
}

int dgd_generic_callback( dgd_action_t *action, 
			  str_bounded_range_t *str,
			  char format,
			  void* argv ) {
   typedef struct {
	 char *curr;
	 char  buf[100];
   } persistent_t;

   persistent_t *p;
   unsigned int size;

   if( action->data_size < sizeof(persistent_t) || action->data == NULL ) {
      action->error = EVAL_ERR_SMALLDATA;
      return EVAL_RES_ERROR;
   }
   
   p = (persistent_t*)action->data;

   if( action->state == EVAL_STATE_NORMAL ) {
      p->curr = p->buf;
      switch( format ) {
	 case 'd': case 'x': case 'u': case 'o': case 'c': 
	    sprintf( p->buf, build_format( &(action->attr), format ), 
		     *(int*)argv );
	    break;
	 case 'e': case 'f': case 'a': case 'g':
	    sprintf( p->buf, build_format( &(action->attr), format ), 
		     *(double*)argv );
	    break;
	 case 's': case 'p': case 'n':
	    sprintf( p->buf, build_format( &(action->attr), format ), 
		     argv );
	    break;
      }
   }

   size = min( strlen( p->curr ), (unsigned)(str->high_bound - str->end) );
   
   if(size > 0 ) {
      memcpy( (char*)str->end, p->curr, size );
      str->end += size;
      p->curr += size;
   }

   if( *p->curr != '\0' ) {
      action->state = EVAL_STATE_RANGED;
      return EVAL_RES_RANGE;
   } 
   
   return EVAL_RES_DONE;
}

int dgd_generic_callback_dec( dgd_action_t *action, 
			      str_bounded_range_t *str,			      
			      void* argv ) {
   return dgd_generic_callback( action, str, 'd', argv );
}

int dgd_generic_callback_oct( dgd_action_t *action, 
			      str_bounded_range_t *str,			      
			      void* argv ) {
   return dgd_generic_callback( action, str, 'o', argv );
}

int dgd_generic_callback_hex( dgd_action_t *action, 
			      str_bounded_range_t *str,			      
			      void* argv ) {
   return dgd_generic_callback( action, str, 'x', argv );
}

int dgd_generic_callback_unsigned( dgd_action_t *action, 
				   str_bounded_range_t *str,
				   void* argv ) {
   return dgd_generic_callback( action, str, 'u', argv );
}

int dgd_generic_callback_char( dgd_action_t *action, 
				   str_bounded_range_t *str,
				   void* argv ) {
   return dgd_generic_callback( action, str, 'c', argv );
}

int dgd_generic_callback_sci( dgd_action_t *action, 
			      str_bounded_range_t *str,			      
			      void* argv ) {
   return dgd_generic_callback( action, str, 'e', argv );
}

int dgd_generic_callback_float( dgd_action_t *action, 
				str_bounded_range_t *str,
				void* argv ) {
   return dgd_generic_callback( action, str, 'f', argv );
}

int dgd_generic_callback_scifloat( dgd_action_t *action, 
				   str_bounded_range_t *str,
				   void* argv ) {
   return dgd_generic_callback( action, str, 'g', argv );
}

int dgd_generic_callback_scihex( dgd_action_t *action, 
				 str_bounded_range_t *str,
				 void* argv ) {
   return dgd_generic_callback( action, str, 'a', argv );
}

static
int dgd_generic_callback_error( dgd_action_t *action, 
				str_bounded_range_t *str,
				void* argv ) {
   typedef struct {
	 char *curr;
	 char  buf[100];
   } persistent_t;

   dgd_error_arg_t *err = (dgd_error_arg_t*)argv;
   char *errdescr = "unknown error";
   persistent_t *p;
   unsigned int size;

   if( action->data_size < sizeof(persistent_t) || action->data == NULL ) {
      action->error = EVAL_ERR_SMALLDATA;
      return EVAL_RES_ERROR;
   }
   
   p = (persistent_t*)action->data;

   if( action->state == EVAL_STATE_NORMAL ) {
      p->curr = p->buf;
      switch( err->error ) {
	 case PARS_ERR_SYNTAX:
	    sprintf( p->buf, "<syntax error before: %.10s>",
		     err->token.begin );
	    break;
	 case PARS_ERR_ALLOC: 
	    sprintf( p->buf, "<alloc error>" );
	    break;
	 case PARS_ERR_ARGGAP:
	    sprintf( p->buf, "<gap in arguments: %d>", err->num );
	    break;
	 case PARS_ERR_ARGTYPE:
	    sprintf( p->buf, "<gap in argument types: %d>", err->num );
	    break;
	 default:
	    sprintf( p->buf, "<unknown error>" );
	    break;
      }
   }
   
   size = min( strlen( p->curr ), (unsigned)(str->high_bound - str->end) );
   
   if(size > 0 ) {
      memcpy( (char*)str->end, p->curr, size );
      str->end += size;
      p->curr += size;
   }

   if( *p->curr != '\0' ) {
      action->state = EVAL_STATE_RANGED;
      return EVAL_RES_RANGE;
   } 
   
   return EVAL_RES_DONE;
}

void 
dgd_generic_callback_init( dgd_action_lookup_t* lookup) {
   lookup[EVAL_ACTION_DEC].callback      = dgd_generic_callback_dec;
   lookup[EVAL_ACTION_OCT].callback      = dgd_generic_callback_oct;
   lookup[EVAL_ACTION_HEX].callback      = dgd_generic_callback_hex;
   lookup[EVAL_ACTION_UNSIGNED].callback = dgd_generic_callback_unsigned;
   lookup[EVAL_ACTION_CHAR].callback     = dgd_generic_callback_char;
   lookup[EVAL_ACTION_SCI].callback      = dgd_generic_callback_sci;
   lookup[EVAL_ACTION_FLOAT].callback    = dgd_generic_callback_float;
   lookup[EVAL_ACTION_SCIFLOAT].callback = dgd_generic_callback_scifloat;
   lookup[EVAL_ACTION_SCIHEX].callback   = dgd_generic_callback_scihex;
   lookup[EVAL_ACTION_ERROR].callback    = dgd_generic_callback_error;
}

/* 
 * Local Variables:
 * compile-command: "make dgd_format_generic.obj"
 * End:
 */

