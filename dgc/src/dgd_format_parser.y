%{ 

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
static int yyparse();
static void yylexreset();
static void yyerror( char* reason );

static
cache_item_t* dgd_format_settle_args( cache_t *cache, 
				      cache_item_t *parse_ring,
				      cache_item_t *error_item );

static lexer_state_t   lexer_state;
static int             parser_init = 0;
static default_cache_t parser_cache;
static char*           dgd_format_string = NULL;
static cache_t        *cache = NULL;
static cache_item_t   *start_ring;

%}


%token LEX_EOF               
%token LEX_ZERO              
%token LEX_MINUS             
%token LEX_SPACE             
%token LEX_PLUS              
%token LEX_PERIOD            
%token LEX_NUMBER            
%token LEX_REFERENCE
%token LEX_PERCENT           
%token LEX_LEFTBRACE         
%token LEX_WORD              
%token LEX_COLON             
%token LEX_EQUAL             
%token LEX_COMMA             
%token LEX_STAR              
%token LEX_DOLLAR            
%token LEX_DIEZ              
%token LEX_BACKSLASH         
%token LEX_LEFT_BRACE        
%token LEX_RIGHT_BRACE       

%token LEX_MOD_HALFHALF      
%token LEX_MOD_HALF          
%token LEX_MOD_LONGLONG      
%token LEX_MOD_LONG          
%token LEX_MOD_INTMAXT       
%token LEX_MOD_SIZET         
%token LEX_MOD_PTRDIFFT      
%token LEX_MOD_LONGDOUBLE    

%token LEX_T_DEC             
%token LEX_T_OCT             
%token LEX_T_UNSIGNED        
%token LEX_T_HEX_LOW         
%token LEX_T_HEX_HIGH        
%token LEX_T_SCI_LOW         
%token LEX_T_SCI_HIGH        
%token LEX_T_FLOAT_LOW       
%token LEX_T_FLOAT_HIGH      
%token LEX_T_SCIORFLOAT_LOW  
%token LEX_T_SCIORFLOAT_HIGH 
%token LEX_T_SCIHEX_LOW      
%token LEX_T_SCIHEX_HIGH     
%token LEX_T_CHAR            
%token LEX_T_STRING          
%token LEX_T_PTR             
%token LEX_T_REPORT          

%start fmt


%%


cmdbegin: 
          LEX_PERCENT 
                     {
                       $$.attr.valid_mask = 0;
                     }
          | LEX_PERCENT LEX_REFERENCE 
                     { 
                       $$.attr.position = $2.lex.value.num;
                       $$.attr.valid_mask = CALL_ATTR_ABSPOS;
                     }
;

subcmd:
          LEX_WORD 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type                 = PARS_T_CALL_BY_NAME;
                       ring->value.call.name      = $1.lex.value.lexeme;
                       ring->value.call.num_param = 0;
		       ring->value.call.index     = -1;

                       $$.ring = ring;                       
                     }
          | LEX_WORD LEX_COLON subcmdparams 
                     { 
                       cache_item_t *args_ring = $3.ring; 
                       cache_item_t *ring      = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type                 = PARS_T_CALL_BY_NAME;
                       ring->value.call.name      = $1.lex.value.lexeme;
                       ring->value.call.num_param = -1;
		       ring->value.call.index     = -1;

                       dgd_ring_push_front( &args_ring, ring );

                       $$.ring = args_ring;                                  
                     }
;

subcmdparams:
	  identifier 
                     { 
                       $$.ring = $1.ring;  
                     }
          | pair     
                     {
                       $$.ring = $1.ring;  
                     }
          | subcmdparams LEX_COMMA identifier
                     {
                       $$.ring = $1.ring;
                     }
          | subcmdparams LEX_COMMA pair
                     {
                       $$.ring = $1.ring;
                     }
;

identifier:
	  LEX_WORD 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_LEXEME;
                       ring->value.lexeme.lexeme = $1.lex.value.lexeme;

                       $$.ring = ring;
                     }
          | LEX_STAR 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_NEXT_ARG;

                       $$.ring = ring;
                     }           
;

pair:
	  identifier LEX_EQUAL identifier
                     {
                       cache_item_t *key_ring   = $1.ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type         = PARS_T_PAIR;

                       dgd_ring_push_front( &key_ring, ring );

                       $$.ring = ring;  
                     }
;

intformat:
          LEX_T_DEC 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type               = PARS_T_DEC;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_OCT 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_OCT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_UNSIGNED 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_UNSIGNED;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_HEX_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_HEX_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

		       ring->value.call.attr.valid_mask |= CALL_ATTR_CAPITAL;
                       ring->value.call.attr.capital = 1;

                       $$.ring = ring;  
                     }
          | LEX_T_REPORT
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_REP;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
;

doubleformat:
          LEX_T_SCI_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
	  | LEX_T_SCI_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

		       ring->value.call.attr.valid_mask |= CALL_ATTR_CAPITAL;
                       ring->value.call.attr.capital = 1;

                       $$.ring = ring;  
                     }
	  | LEX_T_FLOAT_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
	  | LEX_T_FLOAT_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

		       ring->value.call.attr.valid_mask |= CALL_ATTR_CAPITAL;
                       ring->value.call.attr.capital = 1;

                       $$.ring = ring;  
                     }
	  | LEX_T_SCIORFLOAT_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
	  | LEX_T_SCIORFLOAT_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

		       ring->value.call.attr.valid_mask |= CALL_ATTR_CAPITAL;
                       ring->value.call.attr.capital = 1;

                       $$.ring = ring;  
                     }
	  | LEX_T_SCIHEX_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIHEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
	  | LEX_T_SCIHEX_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_SCIHEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

		       ring->value.call.attr.valid_mask |= CALL_ATTR_CAPITAL;
                       ring->value.call.attr.capital = 1;

                       $$.ring = ring;  
                     }
;

stringformat:
	  LEX_T_CHAR 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_CHAR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
	  | LEX_T_STRING
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_STR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
;

ptrformat:
	  LEX_T_PTR
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &start_ring, ring );

                       ring->type         = PARS_T_PTR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }

;

extformat:
	  LEX_LEFT_BRACE subcmd LEX_RIGHT_BRACE
                     {
                       $$.ring = $2.ring;
                     }
;

intmodifier:
	  LEX_MOD_HALFHALF 
                     {
                       $$.attr.byte_count = sizeof(dgd_char_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_HALF 
                     {  
                       $$.attr.byte_count = sizeof(dgd_short_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_LONGLONG 
                     {  
                       $$.attr.byte_count = sizeof(dgd_longlong_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_LONG 
                     {  
                       $$.attr.byte_count = sizeof(dgd_long_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_INTMAXT 
                     {  
                       $$.attr.byte_count = sizeof(dgd_intmax_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_SIZET 
                     {  
                       $$.attr.byte_count = sizeof(dgd_size_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
	  | LEX_MOD_PTRDIFFT 
                     {  
                       $$.attr.byte_count = sizeof(dgd_ptrdiff_t);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
          | 
                     {
                       dgd_call_attr_set_default( &($$.attr) );
                     }
;

doublemodifier:
	  LEX_MOD_LONGDOUBLE 
                     {  
                       $$.attr.byte_count = sizeof(long double);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
          |
                     {
                       dgd_call_attr_set_default( &($$.attr) );
                     }
;

modintformat:
	  intmodifier intformat
                     {
                       dgd_call_attr_assign( &($2.ring->value.call.attr),
                                             &($1.attr) );
                       $$.ring = $2.ring;
                     }
;

moddoubleformat:
	  doublemodifier doubleformat
                     {

                       dgd_call_attr_assign( &($2.ring->value.call.attr),
                                             &($1.attr) );
                       $$.ring = $2.ring;
                     }
;

format:
	  modintformat |
	  moddoubleformat |
	  stringformat |
	  ptrformat |
	  extformat
;

mode:
	  LEX_ZERO 
                     {
                       $$.attr.zero_pad   = 1;
                       $$.attr.valid_mask = CALL_ATTR_ZERO_PAD;
                     }
	  | LEX_DIEZ 
                     {
                       $$.attr.alternate  = 1;
                       $$.attr.valid_mask = CALL_ATTR_ALTERNATE;
                     }
	  | LEX_MINUS 
                     {
                       $$.attr.left_adjust = 1;
                       $$.attr.valid_mask  = CALL_ATTR_LEFT_ADJUST;
                     }
	  | LEX_SPACE
                     {
                       $$.attr.blank      = 1;
                       $$.attr.valid_mask = CALL_ATTR_BLANK;
                     }
	  | LEX_PLUS
                     {
                       $$.attr.sign       = 1;
                       $$.attr.valid_mask = CALL_ATTR_SIGN;
                     }
;

width:
	  LEX_NUMBER 
                     {
                       $$.attr.width      = $1.lex.value.num;
                       $$.attr.valid_mask = CALL_ATTR_WIDTH;
                     }
	  | LEX_STAR 
                     {
                       $$.attr.width      = 0;
                       $$.attr.valid_mask = CALL_ATTR_WIDTH;
                     }
	  | LEX_STAR LEX_REFERENCE
                     {
                       $$.attr.width      = -(int)$2.lex.value.num;
                       $$.attr.valid_mask = CALL_ATTR_WIDTH;
                     }

;

precision: 
          LEX_PERIOD 
                     {
                       $$.attr.valid_mask = 0;
                     }
	  | LEX_PERIOD LEX_NUMBER 
                     {
                       $$.attr.precision  = $2.lex.value.num;
                       $$.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
	  | LEX_PERIOD LEX_STAR 
                     {
                       $$.attr.precision  = 0;
                       $$.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
	  | LEX_PERIOD LEX_STAR LEX_REFERENCE
                     {
                       $$.attr.precision  = -(int)$3.lex.value.num;
                       $$.attr.valid_mask = CALL_ATTR_PRECISION;
                     }
;

modes:
	  mode modes 
                     {
                       dgd_call_attr_assign( &($$.attr), &($2.attr) );
                       dgd_call_attr_assign( &($$.attr), &($1.attr) );
                     }
	  | width modes 
                     {
                       dgd_call_attr_assign( &($$.attr), &($2.attr) );
                       dgd_call_attr_assign( &($$.attr), &($1.attr) );
                     }
	  | precision modes 
                     {
                       dgd_call_attr_assign( &($$.attr), &($2.attr) );
                       dgd_call_attr_assign( &($$.attr), &($1.attr) );
                     }
          |
                     {
                       $$.attr.valid_mask = 0;
                     }
;

cmd:
	  cmdbegin modes format 
                     {

                       dgd_call_attr_assign( &($3.ring->value.call.attr),
                                             &($2.attr) );
		       dgd_call_attr_assign( &($3.ring->value.call.attr),
                                             &($1.attr) );
		       $$.ring = $3.ring;
                       start_ring = NULL;
	               yylexreset();
	             }
          | error 
                     {
                       cache_item_t *ring = NULL;

		       dgd_cache_free( cache, &start_ring, -1 );

		       ring = dgd_cache_alloc( cache, 1 );
                       if( ring != NULL ) {
                         ring->type               = PARS_T_ERROR;
			 ring->value.error.error  = PARS_ERR_SYNTAX;
                         ring->value.error.lexeme = lexer_state.lexeme;
                       }

                       $$.ring = ring;
                      
                       yylexreset();
                     }
                  
;

fmt:
          cmd        
                     {
                       $$.ring = $1.ring;
                     }
	  | LEX_WORD
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type         = PARS_T_WORD;
                       ring->value.word.lexeme = $1.lex.value.lexeme;

                       $$.ring = ring;  
                     }
	  | LEX_BACKSLASH        
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       ring->type               = PARS_T_BACKSLASH;
                       ring->value.backslash.ch = $1.lex.value.ch;

                       $$.ring = ring;  
                     }
	  | fmt cmd 
                     {
                       dgd_ring_push_back( &($1.ring), $2.ring );
                       $$.ring = $1.ring;
                     }
	  | fmt LEX_WORD
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &($1.ring), ring );

                       ring->type              = PARS_T_WORD;
                       ring->value.word.lexeme = $2.lex.value.lexeme;

                       $$.ring = $1.ring;
                     }
	  | fmt LEX_BACKSLASH
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) YYERROR;

                       dgd_ring_push_back( &($1.ring), ring );

                       ring->type               = PARS_T_BACKSLASH;
                       ring->value.backslash.ch = $2.lex.value.ch;

                       $$.ring = $1.ring;
                     }
;

%%

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

cache_item_t *dgd_format_parse( char* format_string,
				unsigned int flags ) {
   cache_item_t *result, *chain, err_item;
   
   if( !parser_init ) {
      parser_init = 1;

      dgd_lookup_format( &parser_cache );
      cache = (cache_t*) &parser_cache;
   }


   dgd_format_string = format_string;
   start_ring = NULL;

   init_lexer_state( &lexer_state );	
   yyparse();

   result = dgd_format_settle_args( cache, yyval.ring, &err_item );
   if( result == NULL ) {
      cache_item_t *ring = NULL;

      ring = dgd_cache_alloc( cache, 1 );
      if( ring != NULL ) {
	 ring->type        = PARS_T_ERROR;
	 ring->value.error = err_item.value.error;

	 dgd_ring_push_back( &ring, yyval.ring );
      } else {
	 dgd_cache_free( cache, &(yyval.ring), -1 );
	 // try again
	 ring = dgd_cache_alloc( cache, 1 );
	 if( ring != NULL ) {
	    ring->type              = PARS_T_ERROR;
	    ring->value.error.error = PARS_ERR_ALLOC;
	 }
      }
      result = ring;
   } 

   if( flags & PARS_FLAG_NOCHAIN )
      return result;

   chain = dgd_cache_new( cache, format_string, result );
   if( chain == NULL ) {
      cache_item_t *ring = NULL;

      dgd_cache_free( cache, &result, -1 );

      ring = dgd_cache_alloc( cache, 1 );
      if( ring != NULL ) {
	 ring->type              = PARS_T_ERROR;
	 ring->value.error.error = PARS_ERR_ALLOC;
      }
      chain = dgd_cache_new( cache, format_string, ring );
   }

   return chain;
}

#define FIND_OK   0
#define FIND_NOPE 1

static 
unsigned int dgd_ring_find( cache_item_t  *ring, 
			    cache_item_t **item, 
			    unsigned int   index ) {
   cache_item_t *next;

   if( ring == NULL ) {
      *item = NULL;
      return FIND_NOPE;
   }

   next = ring;

   do {
      if( next->value.argload.index >= index )
	 break;
      dgd_ring_forward( next );
   } while( next != ring );

   *item = next;
   if( next->value.argload.index == index ) {
      return FIND_OK;
   }
   return FIND_NOPE;
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

static
cache_item_t*
dgd_format_settle_args( cache_t *cache, 
			cache_item_t *parse_ring,
			cache_item_t *error_item ) {
   cache_item_t *eval_item;
   cache_item_t *parse_prefix = NULL;
   cache_item_t *ring = NULL;
   unsigned argn = 1, maxargn = 1, i, type;
   
   eval_item = parse_ring;

   do {
      switch( eval_item->type ) {
	 case PARS_T_NEXT_ARG:
	    switch( dgd_ring_find( parse_prefix, &ring, argn ) ) {
	       case FIND_NOPE:
		  if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) {
		     if( error_item != NULL ) 
			error_item->value.error.error = PARS_ERR_ALLOC;
		     goto error;
		  }
		  
		  ring->type = EVAL_T_PTR;
		  ring->value.argload.index = argn;
		  dgd_ring_push_and_sort( &parse_prefix, ring );
		  /* fall through */
	       case FIND_OK:
		  if( ring->type != EVAL_T_PTR &&
		      ring->type != EVAL_T_INT ) {
		     if( error_item != NULL ) {
			error_item->value.error.error = PARS_ERR_ARGTYPE;
			error_item->value.error.num   = argn;
		     }
		     goto error;
		  }
		  argn++; /* dont even think to insert ++ into the macro! */
		  maxargn = max( maxargn, argn );
		  break;
	    }
	    
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

	       if( eval_item->value.call.attr.width < 0 ) 
		  argn = -eval_item->value.call.attr.width;

	       switch( dgd_ring_find( parse_prefix, &ring, argn ) ) {
		  case FIND_NOPE:
		     if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) {
			if( error_item != NULL ) 
			   error_item->value.error.error = PARS_ERR_ALLOC;
			goto error;
		     }
	       
		     ring->type = EVAL_T_INT;		     
		     ring->value.argload.index = argn;
		     dgd_ring_push_and_sort( &parse_prefix, ring );
		     /* fall through */
		  case FIND_OK:
		     if( ring->type != EVAL_T_INT  &&
			 ring->type != EVAL_T_PTR) {
			if( error_item != NULL ) {
			   error_item->value.error.error = PARS_ERR_ARGTYPE;
			   error_item->value.error.num   = argn;
			}
			goto error;
		     }
		     argn++; /* dont even think to insert ++ into the macro! */
		     maxargn = max( maxargn, argn );
		     break;
	       }
	       eval_item->value.call.width = ring;	       
	    }

	    if( (eval_item->value.call.attr.valid_mask & 
		 CALL_ATTR_PRECISION) &&
		eval_item->value.call.attr.precision <= 0 ) {

	       if( eval_item->value.call.attr.precision < 0 ) 
		  argn = -eval_item->value.call.attr.precision;

	       switch( dgd_ring_find( parse_prefix, &ring, argn ) ) {
		  case FIND_NOPE:
		     if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) {
			if( error_item != NULL ) 
			   error_item->value.error.error = PARS_ERR_ALLOC;
			goto error;
		     }
		     
		     ring->type = EVAL_T_INT;
		     ring->value.argload.index = argn;
		     dgd_ring_push_and_sort( &parse_prefix, ring );
		     /* fall through */
		  case FIND_OK:
		     if( ring->type != EVAL_T_INT &&
			 ring->type != EVAL_T_PTR ) {
			if( error_item != NULL ) {
			   error_item->value.error.error = PARS_ERR_ARGTYPE;
			   error_item->value.error.num   = argn;
			}
			goto error;
		     }
		     argn++; /* dont even think to insert ++ into the macro! */
		     maxargn = max( maxargn, argn );
		     break;
	       }	       
	       eval_item->value.call.precision = ring;	       
	    }	       

	    if( eval_item->value.call.attr.valid_mask & CALL_ATTR_ABSPOS ) 
	       argn = eval_item->value.call.attr.position;

	    switch( eval_item->type ) {
	       case PARS_T_SCI:
	       case PARS_T_FLOAT:
	       case PARS_T_SCIORFLOAT:
	       case PARS_T_SCIHEX:
		  type = EVAL_T_DOUBLE;
		  break;
	       case PARS_T_STR:
	       case PARS_T_PTR:
	       case PARS_T_REP:
		  type = EVAL_T_PTR;
		  break;
	       default:
		  type = EVAL_T_INT;
		  break;
	    }

	    switch( dgd_ring_find( parse_prefix, &ring, argn ) ) {
	       case FIND_NOPE:
		  if( (ring = dgd_cache_alloc( cache, 1 )) == NULL ) {
		     if( error_item != NULL ) 
			error_item->value.error.error = PARS_ERR_ALLOC;
		     goto error;
		  }
	    
		  ring->type                = type;
		  ring->value.argload.index = argn;
		  dgd_ring_push_and_sort( &parse_prefix, ring );
		  /* fall through */
	       case FIND_OK:
		  if( ring->type != type && 
		      ( ring->type == EVAL_T_INT && type != EVAL_T_PTR ) &&
		      ( ring->type == EVAL_T_PTR && type != EVAL_T_INT ) ) {
		     if( error_item != NULL ) {
			error_item->value.error.error = PARS_ERR_ARGTYPE;
			error_item->value.error.num   = argn;
		     }
		     goto error;
		  }
		  argn++; /* dont even think to insert ++ into the macro! */
		  maxargn = max( maxargn, argn );
		  break;
	    }	       
	    
	    eval_item->value.call.arg = ring;

	    break;
	 default:
	    break;
      }

      dgd_ring_forward(eval_item);
   } while( eval_item != parse_ring );

   eval_item = parse_prefix;
   for( i = 1; i < maxargn; i++ ) {
      if( eval_item->value.argload.index != i ) {
	 if( error_item != NULL ) {
	    error_item->value.error.error = PARS_ERR_ARGGAP;
	    error_item->value.error.num   = i;
	 }
	 goto error;
      }

      dgd_ring_forward(eval_item);
   }

   dgd_ring_push_front( &parse_ring, parse_prefix );
   return parse_ring;

  error:
   dgd_cache_free( cache, &parse_prefix, -1 );
   eval_item = parse_ring;

   do {
      switch( eval_item->type ) {
	 case PARS_T_NEXT_ARG:
	    eval_item->value.next_arg.arg = NULL;
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
	    eval_item->value.call.width     = NULL;
	    eval_item->value.call.precision = NULL;
	    eval_item->value.call.arg       = NULL;
	    break;
	 default:
	    break;
      }

      dgd_ring_forward(eval_item);
   } while( eval_item != parse_ring );

   return NULL;
}
