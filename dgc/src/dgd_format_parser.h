// -*- c++ -*-
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
// dgd_format_parser.h -- declarations for format parser
//

#ifndef _dgd_format_parser_h_
#define _dgd_format_parser_h_

#include "dgd_format_cache.h" /* need here because YYSTYPE will be there */

#define PARS_T_ERROR        0
#define PARS_T_PAIR         1
#define PARS_T_NEXT_ARG     2
#define PARS_T_LEXEME       4
#define PARS_T_CALL_BY_NAME 5
#define PARS_T_DEC          6
#define PARS_T_OCT          7
#define PARS_T_UNSIGNED     8
#define PARS_T_HEX          9
#define PARS_T_REP          10
#define PARS_T_SCI          11
#define PARS_T_FLOAT        12
#define PARS_T_SCIORFLOAT   13
#define PARS_T_SCIHEX       14
#define PARS_T_CHAR         15
#define PARS_T_STR          16
#define PARS_T_PTR          17
#define PARS_T_WORD         18
#define PARS_T_BACKSLASH    19

#define EVAL_T_INT          20
#define EVAL_T_DOUBLE       21
#define EVAL_T_PTR          22

/* content of y.tab.h goes here */
#define LEX_EOF 257
#define LEX_ZERO 258
#define LEX_MINUS 259
#define LEX_SPACE 260
#define LEX_PLUS 261
#define LEX_PERIOD 262
#define LEX_NUMBER 263
#define LEX_REFERENCE 264
#define LEX_PERCENT 265
#define LEX_LEFTBRACE 266
#define LEX_WORD 267
#define LEX_COLON 268
#define LEX_EQUAL 269
#define LEX_COMMA 270
#define LEX_STAR 271
#define LEX_DOLLAR 272
#define LEX_DIEZ 273
#define LEX_BACKSLASH 274
#define LEX_LEFT_BRACE 275
#define LEX_RIGHT_BRACE 276
#define LEX_MOD_HALFHALF 277
#define LEX_MOD_HALF 278
#define LEX_MOD_LONGLONG 279
#define LEX_MOD_LONG 280
#define LEX_MOD_INTMAXT 281
#define LEX_MOD_SIZET 282
#define LEX_MOD_PTRDIFFT 283
#define LEX_MOD_LONGDOUBLE 284
#define LEX_T_DEC 285
#define LEX_T_OCT 286
#define LEX_T_UNSIGNED 287
#define LEX_T_HEX_LOW 288
#define LEX_T_HEX_HIGH 289
#define LEX_T_SCI_LOW 290
#define LEX_T_SCI_HIGH 291
#define LEX_T_FLOAT_LOW 292
#define LEX_T_FLOAT_HIGH 293
#define LEX_T_SCIORFLOAT_LOW 294
#define LEX_T_SCIORFLOAT_HIGH 295
#define LEX_T_SCIHEX_LOW 296
#define LEX_T_SCIHEX_HIGH 297
#define LEX_T_CHAR 298
#define LEX_T_STRING 299
#define LEX_T_PTR 300
#define LEX_T_REPORT 301

cache_item_t *dgd_format_parse( char* format_string );
cache_t      *dgd_format_parser_cache();
cache_item_t* dgd_format_settle_args( cache_t *cache, 
				      cache_item_t *parse_ring );

#endif /* _dgd_format_parser_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_format_parser.obj"
 * End:
 */


