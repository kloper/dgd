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
 * dgd_types.c -- implementation of dgd_types.h
 *
 */

#include <string.h>

#include "dgd_types.h"

char oct2char( str_range_t *range ) {
   register char *begin, *end;
   register char res;

   if( (begin = range->begin) == NULL ) 
      return 0;
   
   end = range->end;

   
   res = 0;
   while( (end != NULL && begin < end) || ( end == NULL && *begin ) ) {
      res <<= 3;
      res += *begin++ - '0';
   }

   return res;
}

char hex2char( str_range_t *range ) {
   register char *begin, *end;
   register char res;

   if( (begin = range->begin) == NULL ) 
      return 0;
   
   end = range->end;

   
   res = 0;
   while( (end != NULL && begin < end) || ( end == NULL && *begin ) ) {
      res <<= 4;
      if( *begin >= '0' && *begin <= '9' )
	 res += *begin - '0';
      else if( *begin >= 'a' && *begin <= 'f' )
	 res += *begin - 'a' + 10;
      else if( *begin >= 'A' && *begin <= 'F' )
	 res += *begin - 'A' + 10;
      begin++;
   }

   return res;
}

int dec2int( str_range_t *range ) {
   register char *begin, *end;
   register int res;

   if( (begin = range->begin) == NULL ) 
      return 0;
   
   end = range->end;

   
   res = 0;
   while( (end != NULL && begin < end) || ( end == NULL && *begin ) ) {
      res *= 10;
      res += *begin++ - '0';
   }

   return res;
}


void dgd_call_attr_set_default( call_attr_t *attr ) {
   memset( (char*)attr, '\0', sizeof(call_attr_t) );
}

void dgd_call_attr_assign( call_attr_t *dst, call_attr_t *src ) {
   if( src->valid_mask & CALL_ATTR_ALTERNATE ) {
      dst->alternate = src->alternate;
   }
   if( src->zero_pad & CALL_ATTR_ZERO_PAD ) {
      dst->zero_pad = src->zero_pad;
   }
   if( src->left_adjust & CALL_ATTR_LEFT_ADJUST ) {
      dst->left_adjust = src->left_adjust;
   }
   if( src->blank & CALL_ATTR_BLANK ) {
      dst->blank = src->blank;
   }
   if( src->sign & CALL_ATTR_SIGN ) {
      dst->sign = src->sign;
   }
   if( src->capital & CALL_ATTR_CAPITAL ) {
      dst->capital = src->capital;
   }
   if( src->width & CALL_ATTR_WIDTH ) {
      dst->width = src->width;
   }
   if( src->precision & CALL_ATTR_PRECISION ) {
      dst->precision = src->precision;
   }
   if( src->byte_count & CALL_ATTR_BYTECOUNT ) {
      dst->byte_count = src->byte_count;
   }

   dst->valid_mask |= src->valid_mask;
}
