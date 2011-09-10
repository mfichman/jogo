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

#include "Array.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>


Array Array__init(Int capacity) {
    Array ret = calloc(sizeof(struct Array), 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Array__vtable;
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

void Array__destroy(Array self) {
    for (Int i = 0; i < self->count; ++i) {
        Object__refcount_dec(self->data[i]);
    }
    free(self->data);
    free(self);
    
}

Object Array__index(Array self, Int index) {
    if (index < 0 || index >= self->count) {
        return 0;
    } else {
        // Need to increment the refcount here; the code generator normally
        // does this automatically.
        Object__refcount_inc(self->data[index]);
        return self->data[index];
    }
}

void Array__insert(Array self, Int index, Object obj) {
    if (!obj || index < 0) {
        return;
    }

    if (index >= self->capacity) {
        if (self->capacity*2 > index+1) {
            self->capacity *= 2;
        } else {
            self->capacity = index+1;
        }
        Object* temp = calloc(sizeof(Object), self->capacity);
        if (!temp) {
            fprintf(stderr, "Out of memory");
            fflush(stderr);
            abort();
        }
        for (Int i = 0; i < self->count; ++i) {
            temp[i] = self->data[i];
        } 
        free(self->data);   
        self->data = temp;
    }
    if (index >= self->count) {
        self->count = index+1;
    }

    self->data[index] = obj;
    Object__refcount_inc(obj);
}

