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
// dgd_format_lexer.h -- lexical analizer for sprintf format string and more
//

#ifndef _dgd_format_lexer_h_
#define _dgd_format_lexer_h_

#include "dgd_types.h"
#include "dgd_format_parser.h"


#define LEX_STATE_NORMAL        0
#define LEX_STATE_EOF           0xfe
#define LEX_STATE_DONE          0xff
#define LEX_STATE_BACKSLASH     0x01
#define LEX_STATE_OCT           0x02
#define LEX_STATE_OCT1          0x03
#define LEX_STATE_OCTORHEX      0x04
#define LEX_STATE_HEX           0x05
#define LEX_STATE_HEX1          0x06
#define LEX_STATE_COMMAND       0x10
#define LEX_STATE_COMMAND_PRE   0x22
#define LEX_STATE_CMD_NUMBER    0x11
#define LEX_STATE_CMD_HALF      0x12
#define LEX_STATE_CMD_LONG      0x13
#define LEX_STATE_CMDEXT        0x20
#define LEX_STATE_CMDEXT_WORD   0x21

typedef str_range_t lexeme_t;

typedef struct _lexer_value_t {
      unsigned int token;      
      union {
	    char          ch;
	    unsigned int  num;
	    void         *ptr;
	    char         *str;
	    lexeme_t      lexeme;
      } value;
} lexer_value_t;

typedef struct _lexer_state_t {
      lexeme_t       lexeme;
      unsigned int   state;
} lexer_state_t;

void init_lexer_state( lexer_state_t *lex_state );
void format_lexer( lexer_state_t *lex_state, 
		   char          *format, 
		   lexer_value_t *token  );
void format_lexer_set_state( lexer_state_t *lex_state,
			     unsigned char state );

#endif /* _dgd_format_lexer_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_format_lexer.obj"
 * End:
 */


