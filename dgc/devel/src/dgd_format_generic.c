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

#include "dgd_config.h"
#include "dgd_format_eval.h"

static
char *build_format( call_attr_t *attr, char format ) {
   static char buffer[100];
   char *curr;

   curr = buffer;

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
	 case sizeof(dgd_longlong_t):
	    *curr++ = 'll';
	    break;
      }
   }

   *curr++ = format;
   *curr++ = '\0';
   return buffer;
}

int dgd_generic_callback_dec( dgd_action_t *action, 
			     str_bounded_range_t *str,
			     void* argv ) {
   typedef struct {
	 char *curr;
	 char  buf[20];
   } persistent_t;

   persistent_t *p;

   if( action->data_size < sizeof(persistent_t) || action->data == NULL ) 
      return EVAL_RES_ERROR;
   
   p = (persistent_t*)action->data;

   if( action->state == EVAL_STATE_NORMAL ) {
      p->curr = p->buf;
      sprintf( p->buf, build_format( &(action->attr) ), *(int*)argv );
   }

   size = min( strlen( p->curr ), str->high_bound - str->end );
   
   if(size > 0 ) {
      memcpy( (char*)str->end, p->curr, size );
      str->end += size;
      p->curr += size;
   }

   if( *p->curr != '\0'; ) {
      return EVAL_RES_RANGE;
   } 
   
   return EVAL_RES_DONE;
}

void 
dgd_generic_callback_init( dgd_action_lookup_t* lookup) {
   lookup[EVAL_ACTION_DEC].callback = dgd_generic_callback_dec;
}

/* 
 * Local Variables:
 * compile-command: "make dgd_format_generic.obj"
 * End:
 */

