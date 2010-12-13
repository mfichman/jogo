/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#ifndef APHASH_H
#define APHASH_H

#include <apollo.h>

/* General purpose hash table */

typedef int (*aphash_compfn_t)(const void *, const void *);
typedef unsigned int (*aphash_hashfn_t)(const void *);
typedef int aphash_iter_t;

aphash_t *aphash_alloc(aphash_compfn_t comp, aphash_hashfn_t hash);
void *aphash_get(aphash_t *self, const void *key);
void *aphash_put(aphash_t *self, const void *key, void *value);
void *aphash_remove(aphash_t *self, const void *key);
aphash_iter_t aphash_iter(aphash_t *self);
void *aphash_next(aphash_t *self, aphash_iter_t *iter);
unsigned int aphash_string(const void *key);
unsigned int aphash_pointer(const void *key);
void aphash_free(aphash_t *self);

#endif
