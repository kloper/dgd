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
 * dgd_format_lexer.c -- implementation of printf format string lexer
 *
 */

#include <stdlib.h>
#include <string.h>

#include "dgd_config.h"
#include "dgd_format_lexer.h"

void init_lexer_state( lexer_state_t *lex_state ) {
   lex_state->lexeme.begin = lex_state->lexeme.end = NULL;
   lex_state->state = LEX_STATE_DONE;
}

void format_lexer_set_state( lexer_state_t *lex_state,
			     unsigned char state ) {
   lex_state->state = state;
}

void format_lexer( lexer_state_t *lex_state, 
		   char          *format, 
		   lexer_value_t *token  ) {

   if( lex_state->state == LEX_STATE_DONE ) {
      lex_state->state        = LEX_STATE_NORMAL;
      lex_state->lexeme.begin = format;
      lex_state->lexeme.end   = format;
   }

   token->token = LEX_EOF;
   memset( (char*)&(token->value), '\0', sizeof(token->value) );

   while( 1 ) {
      switch( lex_state->state ) {
	 case LEX_STATE_NORMAL: {
	    switch( *lex_state->lexeme.end ) {
	       case '\\': {
		  bool_t token_ready = FALSE;

		  if( lex_state->lexeme.begin < lex_state->lexeme.end ) {
		     token->token = LEX_WORD;
		     token->value.lexeme.begin = lex_state->lexeme.begin;
		     token->value.lexeme.end   = lex_state->lexeme.end;
		     token_ready = TRUE;
		  }
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_BACKSLASH;

		  if( token_ready ) 
		     return;
	       } break;
	       case '%': {
		  bool_t token_ready = FALSE;

		  if( lex_state->lexeme.begin < lex_state->lexeme.end ) {
		     token->token = LEX_WORD;
		     token->value.lexeme.begin = lex_state->lexeme.begin;
		     token->value.lexeme.end   = lex_state->lexeme.end;
		     token_ready = TRUE;
		  }
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_COMMAND;

		  if( token_ready ) 
		     return;
	       } break;
	       case '\0': {
		  if( lex_state->lexeme.begin < lex_state->lexeme.end ) {
		     token->token = LEX_WORD;
		     token->value.lexeme.begin = lex_state->lexeme.begin;
		     token->value.lexeme.end   = lex_state->lexeme.end;
		  }
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_EOF;
		  return;
	       } break;
	       default: {
		  lex_state->lexeme.end++;
	       } break;
	    }
	 } break;
	 case LEX_STATE_EOF: {
	    token->token = LEX_EOF;
	    lex_state->state = LEX_STATE_DONE;
	    return;
	 } break;
	 case LEX_STATE_BACKSLASH: {
	    switch( *lex_state->lexeme.end ) {
	       case 'a':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\a';
		  return;
	       case 'b':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\b';
		  return;
	       case 'f':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\f';
		  return;
	       case 'n':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\n';
		  return;
	       case 'r':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\r';
		  return;
	       case 't':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\t';
		  return;
	       case 'v':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;

		  token->token = LEX_BACKSLASH;
		  token->value.ch = '\v';
		  return;
	       case '0':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_OCT;
		  break;
	       case 'x':
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_HEX;
		  break;
	       default:
		  token->token = LEX_BACKSLASH;
		  token->value.ch = *lex_state->lexeme.begin;

		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  lex_state->state = LEX_STATE_NORMAL;
		  return;
	    }
	 } break;
	 case LEX_STATE_OCT: {
	    if( *lex_state->lexeme.end >= '0' &&
		*lex_state->lexeme.end <= '7' &&
		lex_state->lexeme.end < lex_state->lexeme.begin + 3 ) {
	       lex_state->lexeme.end++;
	       break;
	    } else {
	       token->token = LEX_BACKSLASH;
	       token->value.ch = oct2char( &(lex_state->lexeme) );
	       
	       lex_state->state        = LEX_STATE_NORMAL;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    }
	 } break;
	 case LEX_STATE_HEX: {
	    if( ( ( *lex_state->lexeme.end >= '0' &&
		    *lex_state->lexeme.end <= '9' ) ||
		  ( ( *lex_state->lexeme.end >= 'a' &&
		      *lex_state->lexeme.end <= 'f' ) ||
		    ( *lex_state->lexeme.end >= 'A' &&
		      *lex_state->lexeme.end <= 'F' ) ) ) &&
		lex_state->lexeme.end < lex_state->lexeme.begin + 2 ) {
	       lex_state->lexeme.end++;
	       break;
	    } else {
	       token->token = LEX_BACKSLASH;
	       token->value.ch = hex2char( &(lex_state->lexeme) );
	       
	       lex_state->state        = LEX_STATE_NORMAL;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    }
	 } break;
	 case LEX_STATE_COMMAND: {
	    switch( *lex_state->lexeme.end ) {
	       case '0':
		  token->token = LEX_ZERO;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '-':
		  token->token = LEX_MINUS;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case ' ':
		  token->token = LEX_SPACE;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '+':
		  token->token = LEX_PLUS;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '.':
		  token->token = LEX_PERIOD;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '%':
		  lex_state->state = LEX_STATE_COMMAND_PRE;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  break;
	       case '*':
		  token->token = LEX_STAR;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '$':
		  token->token = LEX_DOLLAR;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '#':
		  token->token = LEX_DIEZ;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '{':
		  lex_state->state = LEX_STATE_CMDEXT;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  break;
	       case '1':
	       case '2':
	       case '3':
	       case '4':
	       case '5':
	       case '6':
	       case '7':
	       case '8':
	       case '9':
		  lex_state->state = LEX_STATE_CMD_NUMBER;
		  lex_state->lexeme.end++;
		  break;
	       case 'h': 
		  lex_state->state = LEX_STATE_CMD_HALF;
		  lex_state->lexeme.end++;
		  break;
	       case 'l': 
		  lex_state->state = LEX_STATE_CMD_LONG;
		  lex_state->lexeme.end++;
		  break;
	       case 'L':
		  token->token = LEX_MOD_LONGDOUBLE;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;		  
	       case 'q':
		  token->token = LEX_MOD_LONGLONG;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'j':
		  token->token = LEX_MOD_INTMAXT;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'z':
		  token->token = LEX_MOD_SIZET;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 't':
		  token->token = LEX_MOD_PTRDIFFT;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'd': case 'i':
		  token->token = LEX_T_DEC;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'o': 
		  token->token = LEX_T_OCT;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'u': 
		  token->token = LEX_T_UNSIGNED;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'x': 
		  token->token = LEX_T_HEX_LOW;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'X':
		  token->token = LEX_T_HEX_HIGH;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'e': 
		  token->token = LEX_T_SCI_LOW;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'f': 
		  token->token = LEX_T_FLOAT_LOW;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'E': 
		  token->token = LEX_T_SCI_HIGH;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'F':
		  token->token = LEX_T_FLOAT_HIGH;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'g': 
		  token->token = LEX_T_SCIORFLOAT_LOW;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'G': 
		  token->token = LEX_T_SCIORFLOAT_HIGH;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'a': 
		  token->token = LEX_T_SCIHEX_LOW;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'A':
		  token->token = LEX_T_SCIHEX_HIGH;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'c': 
		  token->token = LEX_T_CHAR;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 's':
		  token->token = LEX_T_STRING;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'p': 
		  token->token = LEX_T_PTR;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case 'n':
		  token->token = LEX_T_REPORT;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       default:
		  lex_state->state = LEX_STATE_NORMAL;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  break;
	    }
	 } break;
	 case LEX_STATE_COMMAND_PRE: {
	    if( *lex_state->lexeme.end != '%' ) {
	       token->token = LEX_PERCENT;

	       lex_state->state = LEX_STATE_COMMAND;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	    } else {
	       token->token = LEX_BACKSLASH;		  
	       token->value.ch = '%';
	       
	       lex_state->state = LEX_STATE_NORMAL;
	       lex_state->lexeme.end++;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	    }
	    return;

	 } break;
	 case LEX_STATE_CMD_NUMBER: {
	    if( *lex_state->lexeme.end >= '0' &&
		*lex_state->lexeme.end <= '9' ) {
	       lex_state->lexeme.end++;
	    } else {
	       if( *lex_state->lexeme.end != '$' ) {
		  token->token = LEX_NUMBER;
		  token->value.num = dec2int( &(lex_state->lexeme) );
		  
		  lex_state->state = LEX_STATE_COMMAND;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
	       } else {
		  token->token = LEX_REFERENCE;
		  token->value.num = dec2int( &(lex_state->lexeme) );

		  lex_state->state = LEX_STATE_COMMAND;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
	       }
	       return;
	    }
	 } break;
	 case LEX_STATE_CMD_HALF: {
	    if( *lex_state->lexeme.end == 'h' ) {
	       token->token = LEX_MOD_HALFHALF;

	       lex_state->state = LEX_STATE_COMMAND;
	       lex_state->lexeme.end++;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    } else {
	       token->token = LEX_MOD_HALF;

	       lex_state->state = LEX_STATE_COMMAND;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    }
	 } break;
	 case LEX_STATE_CMD_LONG: {
	    if( *lex_state->lexeme.end == 'l' ) {
	       token->token = LEX_MOD_LONGLONG;

	       lex_state->state = LEX_STATE_COMMAND;
	       lex_state->lexeme.end++;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    } else {
	       token->token = LEX_MOD_LONG;

	       lex_state->state = LEX_STATE_COMMAND;
	       lex_state->lexeme.begin = lex_state->lexeme.end;
	       return;
	    }
	 } break;
	 case LEX_STATE_CMDEXT: {
	    switch( *lex_state->lexeme.end ) {
	       case '{':
		  token->token = LEX_LEFT_BRACE;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '*':
		  token->token = LEX_STAR;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case ':':
		  token->token = LEX_COLON;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '=':
		  token->token = LEX_EQUAL;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case ',':
		  token->token = LEX_COMMA;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '}':
		  token->token = LEX_RIGHT_BRACE;

		  lex_state->state = LEX_STATE_COMMAND;
		  lex_state->lexeme.end++;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  return;
	       case '\0':
		  lex_state->state = LEX_STATE_COMMAND;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  break;
	       default:
		  lex_state->state = LEX_STATE_CMDEXT_WORD;
		  lex_state->lexeme.begin = lex_state->lexeme.end;
		  break;
	    }
	 } break;

      case LEX_STATE_CMDEXT_WORD: {
	 if( *lex_state->lexeme.end != '\0' &&
	     *lex_state->lexeme.end != ',' &&
	     *lex_state->lexeme.end != ':' &&
	     *lex_state->lexeme.end != '=' &&
	     *lex_state->lexeme.end != '}' &&
	     *lex_state->lexeme.end != '*' ) {
	    lex_state->lexeme.end++;
	 } else {
	    token->token = LEX_WORD;
	    token->value.lexeme.begin = lex_state->lexeme.begin;
	    token->value.lexeme.end   = lex_state->lexeme.end;

	    lex_state->state = LEX_STATE_CMDEXT;
	    lex_state->lexeme.begin = lex_state->lexeme.end;
	    return;
	 }	     
      } break;
      }
   }
}

/* 
 * Local Variables:
 * compile-command: "make dgd_format_lexer.obj"
 * End:
 */

