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
 * lexer.c -- test for dgd_format_lexer
 *
 */

#include <stdio.h>

#include "dgd_format_lexer.h"

void print_value( lexer_value_t* value ) {
   switch( value->token ) {
      case LEX_EOF:                 
	 printf( "LEX_EOF\n" );
	 break;
      case LEX_ZERO:     	
	 printf( "LEX_ZERO\n" );
	 break;
      case LEX_MINUS:    	
	 printf( "LEX_MINUS\n" );
	 break;
      case LEX_SPACE:
	 printf( "LEX_SPACE\n" );
	 break;
      case LEX_PLUS:
	 printf( "LEX_PLUS\n" );
	 break;
      case LEX_PERIOD:
	 printf( "LEX_PERIOD\n" );
	 break;
      case LEX_NUMBER:
	 printf( "LEX_NUMBER: %u\n", value->value.num );
	 break;
      case LEX_REFERENCE:
	 printf( "LEX_REFERENCE: %u\n", value->value.num );
	 break;
      case LEX_PERCENT:
	 printf( "LEX_PERCENT\n" );
	 break;
      case LEX_LEFTBRACE:
	 printf( "LEX_LEFTBRACE\n" );
	 break;
      case LEX_WORD:
	 printf( "LEX_WORD: %.*s\n", 
		 value->value.lexeme.end-value->value.lexeme.begin,
		 value->value.lexeme.begin );
	 break;
      case LEX_COLON:
	 printf( "LEX_COLON\n" );
	 break;
      case LEX_EQUAL:
	 printf( "LEX_EQUAL\n" );
	 break;
      case LEX_COMMA:
	 printf( "LEX_COMMA\n" );
	 break;
      case LEX_STAR:
	 printf( "LEX_STAR\n" );
	 break;
      case LEX_DOLLAR:
	 printf( "LEX_DOLLAR\n" );
	 break;
      case LEX_DIEZ:
	 printf( "LEX_DIEZ\n" );
	 break;
      case LEX_LEFT_BRACE:
	 printf( "LEX_LEFT_BRACE\n" );
	 break;
      case LEX_RIGHT_BRACE:
	 printf( "LEX_RIGHT_BRACE\n" );
	 break;
      case LEX_BACKSLASH:
	 printf( "LEX_BACKSLASH: %c (%1$hhx)\n", 
		 value->value.ch );
	 break;
      case LEX_MOD_HALFHALF:
	 printf( "LEX_MOD_HALFHALF\n" );
	 break;
      case LEX_MOD_HALF:
	 printf( "LEX_MOD_HALF\n" );
	 break;
      case LEX_MOD_LONGLONG:
	 printf( "LEX_MOD_LONGLONG\n" );
	 break;
      case LEX_MOD_LONG:
	 printf( "LEX_MOD_LONG\n" );
	 break;
      case LEX_MOD_INTMAXT:
	 printf( "LEX_MOD_INTMAXT\n" );
	 break;
      case LEX_MOD_SIZET:
	 printf( "LEX_MOD_SIZET\n" );
	 break;
      case LEX_MOD_PTRDIFFT:
	 printf( "LEX_MOD_PTRDIFFT\n" );
	 break;
      case LEX_MOD_LONGDOUBLE:
	 printf( "LEX_MOD_LONGDOUBLE\n" );
	 break;
      case LEX_T_DEC:
	 printf( "LEX_T_DEC\n" );
	 break;
      case LEX_T_OCT:
	 printf( "LEX_T_OCT\n" );
	 break;
      case LEX_T_UNSIGNED:
	 printf( "LEX_T_UNSIGNED\n" );
	 break;
      case LEX_T_HEX_LOW:
	 printf( "LEX_T_HEX_LOW\n" );
	 break;
      case LEX_T_HEX_HIGH:
	 printf( "LEX_T_HEX_HIGH\n" );
	 break;
      case LEX_T_SCI_LOW:
	 printf( "LEX_T_SCI_LOW\n" );
	 break;
      case LEX_T_SCI_HIGH:
	 printf( "LEX_T_SCI_HIGH\n" );
	 break;
      case LEX_T_FLOAT_LOW:
	 printf( "LEX_T_FLOAT_LOW\n" );
	 break;
      case LEX_T_FLOAT_HIGH:
	 printf( "LEX_T_FLOAT_HIGH\n" );
	 break;
      case LEX_T_SCIORFLOAT_LOW:
	 printf( "LEX_T_SCIORFLOAT_LOW\n" );
	 break;
      case LEX_T_SCIORFLOAT_HIGH:
	 printf( "LEX_T_SCIORFLOAT_HIGH\n" );
	 break;
      case LEX_T_SCIHEX_LOW:
	 printf( "LEX_T_SCIHEX_LOW\n" );
	 break;
      case LEX_T_SCIHEX_HIGH:
	 printf( "LEX_T_SCIHEX_HIGH\n" );
	 break;
      case LEX_T_CHAR:
	 printf( "LEX_T_CHAR\n" );
	 break;
      case LEX_T_STRING:
	 printf( "LEX_T_STRING: %.*s\n",
		 value->value.lexeme.end - value->value.lexeme.begin,
		 value->value.lexeme.begin );
	 break;
      case LEX_T_PTR:
	 printf( "LEX_T_PTR\n" );
	 break;
      case LEX_T_REPORT:
	 printf( "LEX_T_REPORT\n" );
	 break;
      default:
	 printf( "Unknown\n" );
	 break;
   }
}

int main( int argc, char** argv ) {
   int i;
   lexer_state_t lexer;
   lexer_value_t value;

   init_lexer_state( &lexer );

   for( i = 1; i < argc; i++ ) {
      printf( "> %s\n", argv[i] );
      do {
	 format_lexer( &lexer, argv[i], &value );
	 print_value( &value );
      } while( value.token != LEX_EOF );
   }

   return 0;
}

/* 
 * Local Variables:
 * compile-command: "make lexer.obj"
 * End:
 */

