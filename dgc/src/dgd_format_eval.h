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
// dgd_format_eval.h -- declarations for format evaluator
//

#ifndef _dgd_format_eval_h_
#define _dgd_format_eval_h_

#include "dgd_config.h"
#include "dgd_types.h"
#include "dgd_format_cache.h"

#define EVAL_STATE_NORMAL 1
#define EVAL_STATE_RANGED 2

#define EVAL_RES_DONE     0
#define EVAL_RES_RANGE    1
#define EVAL_RES_ERROR    -1

#define EVAL_ERR_UNKNOWN   0
#define EVAL_ERR_BAD_ARG   1
#define EVAL_ERR_BAD_CACHE 2
#define EVAL_ERR_CONT_ARGS 3
#define EVAL_ERR_CALLBACK  4
#define EVAL_ERR_SMALLDATA 5
#define EVAL_ERR_LOOKUP    6
#define EVAL_ERR_EXTARG    7


#define EVAL_FLAG_IGNORE_ERRORS 0x00000001
#define EVAL_FLAG_DEFAULT (EVAL_FLAG_IGNORE_ERRORS)

#define EVAL_DATA_SIZE 512

#define EVAL_ACTION_LOOKUP_SIZE 512

#define EVAL_ACTION_ERROR    0
#define EVAL_ACTION_DEC      1
#define EVAL_ACTION_OCT      2
#define EVAL_ACTION_HEX      3
#define EVAL_ACTION_UNSIGNED 4
#define EVAL_ACTION_REPORT   5
#define EVAL_ACTION_SCI      6
#define EVAL_ACTION_FLOAT    7
#define EVAL_ACTION_SCIFLOAT 8
#define EVAL_ACTION_SCIHEX   9
#define EVAL_ACTION_CHAR     10
#define EVAL_ACTION_STR      11
#define EVAL_ACTION_PTR      12

typedef struct _dgd_action_t {
      unsigned int  state;
      unsigned int  flags;
      call_attr_t   attr;
      unsigned int  error;
      void         *user_data;
      unsigned int  data_size;
      char         *data;
} dgd_action_t;

typedef int (*dgd_action_callback_t)( dgd_action_t *action, 
				      str_bounded_range_t *str,
				      void* argv );

typedef struct _dgd_action_lookup_t {
      char                 *name;
      dgd_action_callback_t callback;
      dgd_action_t          action;
} dgd_action_lookup_t;

extern dgd_action_lookup_t dgd_action_lookup_table[EVAL_ACTION_LOOKUP_SIZE];

typedef struct _dgd_eval_t {
      unsigned int  state;
      unsigned int  flags;
      unsigned int  error;
      cache_item_t *chain;
      cache_item_t *next_item;
      void         *user_data;
      char          data[EVAL_DATA_SIZE];      
} dgd_eval_t;

typedef struct _dgd_error_arg_t {
      unsigned int error;
      unsigned int num;
      str_range_t  token;
} dgd_error_arg_t;

void dgd_eval_init( dgd_eval_t   *eval,       
		    unsigned int  flags,
		    cache_item_t *chain,
		    void         *user_data );

int dgd_format_eval( dgd_eval_t *eval, 
		     str_bounded_range_t *str, 
		     va_list arg );

int dgd_iterate_ext_arg( void *iterator,  ext_arg_t *arg );

#endif /* _dgd_format_eval_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_format_eval.obj"
 * End:
 */


