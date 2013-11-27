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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "stack.h"

inline stack *
stack_create() {
   return (stack *)calloc(1, sizeof(stack));
}

inline void
stack_destroy(stack *s) {
   free(s);
}

void
stack_destroy_free(stack *s, void (*free_entry)(void *value)) {
   stack_entry *e = s->top;
   stack_entry *tmp;

   while (e) {
      tmp = e->prev; 
      free_entry(e->value);
      free(e);
      e = tmp;
   }

   free(s);
}

inline bool
stack_is_empty(stack *s) {
   return s->size == 0;
}

inline uint32_t
stack_size(stack *s) {
   return s->size;
}

void
stack_push(stack *s, void *value) {
   stack_entry *e = (stack_entry *)calloc(1, sizeof(stack_entry));
   e->value = value;
   
   if (!stack_is_empty(s)) {
      e->prev = s->top; 
   }

   s->top = e;
   s->size++;
}

void *
stack_pop(stack *s) {
   stack_entry *e; 
   void *value;

   if (stack_is_empty(s)) {
      return NULL; 
   }

   e = s->top; 
   s->top = e->prev;
   s->size--;
   value = e->value;
   free(e);
   
   return value;
}

inline void *
stack_peek(stack *s) {
   if (stack_is_empty(s)) {
      return NULL; 
   }

   return s->top->value;
}

stack_it *
stack_it_create(stack *s) {
   stack_it *it = (stack_it *)calloc(1, sizeof(stack_it));

   if (s) {
      it->s = s;
      it->next = s->top;
   }

   return it;
}

void
stack_it_destroy(stack_it *it) {
   free(it);
}

bool 
stack_it_has_next(stack_it *it) {
   return it->next != NULL;
}

void *
stack_it_get_next(stack_it *it) {
   stack_entry *e = it->next;

   if (e) {
      it->next = e->prev; 
      return e->value;
   } 

   return NULL;
}

