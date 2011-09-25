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

#include "Io/Buffer.h"
#include <stdlib.h>
#include <stdio.h>


Io_Buffer Io_Buffer__init(Int capacity) {
    Io_Buffer ret = calloc(sizeof(struct Io_Buffer) + capacity, 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Io_Buffer__vtable;
    ret->_refcount = 1;
    ret->capacity = capacity;
    ret->begin = 0;
    ret->end = 0;
    return ret; 
}

Byte Io_Buffer__index(Io_Buffer self, Int index) {
    if (index < self->begin || index >= self->end) {
        return 0;
    } else {
        return self->data[self->begin+index];
    }
}

void Io_Buffer__insert(Io_Buffer self, Int index, Byte byte) {
    if (index < self->begin || index >= self->end) {
        return;
    } else {
        self->data[index] = byte;
    }
}

