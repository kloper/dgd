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
 * cache.c -- test dgd_compile_cache
 *
 */

#include <stdio.h>
#include <string.h>

#include "dgd_compile_cache.h"

void cache_init( cache_t *cache ) {
   int i;

   for( i = 0; i < cache->size; i++ ) {
      cache->table[i].type = i;
   }
}

void cache_dump( cache_t *cache ) {
   int i;
   cache_item_t *next;

   printf( "cache = %p\n", cache );
   printf( "table (%d) = %p\n", cache->size, cache->table );
   printf( "hash: {\n" );

   for( i = 0; i < DGD_CACHE_RANGE; i++ ) {
      if( !dgd_ring_empty( cache->table + i ) ) {
	 printf( "[%i] ", i );
	 next = cache->table[i].next;
	 do {
	    printf("{ key: %s, ring: %p(%d) } ", 
		 next->value.hash.key,
		 next->value.hash.ring,
		 next->value.hash.ring->type );
	    next = next->next;
	 } while( next != cache->table + i );
	 printf( "\n" );
      }
   }
   printf( "} /* hash */\n" );
   printf( "lru[%d]: {\n", dgd_ring_size( cache->lru_ring ) );
   next = cache->lru_ring;
   if( next != NULL ) {
      do {
	 printf( "%p(%d): { %s }\n", next, next->type, 
		 next->value.ring->value.hash.key );
	 next = next->next;
      } while( next != cache->lru_ring );
      
   }
   printf( "} /* lru */\n" );
   printf( "free[%d]: {\n",  dgd_ring_size( cache->free_ring ) );
   next = cache->free_ring;
   if( next != NULL ) {
      do {
	 printf( "%p(%d) ", next, next->type );
	 next = next->next;
      } while( next != cache->free_ring );
      printf( "\n" );
   }
   printf( "} /* free */\n" );

}

typedef struct _assoc_t {
      char *name;
      cache_item_t *ring;
      struct _assoc_t *next;
} assoc_t;

default_cache_t cache;

int main( int argc, char** argv ) {
   int i;
   FILE *test_file;
   char buffer[21], buffer1[21];
   assoc_t *assoc = NULL, *assoc_item;
   cache_item_t *ring;
   unsigned int num;

   dgd_lookup_format( &cache );
   cache_init( (cache_t*)&cache );
   
   for( i = 1; i < argc; i++ ) {
      test_file = fopen( argv[i], "r" );
      if( test_file == NULL ) {
	 fprintf( stderr, "error: can't open file: %s\n", argv[i] );
	 continue;
      }
      
      while( !feof( test_file ) ) {
	 fscanf( test_file, "%s", buffer );
	 if( strcmp( buffer, "alloc" ) == 0 ) {
	    fscanf( test_file, "%s", buffer );
	    num = 0;
	    fscanf( test_file, "%u", &num );
	    if( num == 0 ) {
	       fprintf( stderr, "error: bad int\n" );
	       return 1;
	    }
	       
	    ring = dgd_cache_alloc( (cache_t*)&cache, num );

	    if( ring == NULL ) {
	       fprintf( stderr, "error: cache is full\n");
	       return 1;
	    }
	    
	    assoc_item = (assoc_t*) malloc( sizeof( assoc_t ) ) ;
	    assoc_item->name = strdup( buffer );
	    assoc_item->ring = ring;
	    assoc_item->next = assoc;
	    assoc = assoc_item;
	 } else if( strcmp( buffer, "assoc" ) == 0 ) {
	    fscanf( test_file, "%s", buffer1 );
	    fscanf( test_file, "%s", buffer );
	    
	    ring = NULL;
	    for( assoc_item = assoc; 
		 assoc_item != NULL; 
		 assoc_item = assoc_item->next ) {
	       if( strcmp( assoc_item->name, buffer ) == 0 ) {
		  ring = assoc_item->ring;
		  break;
	       }
	    }

	    dgd_cache_new( (cache_t*)&cache, buffer1, ring );
	 } else if( strcmp( buffer, "del" ) == 0 ) {
	    fscanf( test_file, "%s", buffer1 );
	    dgd_cache_delete( (cache_t*)&cache, buffer1 );
	 } else if( strcmp( buffer, "dump" ) == 0 ) {
	    cache_dump( (cache_t*)&cache );
	 } else {
	    fprintf( stderr, "error: unknown command %s\n", buffer );
	    return 1;
	 }
      }
      
      fclose( test_file );
   }
   

   return 0;
}

/* 
 * Local Variables:
 * compile-command: "make cache.obj"
 * End:
 */

