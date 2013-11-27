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

#ifndef _HTAB_H_
#define _HTAB_H_

#include <stdbool.h>
#include <stdint.h>

#define HSIZE     16 /* Initial size for the hashtable */
#define REBUILD    3 /* Rebuild factor */

typedef struct htab_entry {
  void *value;                 /* Pointer to the data */
  void *key;                   /* Hashkey */
  struct htab_entry **fstbuck; /* Pointer to the first slot */
  struct htab_entry *nexth;    /* Pointer to next htab_entry */
} htab_entry;

typedef struct htab {
  htab_entry **buckets;  /* Array of pointers to htab_entrys */
  uint32_t num;          /* Number of entries */
  uint32_t maxent;       /* Maximum number of htab_entrys before rebuild */
  uint32_t htsize;       /* Size of the table */
  uint32_t mask;         /* Ensure a key is smaller than the number of buckets */
  bool (*eqfunc)();      /* Comperator function to find the matching entry */
  long (*hashfunc)();    /* Hash function to calculate the key */
} htab;

typedef struct {
   htab *ht;
   htab_entry *next;
   htab_entry **bucket;
} htab_it;


htab *htab_create(void *fhash, void *fequals);
void htab_destroy(htab *ht);
void htab_destroy_free(htab *ht, void (*free)(htab_entry *entry));
void *htab_get(htab* ht, void *key);
htab_entry *htab_get_entry(htab *ht, void *key);
bool htab_contains(htab *ht, void *key);
htab_entry *htab_put(htab* ht, void *key, void *value);
htab_entry *htab_delete(htab *ht, void *key);
void htab_rehash(htab *ht);
void htab_entry_destroy(htab_entry *entry);

htab_it *htab_it_create(htab *ht);
void htab_it_destroy(htab_it *it);
bool htab_it_has_next(htab_it *it);
htab_entry *htab_it_get_next(htab_it *it);
void htab_print_state(htab *ht);

#endif //_HTAB_H_

