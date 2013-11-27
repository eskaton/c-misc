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

#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include <sys/types.h>

typedef struct stack_entry {
   void *value;
   struct stack_entry *prev;  
} stack_entry;

typedef struct {
   u_int32_t size;
   stack_entry *top;
} stack;

typedef struct {
   stack *s;
   stack_entry *next;
} stack_it;

stack *stack_create();
void stack_destroy(stack *s);
void stack_destroy_free(stack *s, void (*free_entry)(void *value));
bool stack_is_empty(stack *s);
u_int32_t stack_size(stack *s);
void stack_push(stack *s, void *value);
void *stack_pop(stack *s);
void *stack_peek(stack *s);

stack_it *stack_it_create(stack *s);
void stack_it_destroy(stack_it *it);
bool stack_it_has_next(stack_it *it);
void *stack_it_get_next(stack_it *it);

#endif //_STACK_H_
