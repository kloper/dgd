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
#include "dgd_compile_cache.h"
   
char* dgd_format_string = NULL;
cache_t *cache = NULL;

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
                       $$.ring = NULL;
                     }
          | LEX_PERCENT LEX_REFERENCE 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type            = PARS_T_SET_ARG;
                       ring->value.num       = $2.lex.value.num;

                       $$.ring = ring;                       
                     }
;

subcmd:
          LEX_WORD 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type            = PARS_T_CALL_BY_NAME;
                       ring->value.call.name = $1.lex.value.lexeme;
                       ring->value.call.args = NULL;

                       $$.ring = ring;                       
                     }
        | LEX_WORD LEX_COLON subcmdparams 
                     { 
                       cache_item_t *args_ring = $3.ring; 
                       cache_item_t *ring      = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type            = PARS_T_CALL_BY_NAME;
                       ring->value.call.name = $1.lex.value.lexeme;
                       ring->value.call.args = args_ring;

                       $$.ring = ring;                                  
                     }
;

subcmdparams:
	  identifier 
                     { 
                       cache_item_t *key_ring   = $1.ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->value.ring = key_ring;

                       $$.ring = ring;  
                     }
          | pair     {
                       $$.ring = $1.ring;  
                     }
          | identifier LEX_COMMA subcmdparams 
                     {
                       dgd_ring_push_back( &($1.ring), $3.ring );
                     }

          | pair LEX_COMMA subcmdparams 
                     {
                       dgd_ring_push_back( &($1.ring), $3.ring );
                     }
;

identifier:
	  LEX_WORD 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_LEXEME;
                       ring->value.lexeme = $1.lex.value.lexeme;
                       $$.ring = ring;
                     }
          | LEX_STAR 
                     { 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_NEXT_ARG;
                       $$.ring = ring;
                     }
;

pair:
	  identifier LEX_EQUAL identifier
                     {
                       cache_item_t *key_ring   = $1.ring;
                       cache_item_t *value_ring = $3.ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_PARAM;
                       dgd_ring_push_back( &key_ring, value_ring );
                       ring->value.ring = key_ring;

                       $$.ring = ring;  
                     }
;

intformat:
          LEX_T_DEC 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_DEC;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_OCT 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_OCT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_UNSIGNED 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_UNSIGNED;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );

                       $$.ring = ring;  
                     }
          | LEX_T_HEX_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       $$.ring = ring;  
                     }
          | LEX_T_HEX_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_HEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;
                       $$.ring = ring;  
                     }
          | LEX_T_REPORT
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_WRITE_REP;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       $$.ring = ring;  
                     }
;

doubleformat:
          LEX_T_SCI_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       $$.ring = ring;  
                     }
	  | LEX_T_SCI_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;
                       $$.ring = ring;  
                     }
	  | LEX_T_FLOAT_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       $$.ring = ring;  
                     }
	  | LEX_T_FLOAT_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_FLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;
                       $$.ring = ring;  
                     }
	  | LEX_T_SCIORFLOAT_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       $$.ring = ring;  
                     }
	  | LEX_T_SCIORFLOAT_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCIORFLOAT;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;
                       $$.ring = ring;  
                     }
	  | LEX_T_SCIHEX_LOW 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCIHEX;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 0;
                       $$.ring = ring;  
                     }
	  | LEX_T_SCIHEX_HIGH 
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_SCI;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       ring->value.call.attr.capital = 1;
                       $$.ring = ring;  
                     }
;

stringformat:
	  LEX_T_CHAR |
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_CHAR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       $$.ring = ring;  
                     }
	  LEX_T_STRING
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_STR;
                       dgd_call_attr_set_default( &(ring->value.call.attr) );
                       $$.ring = ring;  
                     }
;

ptrformat:
	  LEX_T_PTR
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_READ_PTR;
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
;

doublemodifier:
	  LEX_MOD_LONGDOUBLE 
                     {  
                       $$.attr.byte_count = sizeof(long double);
                       $$.attr.valid_mask = CALL_ATTR_BYTECOUNT;
                     }
          |

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
                       dgd_ring_push_back( &($1.ring), $3.ring );
                       $$.ring = $1.ring;

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

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_WORD;
                       ring->value.lexeme = $1.lex.value.lexeme;
                       $$.ring = ring;  
                     }
	  | LEX_BACKSLASH        
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_CHAR;
                       ring->value.ch     = $1.lex.value.ch;
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

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type             = PARS_T_WORD;
                       ring->value.lexeme     = $1.lex.value.lexeme;

                       dgd_ring_push_back( &($1.ring), ring );
                       $$.ring = $1.ring;
                     }
	  | fmt LEX_BACKSLASH
                     {
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror("Unknown");
                       }

                       ring->type         = PARS_T_CHAR;
                       ring->value.ch     = $1.lex.value.ch;

                       dgd_ring_push_back( &($1.ring), ring );
                       $$.ring = $1.ring;
                     }
;

%%

static lexer_state_t   lexer_state;
static int             lexer_state_init = 0;
static default_cache_t parser_cache;

static 
int yylex() {
   if( !lexer_state_init ) {
      init_lexer_state( &lexer_state );
      lexer_state_init = 1;

      dgd_lookup_format( &parser_cache );
      cache = (cache_t*) &parser_cache;
   }

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
   fprintf( stderr, "%s\n", reason );
}
