// -*- c -*-
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
// dgd_format_cache.h -- bytecode for dgd_format
//

#ifndef _dgd_format_cache_h_
#define _dgd_format_cache_h_

#include "dgd_config.h"

#include "dgd_types.h"

typedef struct _cache_item_t {
      unsigned int type;
      union {
	    struct {
		  str_range_t           name;
		  int                   index;
		  call_attr_t           attr;
		  unsigned int          num_param;
		  struct _cache_item_t *arg;
		  struct _cache_item_t *width;
		  struct _cache_item_t *precision;
	    } call;
	    struct {
		  char                 *key;
		  struct _cache_item_t *ring;
		  struct _cache_item_t *lru;		  
	    } chain;
	    struct {
		  struct _cache_item_t *ring;
	    } lru;
	    struct {
		  struct _cache_item_t *arg;
	    } next_arg;
	    struct {
		  str_range_t           lexeme;
	    } lexeme;
	    struct {
		  unsigned int          error;
		  unsigned int          num;
		  str_range_t           lexeme;
	    } error;
	    struct {
		  str_range_t           lexeme;
	    } word;
	    struct {
		  char                  ch;
	    } backslash;
	    struct {		  
		  unsigned int          index;
		  union {
			int             num;
			double          dnum;
			void           *ptr;
			ext_args_t      args;
		  } v;
	    } argload;
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

#endif /* _dgd_format_cache_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd_format_cache.obj"
 * End:
 */


