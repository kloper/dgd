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
#include "dgd_format_cache.h"
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
   indent( stream, nesting_level+2 ); 
   fprintf( stream, "position:  %u\n", attr->position );
   indent( stream, nesting_level ); 
   fprintf( stream, "}\n" );
}

void dgd_dump_parser_bytecode( FILE *stream, 
			       cache_item_t *ring, 
			       unsigned int nesting_level ) {
   cache_item_t *next;

   if( ring == NULL ) 
      return;

   indent( stream, nesting_level ); fprintf( stream, "{\n" );

   next = ring;
   do {
      indent( stream, nesting_level+2 ); 

      switch( next->type ) {
	 case EVAL_T_INT:
	    fprintf( stream, "%x->load integer:\n", (unsigned int)next );
	    attrib( stream, &(next->value.argload.attr), nesting_level+2 );
	    break;
	 case EVAL_T_DOUBLE:
	    fprintf( stream, "%x->load double:\n", (unsigned int)next );
	    attrib( stream, &(next->value.argload.attr), nesting_level+2 );
	    break;
	 case EVAL_T_PTR:
	    fprintf( stream, "%x->load pointer:\n", (unsigned int)next );
	    attrib( stream, &(next->value.argload.attr), nesting_level+2 );
	    break;
         case PARS_T_ERROR:
            fprintf( stream, "%x->error:\n", (unsigned int)next );
            break;
	 case PARS_T_PAIR:
	    fprintf( stream, "%x->pair:\n", (unsigned int)next );
	    break;
	 case PARS_T_NEXT_ARG:
	    fprintf( stream, "%x->next arg [%x]\n", 
		     (unsigned int)next,
		     (unsigned int)next->value.next_arg.arg );
	    
	    break;
	 case PARS_T_LEXEME:
	    fprintf( stream, "%x->lexeme: %.*s\n", (unsigned int)next,
		     next->value.lexeme.lexeme.end - 
		     next->value.lexeme.lexeme.begin,
		     next->value.lexeme.lexeme.begin );
	    break;
	 case PARS_T_CALL_BY_NAME:
	    fprintf( stream, "%x->call by name: %.*s\n", 
		     (unsigned int)next,
		     next->value.call.name.end - next->value.call.name.begin,
		     next->value.call.name.begin );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_DEC:
	    fprintf( stream, "%x->decimal:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );	    
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_OCT:
	    fprintf( stream, "%x->octal:\n" , (unsigned int)next);
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_UNSIGNED:
	    fprintf( stream, "%x->unsigned:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_HEX:
	    fprintf( stream, "%x->hex:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_REP:
	    fprintf( stream, "%x->write report:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_SCI:
	    fprintf( stream, "%x->scientific:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_FLOAT:
	    fprintf( stream, "%x->float:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_SCIORFLOAT:
	    fprintf( stream, "%x->scientific or float:\n", 
		     (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_SCIHEX:
	    fprintf( stream, "%x->scientific hex:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_CHAR:
	    fprintf( stream, "%x->char:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_STR:
	    fprintf( stream, "%x->string:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_PTR:
	    fprintf( stream, "%x->pointer:\n", (unsigned int)next );
	    fprintf( stream, "   [%x] [%x] [%x]\n",
		      (unsigned int)next->value.call.arg,
		      (unsigned int)next->value.call.width,
		      (unsigned int)next->value.call.precision );
	    attrib( stream, &(next->value.call.attr), nesting_level+2 );
	    break;
	 case PARS_T_WORD:
	    fprintf( stream, "%x->word: %.*s\n", (unsigned int)next, 
		     next->value.word.lexeme.end - 
		     next->value.word.lexeme.begin,
		     next->value.word.lexeme.begin );
	    break;
	 case PARS_T_BACKSLASH:
	    fprintf( stream, "%x->backslash: %c\n", (unsigned int)next, 
		     next->value.backslash.ch );
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

