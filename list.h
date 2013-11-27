/*
 *  Copyright (c) 2012, Adrian Moser
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

#ifndef _LIST_H_
#define _LIST_H_

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct list_entry {
   void *value;
   struct list_entry *prev;
   struct list_entry *next;
} list_entry;

typedef struct {
   uint32_t size;
   list_entry *first;
   list_entry *last;
} list;

typedef struct {
   list *l;
   list_entry *next;
} list_it;

list *list_create();
void list_destroy(list *l);
bool list_is_empty(list *l);
uint32_t list_size(list *l);
void list_insert(list *l, void *value);
void list_append(list *l, void *value);
void *list_remove_first(list *l);
void *list_remove_last(list *l);
void list_apply(list *l, void(*f)(void *));

list_it *list_it_create(list *l);
void list_it_destroy(list_it *it);
bool list_it_has_next(list_it *it);
void *list_it_get_next(list_it *it);

#endif //_LIST_H_
