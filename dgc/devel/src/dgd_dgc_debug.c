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
 * dgd_dgc_debug.c -- debugging routines for DGC
 *
 */

#include <stdio.h>

#include "dgd_config.h"
#include "dgd_compile_cache.h"
#include "dgd_format_parser.h"

#include "dgd_dgc_debug.h"

static
void indent( FILE *stream, unsigned int nesting_level ) {
   while( nesting_level-- ) fprintf( stream, " " );
}

static 
void attrib( FILE *stream, call_attr_t *attr, unsigned int nesting_level ) {
   indent( stream, nesting_level ); 
   fprintf( stream, "{\n" );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "valid_mask:  %x\n", attr->valid_mask );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "alternate:   %u\n", attr->alternate  );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "zero_pad:    %u\n", attr->zero_pad   );
   indent( stream, nesting_level+2 );
   fprintf( stream, "left_adjust: %u\n", attr->left_adjust);
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "blank:       %u\n", attr->blank      );
   indent( stream, nesting_level+2 );
   fprintf( stream, "capital:     %u\n", attr->capital    );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "width:       %d\n", attr->width      );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "precision    %d\n", attr->precision  );
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "byte_count:  %u\n", attr->byte_count );
   indent( stream, nesting_level ); 
   fprintf( stream, "}\n" );
}

void dgd_dump_parser_bytecode( FILE *stream, 
			       cache_item_t *ring, 
			       unsigned int nesting_level ) {
   cache_item_t *next;

   
   indent( stream, nesting_level ); fprintf( stream, "{\n" );

   if( ring == NULL ) 
      return;


   next = ring;
   do {
      indent( stream, nesting_level+2 ); 

      switch( next->type ) {
	 case PARS_T_PAIR:
	    fprintf( stream, "pair:\n" );
	    dgd_dump_parser_bytecode( stream, next->value.ring,
				      nesting_level+2 ); 
	    break;
	 case PARS_T_NEXT_ARG:
	    fprintf( stream, "next arg\n" );
	    break;
	 case PARS_T_SET_ARG:
	    fprintf( stream, "set argument: %u\n", next->value.num );
	    break;
	 case PARS_T_LEXEME:
	    fprintf( stream, "lexeme: %.*s\n", 
		     next->value.lexeme.end - next->value.lexeme.begin,
		     next->value.lexeme.begin );
	    break;
	 case PARS_T_CALL_BY_NAME:
	    fprintf( stream, "call by name: %.*s\n", 
		     next->value.call.name.end - next->value.call.name.begin,
		     next->value.call.name.begin );
	    dgd_dump_parser_bytecode( stream, next->value.call.args, 
				      nesting_level+2 );
	    break;
	 case PARS_T_READ_DEC:
	    fprintf( stream, "decimal:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_OCT:
	    fprintf( stream, "octal:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_UNSIGNED:
	    fprintf( stream, "unsigned:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_HEX:
	    fprintf( stream, "hex:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_WRITE_REP:
	    fprintf( stream, "write report:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_SCI:
	    fprintf( stream, "scientific:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_FLOAT:
	    fprintf( stream, "float:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_SCIORFLOAT:
	    fprintf( stream, "scientific or float:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_SCIHEX:
	    fprintf( stream, "scientific hex:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_CHAR:
	    fprintf( stream, "char:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_STR:
	    fprintf( stream, "string:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_READ_PTR:
	    fprintf( stream, "pointer:\n" );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_WORD:
	    fprintf( stream, "word: %.*s\n", 
		     next->value.lexeme.end - next->value.lexeme.begin,
		     next->value.lexeme.begin );
	    break;
	 case PARS_T_CHAR:
	    fprintf( stream, "backslash: %c\n", next->value.ch );
	    break;
	 default:
	    break;
      }

      next = next->next;
   } while( next != ring );

   indent( stream, nesting_level ); fprintf( stream, "}\n" );  
}

/* 
 * Local Variables:
 * compile-command: "make dgd_dgc_debug.obj"
 * End:
 */

