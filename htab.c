/*
 *  Copyright (c) 2011, Adrian Moser
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  * Neither the name of the author nor the
 *  names of its contributors may be used to endorse or promote products
 *  derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

htab * 
htab_create(void *fhash, void *fequals) {
  htab *ht = (htab *)calloc(1, sizeof(htab));
  ht->num = 0;
  ht->maxent = HSIZE * REBUILD;
  ht->mask = HSIZE - 1;
  ht->htsize = HSIZE;
  ht->hashfunc = fhash;
  ht->eqfunc = fequals;
  ht->buckets = (htab_entry **)calloc(ht->htsize, sizeof(htab_entry*)); 
  return ht;
}

void 
htab_destroy(htab *ht) {
   uint32_t i;

   for (i=0; i < ht->htsize; i++) {
      if (ht->buckets[i] != NULL) {
         htab_entry *hi = ht->buckets[i];
         while (hi) {
            htab_entry *next = hi->nexth;
            free(hi);
            hi = next; 
         }
      }
   }

   free(ht->buckets);
   free(ht);
}

void
htab_destroy_free(htab *ht, void (*free_entry)(htab_entry *entry)) {
   uint32_t i;

   for (i=0; i < ht->htsize; i++) {
      if (ht->buckets[i] != NULL) {
         htab_entry *hi = ht->buckets[i];
         while (hi) {
            htab_entry *next = hi->nexth;
            free_entry(hi);
            free(hi);
            hi = next; 
         }
      }
   }

   free(ht->buckets);
   free(ht);
}

void *
htab_get(htab *ht, void *key) {
   htab_entry *entry = htab_get_entry(ht, key);

   if (entry) {
      return entry->value; 
   }

   return NULL;
}

htab_entry *
htab_get_entry(htab *ht, void *key) {
   register htab_entry *hi;

   for (hi=ht->buckets[ht->hashfunc(key) & ht->mask]; hi; hi=hi->nexth) {
      if (ht->eqfunc(key, hi->key)) {
         return hi;
      }
   }

   return NULL;
}

bool
htab_contains(htab *ht, void *key) {
   if (htab_get(ht, key) != NULL) {
      return true;
   }

   return false;
}

static void 
rebuild_ht(htab *ht, int growth_factor) {
   htab_entry **oldbuck = ht->buckets;
   htab_entry **oldbptr;
   htab_entry *hi;     
   int oldsize = ht->htsize;

   ht->htsize *= growth_factor;
   ht->mask = ht->htsize - 1;
   ht->maxent = ht->htsize * growth_factor;
   ht->buckets = (htab_entry **)calloc(ht->htsize, sizeof(htab_entry *));

   /* Rehash the old values */
   for (oldbptr=oldbuck; 0<oldsize--; oldbptr++) {
      for (hi=*oldbptr; hi != NULL; hi=*oldbptr) {
         *oldbptr = hi->nexth;
         hi->fstbuck = &(ht->buckets[ht->hashfunc(hi->key) & ht->mask]);
         hi->nexth = *hi->fstbuck;
         *hi->fstbuck = hi;
      }
   }

   free(oldbuck);
}

void
htab_rehash(htab *ht) {
   rebuild_ht(ht, 1);
}

htab_entry * 
htab_put(htab *ht, void *key, void *value) {
   if (ht->num+1 >= ht->maxent) {
      rebuild_ht(ht, REBUILD);
   }

   htab_entry *entry = (htab_entry *)calloc(1, sizeof(htab_entry));
   htab_entry *existing = htab_get_entry(ht, key);

   if (existing) {
      entry->key = existing->key;
      entry->value = existing->value;
      entry->fstbuck = NULL;
      entry->nexth = NULL;

      existing->key = key;
      existing->value = value;

      return entry;
   } else {
      ht->num++;
      entry->key = key;
      entry->value = value;
      entry->fstbuck = &(ht->buckets[ht->hashfunc(key) & ht->mask]);
      entry->nexth = *entry->fstbuck;
      *entry->fstbuck = entry;
      return NULL;
   }
}

htab_entry *
htab_delete(htab *ht, void *key) {
   htab_entry *hi, *last_hi = NULL;

   for (hi=ht->buckets[ht->hashfunc(key) & ht->mask]; hi; last_hi=hi, hi=hi->nexth) {
      if (ht->eqfunc(key, hi->key)) {
         if (last_hi) {
            last_hi->nexth = hi->nexth;
         } else {
            *hi->fstbuck = hi->nexth;
         } 

         ht->num--;
         return hi;
      }
   }

   return NULL;
}

void 
htab_entry_destroy(htab_entry *entry) {
   free(entry);
}

static inline htab_entry *
htab_get_next_entry(htab *ht, htab_entry ***bucket, htab_entry *e) {
   if (!e && **bucket) {
      return **bucket;
   } else if(!**bucket || !e->nexth) {
      if (*bucket < &ht->buckets[ht->htsize-1]) {
         do {
            (*bucket)++; 
         } while(**bucket == NULL && *bucket < &ht->buckets[ht->htsize-1]);
         return **bucket;
      }

      return NULL;
   }

   return e->nexth;
}


htab_it *
htab_it_create(htab *ht) {
   htab_it *it = (htab_it *)calloc(1, sizeof(htab_it));

   if (ht) {
      it->ht = ht; 
      it->bucket = ht->buckets;
      it->next = htab_get_next_entry(ht, &it->bucket, NULL);
   }

   return it;
}

inline void 
htab_it_destroy(htab_it *it) {
   free(it);
}

inline bool 
htab_it_has_next(htab_it *it) {
   return it->next != NULL;
}

htab_entry *
htab_it_get_next(htab_it *it) {
   htab_entry *e = it->next;

   if (it->next) {
      it->next = htab_get_next_entry(it->ht, &it->bucket, it->next);
   }

   return e;
}

void
htab_print_state(htab *ht) {
   uint32_t i;

   for (i=0; i < ht->htsize; i++) {
      printf("<%p>: %-5u ", &ht->buckets[i], i);
      if (ht->buckets[i] != NULL) {
         htab_entry *hi;
         for (hi=ht->buckets[i]; hi; hi=hi->nexth) {
            printf(" %p(%p,%p)", hi, hi->key, hi->value);
         }
      }
      printf("\n");
   }
}

