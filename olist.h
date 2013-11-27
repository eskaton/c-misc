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

#ifndef _OLIST_H_
#define _OLIST_H_

#include <sys/types.h>
#include <stdbool.h>

typedef struct olist_entry {
   void *value;
   struct olist_entry *prev;
   struct olist_entry *next;
} olist_entry;

typedef struct {
   u_int32_t size;
   olist_entry *first;
   olist_entry *last;
   int (*cmp_func)(void *, void *);
} olist;

typedef struct {
   olist *l;
   olist_entry *next;
} olist_it;

olist *olist_create(int (*cmp_func)(void *, void *));
void olist_destroy(olist *l);
bool olist_is_empty(olist *l);
u_int32_t olist_size(olist *l);
void olist_insert(olist *l, void *value);
void *olist_remove_first(olist *l);
void *olist_remove_last(olist *l);
void *olist_remove(olist *l, void *value);
void olist_apply(olist *l, void(*f)(void *));

olist_it *olist_it_create(olist *l);
void olist_it_destroy(olist_it *it);
bool olist_it_has_next(olist_it *it);
void *olist_it_get_next(olist_it *it);

#endif //_OLIST_H_
