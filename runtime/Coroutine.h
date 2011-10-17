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

struct Coroutine_Stack {
    Int stack[COROUTINE_STACK_SIZE];
    struct Coroutine_Stack* next; // Pointer to the next stack
};

typedef struct Coroutine_Stack Coroutine_Stack;

struct Coroutine {
    Ptr _vtable; // 0
    U64 _refcount; // 8
    Object function;  //16
    Int status; // 24
    Int stack_size; // 32
    Int sp; // 40
    Coroutine_Stack* current; // 48
    Int padding;
    Coroutine_Stack stack;
};

typedef struct Coroutine* Coroutine;

Coroutine Coroutine__init(Object function);
void Coroutine__resume(Coroutine self);
void Coroutine__yield();
void Coroutine__exit();
Ptr Coroutine__grow_stack();

extern Coroutine_Stack* Coroutine__stack;
extern Coroutine Coroutine__current;
extern void Coroutine__vtable();

#endif
