// -*- c -*-
//
// $Id: dgd\\dgc\\src\\Attic\\dgd_compile_cache.h,v 1.1 2003/08/05 20:12:32 dimka Exp $
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
// dgd_compile_cache.h -- bytecode for dgd_format
//

#ifndef _dgd_compile_cache_h_
#define _dgd_compile_cache_h_

#include "dgd_config.h"

#include "dgd_types.h"

typedef struct _cache_item_t {
      unsigned int type;
      union {
	    unsigned int          num;
	    void                 *vptr;
	    char                 *cptr;
	    struct _cache_item_t *ring;
	    void                 *(*func)();
	    char                  ch;
	    str_range_t           lexeme;
	    struct {
		  char           *key;
		  char           *value;
	    } arg;
	    struct {
		  str_range_t           name;
		  struct _cache_item_t *args;
		  call_attr_t           attr;
	    } call;
	    struct {
		  char                 *key;
		  struct _cache_item_t *ring;
		  struct _cache_item_t *lru;
	    } hash;
      } value;

      struct _cache_item_t *next;
      struct _cache_item_t *prev;
} cache_item_t;

typedef struct _cache_t {
      unsigned int       size;
      cache_item_t      *table;
      cache_item_t      *free_ring;
      cache_item_t      *lru_ring;
      cache_item_t      *hash_table;
} cache_t;

typedef struct _default_cache_t {
      cache_t            head;
      cache_item_t       data[DGD_CACHE_SIZE];
} default_cache_t;

#define dgd_ring_empty(ring) ((ring)->next==(ring))
#define dgd_ring_forward(ring) ((ring)=(ring)->next)
#define dgd_ring_backward(ring) ((ring)=(ring)->prev)
#define dgd_ring_front(ring) (ring)
#define dgd_ring_back(ring) ((ring->prev))


unsigned int dgd_ring_size( cache_item_t *ring );
cache_item_t* dgd_ring_pop_front( cache_item_t **ring );
cache_item_t* dgd_ring_pop_back( cache_item_t **ring );
void dgd_ring_push_front( cache_item_t **ring, cache_item_t *item );
void dgd_ring_push_back( cache_item_t **ring, cache_item_t *item );
cache_item_t *dgd_ring_split( cache_item_t **ring, unsigned int size );
cache_item_t *dgd_cache_alloc( cache_t *cache, unsigned int size );
void dgd_cache_free( cache_t *cache, cache_item_t **ring, unsigned int size );
void dgd_lookup_format( default_cache_t *cache );
cache_item_t *dgd_cache_find( cache_t *cache, char *str );
cache_item_t *dgd_cache_new( cache_t *cache, char *str, cache_item_t *data );
void dgd_cache_delete( cache_t *cache, char *str );


#define dgd_cache_free_list

#endif /* _dgd_compile_cache_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_compile_cache.obj"
 * End:
 */


