#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 1 "src/dgd_format_parser.y"
 

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
 * dgd_format_parser.y -- implementation of format line parser
 *
 */
   
#include <stdio.h>

#include "dgd_types.h"
#include "dgd_format_lexer.h"
#include "dgd_format_cache.h"
   
#define YYDEBUG 1

typedef union _yy_stype_t {
   lexer_value_t  lex;
   cache_item_t*  ring;
   call_attr_t    attr;
} yy_stype_t ;

#define YYSTYPE yy_stype_t

static int yylex();
static void yylexreset();
static void yyerror( char* reason );

extern cache_item_t *dgd_format_parser_result();

static lexer_state_t   lexer_state;
static int             parser_init = 0;
static default_cache_t parser_cache;
static char*           dgd_format_string = NULL;
static cache_t        *cache = NULL;
static cache_item_t   *start_ring;

#line 69 "src/dgd_format_parser.c"
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
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    1,    1,    2,    2,    3,    3,    3,    3,    4,    4,
    5,    6,    6,    6,    6,    6,    6,    7,    7,    7,
    7,    7,    7,    7,    7,    8,    9,    8,   10,   11,
   12,   12,   12,   12,   12,   12,   12,   12,   13,   13,
   14,   15,   16,   16,   16,   16,   16,   17,   17,   17,
   17,   17,   18,   18,   18,   19,   19,   19,   19,   20,
   20,   20,   20,   21,   21,    0,    0,    0,    0,    0,
    0,
};
short yylen[] = {                                         2,
    1,    2,    1,    3,    1,    1,    3,    3,    1,    1,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,    2,    1,    3,
    1,    1,    1,    1,    1,    1,    1,    0,    1,    0,
    2,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    1,    2,    2,    3,    2,
    2,    2,    0,    3,    1,    1,    1,    1,    2,    2,
    2,
};
short yydefred[] = {                                      0,
   65,    0,   67,   68,    0,    0,   66,    2,   70,   71,
   69,   48,   50,   51,   52,    0,   53,    0,   49,    0,
    0,    0,    0,   57,    0,   55,   60,   61,   62,    0,
   31,   32,   33,   34,   35,   36,   37,   39,   26,   29,
   45,    0,   46,   47,    0,    0,   43,   44,   64,   59,
    0,    0,   28,   12,   13,   14,   15,   16,   17,   41,
   18,   19,   20,   21,   22,   23,   24,   25,   42,    0,
   30,    9,   10,    0,    0,    6,    0,    0,    0,    8,
   11,
};
short yydgoto[] = {                                       5,
    6,   52,   74,   75,   76,   60,   69,   41,   42,   43,
   44,   45,   46,   47,   48,   49,   20,   21,   22,   23,
    7,
};
short yysindex[] = {                                    -52,
    0, -255,    0,    0,  -42,   -7,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -257,    0, -253,    0,   -7,
   -7,   -7,  -72,    0, -246,    0,    0,    0,    0, -209,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -292,    0,    0,  -68, -153,    0,    0,    0,    0,
 -208, -182,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -259,
    0,    0,    0, -175, -207,    0, -259, -259, -207,    0,
    0,
};
short yyrindex[] = {                                      0,
    0, -258,    0,    0,    0,  -99,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -126,    0, -214,    0,  -99,
  -99,  -99,  -51,    0, -170,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -180,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -179, -260,    0,    0,    0, -220,    0,
    0,
};
short yygindex[] = {                                      0,
    0,    0,    0,  -23,   21,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   31,
   94,
};
#define YYTABLESIZE 266
short yytable[] = {                                       1,
    1,    1,    1,    1,    1,   24,   53,   72,    8,    5,
   26,   73,    1,   25,    1,    5,    1,   50,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,   54,   54,   54,   54,   54,   54,    7,
   27,   28,   29,   79,   81,    7,   54,   51,   54,   70,
   54,   78,   54,   54,   54,   54,   54,   54,   54,   54,
   54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
   54,   54,   54,   54,   54,   54,   54,   58,   58,   58,
   58,   58,   58,   71,   77,    3,    4,   80,   11,    0,
   58,    0,   58,    0,   58,    0,   58,   58,   58,   58,
   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
   58,   56,   56,   56,   56,   56,   61,   62,   63,   64,
   65,   66,   67,   68,    0,    0,   56,    0,   56,    0,
   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
   56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
   56,   56,   56,   56,   56,   63,    0,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,
   63,   63,   30,    1,   31,   32,   33,   34,   35,   36,
   37,   38,    2,    1,    3,    0,   54,   55,   56,   57,
   58,    4,    2,    0,    9,   39,    0,   40,    0,    0,
    0,   10,   59,   38,   38,   38,   38,   38,   40,   40,
   40,   40,   40,   40,   40,   40,    0,   27,    0,   38,
   12,   13,   14,   15,   16,   17,    0,    0,    0,    0,
    0,    0,    0,   18,    0,   19,
};
short yycheck[] = {                                     258,
  259,  260,  261,  262,  263,  263,  299,  267,  264,  270,
  264,  271,  271,  271,  273,  276,  275,  264,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
  299,  300,  301,  258,  259,  260,  261,  262,  263,  270,
   20,   21,   22,   77,   78,  276,  271,  267,  273,  268,
  275,  269,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,  258,  259,  260,
  261,  262,  263,  276,  270,  276,  276,   77,    5,   -1,
  271,   -1,  273,   -1,  275,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  258,  259,  260,  261,  262,  290,  291,  292,  293,
  294,  295,  296,  297,   -1,   -1,  273,   -1,  275,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  295,  296,
  297,  298,  299,  300,  301,  275,   -1,  277,  278,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  275,  256,  277,  278,  279,  280,  281,  282,
  283,  284,  265,  256,  267,   -1,  285,  286,  287,  288,
  289,  274,  265,   -1,  267,  298,   -1,  300,   -1,   -1,
   -1,  274,  301,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  295,  296,  297,   -1,  299,   -1,  301,
  258,  259,  260,  261,  262,  263,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  271,   -1,  273,
};
#define YYFINAL 5
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 301
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"LEX_EOF","LEX_ZERO","LEX_MINUS",
"LEX_SPACE","LEX_PLUS","LEX_PERIOD","LEX_NUMBER","LEX_REFERENCE","LEX_PERCENT",
"LEX_LEFTBRACE","LEX_WORD","LEX_COLON","LEX_EQUAL","LEX_COMMA","LEX_STAR",
"LEX_DOLLAR","LEX_DIEZ","LEX_BACKSLASH","LEX_LEFT_BRACE","LEX_RIGHT_BRACE",
"LEX_MOD_HALFHALF","LEX_MOD_HALF","LEX_MOD_LONGLONG","LEX_MOD_LONG",
"LEX_MOD_INTMAXT","LEX_MOD_SIZET","LEX_MOD_PTRDIFFT","LEX_MOD_LONGDOUBLE",
"LEX_T_DEC","LEX_T_OCT","LEX_T_UNSIGNED","LEX_T_HEX_LOW","LEX_T_HEX_HIGH",
"LEX_T_SCI_LOW","LEX_T_SCI_HIGH","LEX_T_FLOAT_LOW","LEX_T_FLOAT_HIGH",
"LEX_T_SCIORFLOAT_LOW","LEX_T_SCIORFLOAT_HIGH","LEX_T_SCIHEX_LOW",
"LEX_T_SCIHEX_HIGH","LEX_T_CHAR","LEX_T_STRING","LEX_T_PTR","LEX_T_REPORT",
};
char *yyrule[] = {
"$accept : fmt",
"cmdbegin : LEX_PERCENT",
"cmdbegin : LEX_PERCENT LEX_REFERENCE",
"subcmd : LEX_WORD",
"subcmd : LEX_WORD LEX_COLON subcmdparams",
"subcmdparams : identifier",
"subcmdparams : pair",
"subcmdparams : subcmdparams LEX_COMMA identifier",
"subcmdparams : subcmdparams LEX_COMMA pair",
"identifier : LEX_WORD",
"identifier : LEX_STAR",
"pair : identifier LEX_EQUAL identifier",
"intformat : LEX_T_DEC",
"intformat : LEX_T_OCT",
"intformat : LEX_T_UNSIGNED",
"intformat : LEX_T_HEX_LOW",
"intformat : LEX_T_HEX_HIGH",
"intformat : LEX_T_REPORT",
"doubleformat : LEX_T_SCI_LOW",
"doubleformat : LEX_T_SCI_HIGH",
"doubleformat : LEX_T_FLOAT_LOW",
"doubleformat : LEX_T_FLOAT_HIGH",
"doubleformat : LEX_T_SCIORFLOAT_LOW",
"doubleformat : LEX_T_SCIORFLOAT_HIGH",
"doubleformat : LEX_T_SCIHEX_LOW",
"doubleformat : LEX_T_SCIHEX_HIGH",
"stringformat : LEX_T_CHAR",
"$$1 :",
"stringformat : $$1 LEX_T_STRING",
"ptrformat : LEX_T_PTR",
"extformat : LEX_LEFT_BRACE subcmd LEX_RIGHT_BRACE",
"intmodifier : LEX_MOD_HALFHALF",
"intmodifier : LEX_MOD_HALF",
"intmodifier : LEX_MOD_LONGLONG",
"intmodifier : LEX_MOD_LONG",
"intmodifier : LEX_MOD_INTMAXT",
"intmodifier : LEX_MOD_SIZET",
"intmodifier : LEX_MOD_PTRDIFFT",
"intmodifier :",
"doublemodifier : LEX_MOD_LONGDOUBLE",
"doublemodifier :",
"modintformat : intmodifier intformat",
"moddoubleformat : doublemodifier doubleformat",
"format : modintformat",
"format : moddoubleformat",
"format : stringformat",
"format : ptrformat",
"format : extformat",
"mode : LEX_ZERO",
"mode : LEX_DIEZ",
"mode : LEX_MINUS",
"mode : LEX_SPACE",
"mode : LEX_PLUS",
"width : LEX_NUMBER",
"width : LEX_STAR",
"width : LEX_STAR LEX_REFERENCE",
"precision : LEX_PERIOD",
"precision : LEX_PERIOD LEX_NUMBER",
"precision : LEX_PERIOD LEX_STAR",
"precision : LEX_PERIOD LEX_STAR LEX_REFERENCE",
"modes : mode modes",
"modes : width modes",
"modes : precision modes",
"modes :",
"cmd : cmdbegin modes format",
"cmd : error",
"fmt : cmd",
"fmt : LEX_WORD",
"fmt : LEX_BACKSLASH",
"fmt : fmt cmd",
"fmt : fmt LEX_WORD",
"fmt : fmt LEX_BACKSLASH",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 734 "src/dgd_format_parser.y"

static 
int yylex() {
   format_lexer( &lexer_state, dgd_format_string, &(yylval.lex) );

   if( yylval.lex.token == LEX_EOF ) {
      return -1;
   }
   return yylval.lex.token;
}

void yylexreset() {
   format_lexer_set_state( &lexer_state, LEX_STATE_NORMAL );
}

static
void yyerror( char* reason ) {
}

cache_t *dgd_format_parser_cache() {
   return (cache_t*)&parser_cache;
}

cache_item_t *dgd_format_parse( char* format_string ) {
   cache_item_t *result;

   if( !parser_init ) {
      parser_init = 1;

      dgd_lookup_format( &parser_cache );
      cache = (cache_t*) &parser_cache;
   }


   dgd_format_string = format_string;
   start_ring = NULL;

   init_lexer_state( &lexer_state );	
   yyparse();

   result = dgd_format_settle_args( cache, yyval.ring );
   if( result == NULL ) {
      dgd_cache_free( cache, &(yyval.ring), -1 );
      return NULL;
   }

   result = dgd_cache_new( cache, format_string, result );
   if( result == NULL ) {
      dgd_cache_free( cache, &result, -1 );
      return NULL;
   }

   return result;
}

static
void dgd_ring_push_and_sort( cache_item_t **ring, cache_item_t *item ) {
   cache_item_t *prev;

   if( *ring == NULL ) {
      dgd_ring_push_back( ring, item );
      return;
   }

   prev = (*ring)->prev;   

   do {
      if( prev->value.argload.index <= item->value.argload.index )
	 break;
      prev = prev->prev;
   } while( prev != *ring );
	
   if( prev == *ring && 
       prev->value.argload.index > item->value.argload.index ) {
      dgd_ring_push_front( ring, item );
   } else {
      // this is actually push after
      dgd_ring_forward( prev );
      dgd_ring_push_back( &prev, item );
   }
}

cache_item_t*
dgd_format_settle_args( cache_t *cache, cache_item_t *parse_ring ) {
   cache_item_t *eval_item;
   cache_item_t *parse_prefix = NULL;
   cache_item_t *ring = NULL;
   unsigned argn = 0;

   eval_item = parse_ring;

   do {
      switch( eval_item->type ) {
	 case PARS_T_NEXT_ARG:
	    if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
	       goto error;
	    
	    ring->type = EVAL_T_PTR;
	    ring->value.argload.index = argn++;
	    ring->value.argload.attr.valid_mask = 0;

	    dgd_ring_push_and_sort( &parse_prefix, ring );
	    
	    eval_item->value.next_arg.arg = ring;
	    break;
	 case PARS_T_DEC:
	 case PARS_T_OCT:
	 case PARS_T_UNSIGNED:
	 case PARS_T_HEX:
	 case PARS_T_CHAR:

	 case PARS_T_SCI:
	 case PARS_T_FLOAT:
	 case PARS_T_SCIORFLOAT:
	 case PARS_T_SCIHEX:

	 case PARS_T_STR:
	 case PARS_T_PTR:
	 case PARS_T_REP:
	    if( (eval_item->value.call.attr.valid_mask & CALL_ATTR_WIDTH) &&
		eval_item->value.call.attr.width <= 0 ) {
	       if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
		  goto error;
	       
	       ring->type = EVAL_T_INT;
	       if( eval_item->value.call.attr.width < 0 ) 
		  argn = -eval_item->value.call.attr.width;
	       ring->value.argload.index = argn++;
	       ring->value.argload.attr.valid_mask = 0;

	       dgd_ring_push_and_sort( &parse_prefix, ring );
	       
	       eval_item->value.call.width = ring;	       
	    }

	    if( (eval_item->value.call.attr.valid_mask & 
		 CALL_ATTR_PRECISION) &&
		eval_item->value.call.attr.precision <= 0 ) {
	       if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
		  goto error;
	       
	       ring->type = EVAL_T_INT;
	       if( eval_item->value.call.attr.precision < 0 ) 
		  argn = -eval_item->value.call.attr.precision;
	       ring->value.argload.index = argn++;
	       ring->value.argload.attr.valid_mask = 0;

	       dgd_ring_push_and_sort( &parse_prefix, ring );
	       
	       eval_item->value.call.precision = ring;	       
	    }	       

	    if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) 
	       goto error;
	    
	    switch( eval_item->type ) {
	       	 case PARS_T_SCI:
	       case PARS_T_FLOAT:
	       case PARS_T_SCIORFLOAT:
	       case PARS_T_SCIHEX:
		  ring->type = EVAL_T_DOUBLE;
		  break;
	       case PARS_T_STR:
	       case PARS_T_PTR:
	       case PARS_T_REP:
		  ring->type = EVAL_T_PTR;
		  break;
	       default:
		  ring->type = EVAL_T_INT;
		  break;
	    }

	    if( eval_item->value.call.attr.valid_mask & CALL_ATTR_ABSPOS ) 
	       argn = eval_item->value.call.attr.position;
	    ring->value.argload.index = argn++;
	    dgd_call_attr_assign( &(ring->value.argload.attr),
				  &(eval_item->value.call.attr) );

	    dgd_ring_push_and_sort( &parse_prefix, ring );
	    
	    eval_item->value.call.arg = ring;

	    break;
	 default:
	    break;
      }

      dgd_ring_forward(eval_item);
   } while( eval_item != parse_ring );

  finish:
   dgd_ring_push_front( &parse_ring, parse_prefix );
   return parse_ring;

  error:
   dgd_cache_free( cache, &parse_prefix, -1 );
   return NULL;
}
#line 561 "src/dgd_format_parser.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 116 "src/dgd_format_parser.y"
{
                       yyval.attr.valid_mask = 0;
                     }
break;
case 2:
#line 120 "src/dgd_format_parser.y"
{ 
                       yyval.attr.position = yyvsp[0].lex.value.num;
                       yyval.attr.valid_mask = CALL_ATTR_ABSPOS;
                     }
break;
case 3:
#line 128 "src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type                 = PARS_T_CALL_BY_NAME;
                       ring->value.call.name      = yyvsp[0].lex.value.lexeme;
                       ring->value.call.num_param = 0;

                       yyval.ring = ring;                       
                     }
break;
case 4:
#line 142 "src/dgd_format_parser.y"
{ 
                       cache_item_t *args_ring = yyvsp[0].ring; 
                       cache_item_t *ring      = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type                 = PARS_T_CALL_BY_NAME;
                       ring->value.call.name      = yyvsp[-2].lex.value.lexeme;
                       ring->value.call.num_param = dgd_ring_size( args_ring );

                       dgd_ring_push_front( &args_ring, ring );

                       yyval.ring = args_ring;                                  
                     }
break;
case 5:
#line 160 "src/dgd_format_parser.y"
{ 
                       yyval.ring = yyvsp[0].ring;  
                     }
break;
case 6:
#line 164 "src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[0].ring;  
                     }
break;
case 7:
#line 168 "src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[-2].ring;
                     }
break;
case 8:
#line 172 "src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[-2].ring;
                     }
break;
case 9:
#line 179 "src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_LEXEME;
                       ring->value.lexeme.lexeme = yyvsp[0].lex.value.lexeme;

                       yyval.ring = ring;
                     }
break;
case 10:
#line 192 "src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_NEXT_ARG;

                       yyval.ring = ring;
                     }
break;
case 11:
#line 207 "src/dgd_format_parser.y"
{
                       cache_item_t *key_ring   = yyvsp[-2].ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type         = PARS_T_PAIR;

                       dgd_ring_push_front( &key_ring, ring );

                       yyval.ring = ring;  
                     }
break;
case 12:
#line 223 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type               = PARS_T_DEC;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 13:
#line 236 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_OCT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 14:
#line 249 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_UNSIGNED;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 15:
#line 262 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       yyval.ring = ring;  
                     }
break;
case 16:
#line 275 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;

                       yyval.ring = ring;  
                     }
break;
case 17:
#line 289 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_REP;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 18:
#line 305 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;

                       yyval.ring = ring;  
                     }
break;
case 19:
#line 319 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;

                       yyval.ring = ring;  
                     }
break;
case 20:
#line 333 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;

                       yyval.ring = ring;  
                     }
break;
case 21:
#line 347 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;

                       yyval.ring = ring;  
                     }
break;
case 22:
#line 361 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;

                       yyval.ring = ring;  
                     }
break;
case 23:
#line 375 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;

                       yyval.ring = ring;  
                     }
break;
case 24:
#line 389 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIHEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;

                       yyval.ring = ring;  
                     }
break;
case 25:
#line 403 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIHEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;

                       yyval.ring = ring;  
                     }
break;
case 27:
#line 420 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_CHAR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 28:
#line 433 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_STR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 29:
#line 449 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_PTR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       yyval.ring = ring;  
                     }
break;
case 30:
#line 466 "src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[-1].ring;
                     }
break;
case 31:
#line 473 "src/dgd_format_parser.y"
{
                       yyval.attr.byte_count = sizeof(dgd_char_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 32:
#line 478 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_short_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 33:
#line 483 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_longlong_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 34:
#line 488 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_long_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 35:
#line 493 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_intmax_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 36:
#line 498 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_size_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 37:
#line 503 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(dgd_ptrdiff_t);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 38:
#line 508 "src/dgd_format_parser.y"
{
                       dgd_call_attr_set_default( &(yyval.attr) );
                     }
break;
case 39:
#line 515 "src/dgd_format_parser.y"
{  
                       yyval.attr.byte_count = sizeof(long double);
                       yyval.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
break;
case 40:
#line 520 "src/dgd_format_parser.y"
{
                       dgd_call_attr_set_default( &(yyval.attr) );
                     }
break;
case 41:
#line 527 "src/dgd_format_parser.y"
{
                       dgd_call_attr_assign( &(yyvsp[0].ring->value.call.attr),
                                             &(yyvsp[-1].attr) );
                       yyval.ring = yyvsp[0].ring;
                     }
break;
case 42:
#line 536 "src/dgd_format_parser.y"
{

                       dgd_call_attr_assign( &(yyvsp[0].ring->value.call.attr),
                                             &(yyvsp[-1].attr) );
                       yyval.ring = yyvsp[0].ring;
                     }
break;
case 48:
#line 554 "src/dgd_format_parser.y"
{
                       yyval.attr.zero_pad   = 1;
                       yyval.attr.valid_mask = CALL_ATTR_ZERO_PAD;
                     }
break;
case 49:
#line 559 "src/dgd_format_parser.y"
{
                       yyval.attr.alternate  = 1;
                       yyval.attr.valid_mask = CALL_ATTR_ALTERNATE;
                     }
break;
case 50:
#line 564 "src/dgd_format_parser.y"
{
                       yyval.attr.left_adjust = 1;
                       yyval.attr.valid_mask  = CALL_ATTR_LEFT_ADJUST;
                     }
break;
case 51:
#line 569 "src/dgd_format_parser.y"
{
                       yyval.attr.blank      = 1;
                       yyval.attr.valid_mask = CALL_ATTR_BLANK;
                     }
break;
case 52:
#line 574 "src/dgd_format_parser.y"
{
                       yyval.attr.sign       = 1;
                       yyval.attr.valid_mask = CALL_ATTR_SIGN;
                     }
break;
case 53:
#line 582 "src/dgd_format_parser.y"
{
                       yyval.attr.width      = yyvsp[0].lex.value.num;
                       yyval.attr.valid_mask = CALL_ATTR_WIDTH;
                     }
break;
case 54:
#line 587 "src/dgd_format_parser.y"
{
                       yyval.attr.width      = 0;
                       yyval.attr.valid_mask = CALL_ATTR_WIDTH;
                     }
break;
case 55:
#line 592 "src/dgd_format_parser.y"
{
                       yyval.attr.width      = -(int)yyvsp[0].lex.value.num;
                       yyval.attr.valid_mask = CALL_ATTR_WIDTH;
                     }
break;
case 56:
#line 601 "src/dgd_format_parser.y"
{
                       yyval.attr.valid_mask = 0;
                     }
break;
case 57:
#line 605 "src/dgd_format_parser.y"
{
                       yyval.attr.precision  = yyvsp[0].lex.value.num;
                       yyval.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
break;
case 58:
#line 610 "src/dgd_format_parser.y"
{
                       yyval.attr.precision  = 0;
                       yyval.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
break;
case 59:
#line 615 "src/dgd_format_parser.y"
{
                       yyval.attr.precision  = -(int)yyvsp[0].lex.value.num;
                       yyval.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
break;
case 60:
#line 623 "src/dgd_format_parser.y"
{
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[0].attr) );
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[-1].attr) );
                     }
break;
case 61:
#line 628 "src/dgd_format_parser.y"
{
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[0].attr) );
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[-1].attr) );
                     }
break;
case 62:
#line 633 "src/dgd_format_parser.y"
{
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[0].attr) );
                       dgd_call_attr_assign( &(yyval.attr), &(yyvsp[-1].attr) );
                     }
break;
case 63:
#line 638 "src/dgd_format_parser.y"
{
                       yyval.attr.valid_mask = 0;
                     }
break;
case 64:
#line 645 "src/dgd_format_parser.y"
{

                       dgd_call_attr_assign( &(yyvsp[0].ring->value.call.attr),
                                             &(yyvsp[-1].attr) );
		       dgd_call_attr_assign( &(yyvsp[0].ring->value.call.attr),
                                             &(yyvsp[-2].attr) );
		       yyval.ring = yyvsp[0].ring;
                       start_ring = NULL;
	               yylexreset();
	             }
break;
case 65:
#line 656 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       dgd_cache_free( cache, &start_ring, -1 );

                       if( ring != NULL ) {
                         ring->type               = PARS_T_ERROR;
                         ring->value.error.lexeme = lexer_state.lexeme;
                       }

                       yyval.ring = ring;
                      
                       yylexreset();
                     }
break;
case 66:
#line 675 "src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[0].ring;
                     }
break;
case 67:
#line 679 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type         = PARS_T_WORD;
                       ring->value.word.lexeme = yyvsp[0].lex.value.lexeme;

                       yyval.ring = ring;  
                     }
break;
case 68:
#line 690 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type               = PARS_T_BACKSLASH;
                       ring->value.backslash.ch = yyvsp[0].lex.value.ch;

                       yyval.ring = ring;  
                     }
break;
case 69:
#line 701 "src/dgd_format_parser.y"
{
                       dgd_ring_push_back( &(yyvsp[-1].ring), yyvsp[0].ring );
                       yyval.ring = yyvsp[-1].ring;
                     }
break;
case 70:
#line 706 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &(yyvsp[-1].ring), ring );

                       ring->type              = PARS_T_WORD;
                       ring->value.word.lexeme = yyvsp[0].lex.value.lexeme;

                       yyval.ring = yyvsp[-1].ring;
                     }
break;
case 71:
#line 719 "src/dgd_format_parser.y"
{
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &(yyvsp[-1].ring), ring );

                       ring->type               = PARS_T_BACKSLASH;
                       ring->value.backslash.ch = yyvsp[0].lex.value.ch;

                       yyval.ring = yyvsp[-1].ring;
                     }
break;
#line 1380 "src/dgd_format_parser.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
