/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "Hash.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>

Hash Hash__init(Int capacity) {
    Hash ret = calloc(sizeof(struct Hash), 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Hash__vtable;
    ret->_refcount = 1;
    ret->data = calloc(sizeof(Object), capacity);
    if (!ret->data) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->capacity = capacity;
    ret->count = 0;
    return ret; 
}

void Hash__destroy(Hash self) {
    Int i = 0;
    for (; i < self->count; ++i) {
        Object__refcount_dec(self->data[i]);
    }
    free(self->data);
    free(self);
}

Object Hash__bucket(Hash self, Int index) {
    Object ret = self->data[index];
    Object__refcount_inc(ret);
    return ret;
}
