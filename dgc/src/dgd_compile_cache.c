/* 
 * $Id: dgd\\dgc\\src\\Attic\\dgd_compile_cache.c,v 1.3 2003/08/09 23:05:11 dimka Exp $
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
 * dgd_compile_cache.c -- bytecode for dgd_format
 *
 */

#include "dgd_compile_cache.h"

unsigned int dgd_ring_size( cache_item_t *ring ) {
   cache_item_t* next;
   unsigned int count = 0;

   if( ring == NULL )
      return 0;

   next = ring;

   do {
      count++;
      next = next->next;
   } while( next != ring );

   return count;
}

cache_item_t* dgd_ring_pop_front( cache_item_t **ring ) {
   cache_item_t* front;
   cache_item_t* next;

   if( ring == NULL || *ring == NULL )
      return NULL;

   front = *ring;
   next = (*ring)->next;

   if( next == front ) {
      *ring = NULL;
      return front;
   }

   *ring = next;
   
   next->prev = front->prev;
   front->prev->next = next;

   front->next = front;
   front->prev = front;

   return front;
}

cache_item_t* dgd_ring_pop_back( cache_item_t **ring ) {
   cache_item_t* front;
   cache_item_t* back;
   cache_item_t* prev;

   if( ring == NULL || *ring == NULL )
      return NULL;

   front = *ring;
   back = (*ring)->prev;
   prev = back->prev;

   if( back == front ) {
      *ring = NULL;
      return front;
   }

   prev->next = back->next;
   back->next->prev = prev;

   back->next = back;
   back->prev = back;

   return back;
}

void dgd_ring_push_front( cache_item_t **ring, cache_item_t *item ) {
   cache_item_t *front;
   cache_item_t *back;
   cache_item_t *ifront;
   cache_item_t *iback;

   if( ring == NULL || item == NULL )
      return;

   if( *ring == NULL ) {
      *ring = item;
      return;
   }

   front = (*ring);
   back = front->prev;
   ifront = item;
   iback = item->prev;

   iback->next = front;
   front->prev = iback;
   
   ifront->prev = back;
   back->next = ifront;

   *ring = ifront;
}

void dgd_ring_push_back( cache_item_t **ring, cache_item_t *item ) {
   cache_item_t *front;
   cache_item_t *back;
   cache_item_t *ifront;
   cache_item_t *iback;

   if( ring == NULL || item == NULL )
      return;

   if( *ring == NULL ) {
      *ring = item;
      return;
   }

   front = (*ring);
   back = front->prev;
   ifront = item;
   iback = item->prev;

   back->next = ifront;
   ifront->prev = back;
   
   front->prev = iback;
   iback->next = front;
}


cache_item_t *dgd_ring_split( cache_item_t **ring, unsigned int size ) {
   cache_item_t *front;
   cache_item_t *back;
   cache_item_t *ifront;
   cache_item_t *iback;

   if( ring == NULL || *ring == NULL || size == 0 )
      return NULL;

   front = (*ring);
   back = front;

   if( front->next == back && size > 0 ) {
      *ring = NULL;
      return front;
   }

   while( --size > 0 && back->next != front ) back = back->next;

   if( back->next == front ) {
      *ring = NULL;
      return front;
   }
   
   ifront = back->next;
   iback = front->prev;

   back->next = front;
   front->prev = back;

   ifront->prev = iback;
   iback->next = ifront;

   *ring = ifront;
   
   return front;
}

cache_item_t *dgd_cache_alloc( cache_t *cache, unsigned int size ) {
   cache_item_t *res, *hash_ring, *data_ring, *ring;
   unsigned int res_size;

   if( cache == NULL || size > cache->size )
      return NULL;

   res = dgd_ring_split( &(cache->free_ring), size );
   res_size = dgd_ring_size( res );

   while( res_size < size ) {      
      if( cache->lru_ring == NULL )
	 return NULL;

      hash_ring = cache->lru_ring->value.ring;

      dgd_cache_free( cache, &(cache->lru_ring), 1  );      

      data_ring = hash_ring->value.hash.ring;

      dgd_cache_free( cache, &hash_ring, 1  );

      dgd_cache_free( cache, &data_ring, -1 );

      ring = dgd_ring_split( &(cache->free_ring), size-res_size );
      res_size += dgd_ring_size( ring );
      dgd_ring_push_back( &res, ring );
   }

   return res;
}

void dgd_cache_free( cache_t *cache, cache_item_t **ring, unsigned int size ) {
   cache_item_t *to_be_free;

   if( ring == NULL || *ring == NULL || 
       *ring - cache->table < DGD_CACHE_RANGE ) 
      return;

   to_be_free = dgd_ring_split( ring, size );

   if( to_be_free == NULL )
      return;

   dgd_ring_push_back( &(cache->free_ring), to_be_free );
}

void dgd_lookup_format( default_cache_t *cache ) {
   int i, data_size;

   memset( (char*)cache, '\0', sizeof(default_cache_t) );
   
   cache->head.table = cache->data;
   cache->head.size  = DGD_CACHE_SIZE;

   for( i = 0; i < DGD_CACHE_RANGE; i++ ) {
      cache->data[i].next = cache->data + i;
      cache->data[i].prev = cache->data + i;
   }

   data_size = cache->head.size - DGD_CACHE_RANGE;

   for( i = 0 ; i < data_size; i++ ) {
      cache->data[DGD_CACHE_RANGE+i].next = 
	 &(cache->data[ DGD_CACHE_RANGE + (i+1) % data_size ]);

      cache->data[DGD_CACHE_RANGE+i].prev = 
	 &(cache->head.table[ DGD_CACHE_RANGE + 
			 (i+data_size-1) % data_size ]);
   }

   /* first DGD_CACHE_RANGE entries are for the lookup table */
   cache->head.free_ring = cache->data + DGD_CACHE_RANGE; 
   cache->head.lru_ring = NULL;
   cache->head.hash_table = cache->data;
}


cache_item_t *dgd_cache_find( cache_t *cache, char *str ) {
   unsigned int hash_num;
   cache_item_t *chain, *next;
   cache_item_t *chain_ring, *lru_ring;

   hash_num = (unsigned int)str & (DGD_CACHE_RANGE-1);

   chain = cache->hash_table + hash_num;

   chain_ring = NULL;
   
   for( next = chain->next; next != chain; next = next->next ) {
      if( next->value.hash.key == str ) {
	 chain_ring = next;
	 break;
      }
   }
   
   if( chain_ring == NULL ) 
      return NULL;
   
   lru_ring = dgd_ring_pop_front( &(cache->lru_ring) );
   dgd_ring_push_back( &(cache->lru_ring), lru_ring );

   return chain_ring;
}

cache_item_t *dgd_cache_new( cache_t *cache, char *str, cache_item_t *data ) {
   cache_item_t *chain;
   cache_item_t *chain_ring, *lru_ring;
   unsigned int hash_num;

   hash_num = (unsigned int)str & (DGD_CACHE_RANGE-1);

   chain = cache->hash_table + hash_num;

   chain_ring = dgd_cache_alloc( cache, 1 );
   lru_ring   = dgd_cache_alloc( cache, 1 );

   if( chain_ring == NULL || lru_ring == NULL ) 
      return NULL;

   chain_ring->value.hash.key = str;
   chain_ring->value.hash.ring = data;
   chain_ring->value.hash.lru = lru_ring;

   lru_ring->value.ring = chain_ring;

   dgd_ring_push_back( &chain, chain_ring );
   dgd_ring_push_back( &(cache->lru_ring), lru_ring );
   
   return chain_ring;
}


void dgd_cache_delete( cache_t *cache, char *str ) {
   cache_item_t *chain_ring,*lru_ring, *data_ring;
   int reset_cache_lru = 0;

   chain_ring = dgd_cache_find( cache, str );

   if( chain_ring == NULL )
      return;

   lru_ring = chain_ring->value.hash.lru;
   data_ring = chain_ring->value.hash.ring;

   reset_cache_lru = ( cache->lru_ring == lru_ring );
   dgd_cache_free( cache, &lru_ring,   1 );
   if( reset_cache_lru )
      cache->lru_ring = lru_ring;

   dgd_cache_free( cache, &chain_ring, 1 );
   dgd_cache_free( cache, &data_ring, -1 );
}

