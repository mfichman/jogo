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

#ifndef APOLLO_COROUTINE_H
#define APOLLO_COROUTINE_H

#include "Primitives.h"

#define COROUTINE_STACK_SIZE 4096
#define COROUTINE_CONTEXT_SIZE 4096

struct Coroutine {
    Ptr _vtable;
    U64 _refcount;
    Object function; 
    Int status;
    Int sp;
    Int padding; // Force stack to align 16
    Int stack[COROUTINE_STACK_SIZE];
};

typedef struct Coroutine* Coroutine;

Coroutine Coroutine__init(Object function);
void Coroutine__resume(Coroutine self);
void Coroutine__yield();
void Coroutine__exit();
extern void Coroutine__vtable();

#endif
