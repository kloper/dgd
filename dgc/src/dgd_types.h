// -*- c -*-
//
// $Id$
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// This file is part of Depression Glass library.
//
// Copyright (c) 2003. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// dgd_types.h -- string utility functions
//

#ifndef _dgd_types_h_
#define _dgd_types_h_

#define CALL_ATTR_ALTERNATE   0x00000001
#define CALL_ATTR_ZERO_PAD    0x00000002
#define CALL_ATTR_LEFT_ADJUST 0x00000004
#define CALL_ATTR_BLANK       0x00000008
#define CALL_ATTR_SIGN        0x00000010
#define CALL_ATTR_CAPITAL     0x00000020
#define CALL_ATTR_WIDTH       0x00000040
#define CALL_ATTR_PRECISION   0x00000080
#define CALL_ATTR_BYTECOUNT   0x00000100
#define CALL_ATTR_ABSPOS      0x00000200

typedef struct _call_attr_t {
      unsigned int valid_mask;
      unsigned int alternate   : 1;
      unsigned int zero_pad    : 1;
      unsigned int left_adjust : 1;
      unsigned int blank       : 1;
      unsigned int sign        : 1;
      unsigned int capital     : 1;
               int width;
               int precision;
      unsigned int byte_count;
      unsigned int position;
} call_attr_t;


typedef struct _str_bounded_range_t {
      char   *begin;
      char   *end;
      char   *low_bound;
      char   *high_bound;
} str_bounded_range_t;

typedef struct _str_range_t {
      char   *begin;
      char   *end;
} str_range_t;

typedef struct _ext_arg_t {
      str_range_t name;
      str_range_t value;
} ext_arg_t;

typedef struct _ext_args_t {
      unsigned int argc;
      ext_arg_t   *argv;
} ext_args_t;

#define dgd_out_of_range(r) (((r)==NULL)||\
                         ((r)->low_bound>(r)->high_bound)||\
                         ((r)->begin<(r)->low_bound)||\
                         ((r)->begin>(r)->high_bound)||\
                         ((r)->end<(r)->low_bound)||\
                         ((r)->end>(r)->high_bound)||\
                         ((r)->end<(r)->begin))

#define dgd_init_bounded_range(r,b,s) ((r)->low_bound=(b),\
				       (r)->high_bound=((char*)b)+(s),\
				       (r)->begin=(b),\
				       (r)->end=(b))

void dgd_call_attr_set_default( call_attr_t *attr );
void dgd_call_attr_assign( call_attr_t *dst, call_attr_t *src );


char oct2char( str_range_t *range );
char hex2char( str_range_t *range );
int  dec2int ( str_range_t *range );

#endif /* _dgd_types_h_ */



