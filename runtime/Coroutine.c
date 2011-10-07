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

#include "Coroutine.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


Coroutine Coroutine__init(Object func) {
    // Initializes a function with a new stack and instruction pointer. When
    // the coroutine is resumed, it will begin executing at 'function' with its
    // own stack.
    Coroutine ret = calloc(sizeof(struct Coroutine), 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Coroutine__vtable;
    ret->_refcount = 1; 
    ret->function = func;
    ret->status = 0; // New coroutine status code 

    Int stack_index = COROUTINE_STACK_SIZE - 2 - 1; 
    // -2 is for the two return addresses that are initially on the stack
    // -1 is for the initial values of RBP

    ret->sp = (Int)(ret->stack + stack_index); // ESP = R6
    
    ret->stack[COROUTINE_STACK_SIZE-1] = (Int)Coroutine__exit;
    ret->stack[COROUTINE_STACK_SIZE-2] = (Int)Object__dispatch2(func, "@call");

    return ret;
}

void Coroutine__call(Coroutine self) {
    // Resumes a coroutine if it is still running.  Otherwise, returns
    // immediately.
    if (self->status == 3 || self->status == 1) {
        // Coroutine is dead or already running
        return;
    } else {
        self->status = 1;
        Coroutine__resume(self);
        if (self->status != 3) {
            self->status = 2;
        }
    }
}

