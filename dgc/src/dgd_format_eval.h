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

#define EVAL_STATE_INIT   0
#define EVAL_STATE_NORMAL 1
#define EVAL_STATE_RANGED 2

#define EVAL_RES_DONE     0
#define EVAL_RES_RANGE    1
#define EVAL_RES_ERROR    -1

#define EVAL_ERR_UNKNOWN   0
#define EVAL_ERR_BAD_ARG   1
#define EVAL_ERR_BAD_CACHE 2
#define EVAL_ERR_BAD_CALL  3
#define EVAL_ERR_CONT_ARGS 4

#define EVAL_T_INT         20
#define EVAL_T_DOUBLE      21
#define EVAL_T_PTR         22

#define EVAL_FLAG_CACHE_CALLS 0x00000001
#define EVAL_FLAG_CACHE_STDS  0x00000002
#define EVAL_ESTIMATE         0x00000004

#define EVAL_DATA_SIZE 512

typedef struct _dgd_action_t {
      unsigned int  flags;
      unsigned int  error;
      void         *user_data;
      char         *data;
} dgd_action_t;

typedef int (dgd_action_callback_t*)( dgd_action_t *action, 
				      str_bounded_range_t *str,
				      call_attr_t *attr,
				      int argc,
				      char **argv );

typedef struct _dgd_eval_t {
      unsigned int  state;
      unsigned int  flags;
      unsigned int  error;
      unsigned int  argn;
      cache_item_t *chain;
      cache_item_t *next_item;
      void         *user_data;
      char          data[EVAL_DATA_SIZE];      
} dgd_eval_t;


#endif /* _dgd_format_eval_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_format_eval.obj"
 * End:
 */


