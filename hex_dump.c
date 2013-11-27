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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>

#define BYTES_PER_WORD 4
#define BYTES_PER_LINE (4 * BYTES_PER_WORD)

static const char const *spc_line = "                                    ";

static int 
print_word(FILE *dest, void *buf, size_t len) {
   size_t i;
   int word_len = 0;

   for (i = 0; i < len; i++) {
      fprintf(dest, "%02x", *(u_int8_t*)buf++);
      word_len += 2;
   } 

   fprintf(dest, " ");
   
   return word_len + 1;
}

static void
print_line(FILE *dest, void *buf, size_t len, size_t addr, char *addr_fmt) {
   size_t words = len / BYTES_PER_WORD;
   size_t last_word = len % BYTES_PER_WORD;
   long *pos = buf;
   size_t i;
   uint32_t line_pos = 0;

   fprintf(dest, addr_fmt, addr);

   for (i = 0; i < words; i++) {
      line_pos += print_word(dest, pos, BYTES_PER_WORD);
      pos++;
   }

   if (last_word) {
      line_pos += print_word(dest, pos, last_word);
   }

   fprintf(dest, "%s", &spc_line[line_pos]);
   fprintf(dest, "| ");

   for (i = 0; i < len; i++) {
      char c = ((char*)buf)[i];
      if (isprint(c)) {
         fprintf(dest, "%c", c); 
      } else {
         fprintf(dest, "."); 
      }
   }

   fprintf(dest, "\n");
}

void 
hex_dump_to(FILE *dest, void *buf, size_t len, size_t addr) {
   int lines = len / BYTES_PER_LINE;
   int last_len = len % BYTES_PER_LINE;
   int addr_len = ceil(log(len) / log(BYTES_PER_LINE));
   char *addr_fmt = malloc(20);
   int i;

   if (addr > 0) {
      sprintf(addr_fmt, "%%p | ");
   } else {
      sprintf(addr_fmt, "%%0%dx | ", addr_len);
   }

   for (i = 0; i < lines; i++) {
      print_line(dest, buf, BYTES_PER_LINE, addr, addr_fmt); 
      buf += BYTES_PER_LINE;
      addr += BYTES_PER_LINE;
   }

   if (last_len > 0) {
      print_line(dest, buf, last_len, addr, addr_fmt); 
   }

   free(addr_fmt);
}

void 
hex_dump(void *buf, size_t len, size_t addr) {
   hex_dump_to(stderr, buf, len, addr);
}

