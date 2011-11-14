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
#include "String.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

Coroutine Coroutine__current = 0;
Coroutine_Stack* Coroutine__stack = 0;

Coroutine Coroutine__init(Object func) {
    // Initializes a function with a new stack and instruction pointer. When
    // the coroutine is resumed, it will begin executing at 'function' with its
    // own stack.
    Int stack_index = COROUTINE_STACK_SIZE - 2 - 8; 
    // -2 is for the two return addresses that are initially on the stack
    // -16 is for the initial values of RBP + caller regs
    
    Coroutine ret = calloc(sizeof(struct Coroutine), 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }

    ret->_vtable = Coroutine__vtable;
    ret->_refcount = 1; 
    ret->function = func;
    ret->current = &ret->stack;
    ret->stack_size = COROUTINE_STACK_SIZE;

    ret->sp = (Int)(ret->stack.stack + stack_index); // ESP = R6
    ret->stack.next = 0;

    Object__refcount_inc(func);
    if (func) {
        static struct String call_str = { String__vtable, 1, 5, "@call" };
        Int exit = (Int)Coroutine__exit;
        Int call = (Int)Object__dispatch(func, &call_str);
        ret->stack.stack[COROUTINE_STACK_SIZE-1] = exit;
        ret->stack.stack[COROUTINE_STACK_SIZE-2] = call;
        ret->status = CoroutineStatus_NEW; // New coroutine status code 
    } else {
        ret->status = CoroutineStatus_DEAD; 
    }

    return ret;
}

void Coroutine__destroy(Coroutine self) {
    // Free the stack, and the pointer to the closure object so that no memory
    // is leaked.
    Coroutine_Stack* stack = 0;
    for (stack = self->stack.next; stack;) {
        Coroutine_Stack* temp = stack;
        stack = stack->next;
        free(temp);
    }

    Object__refcount_dec(self->function);
    free(self);
}

void Coroutine__call(Coroutine self) {
    // Resumes a coroutine if it is still running.  Otherwise, returns
    // immediately.
    if (self->status == CoroutineStatus_DEAD) {
        return;
    } else if (self->status == CoroutineStatus_RUNNING) {
        // Coroutine is dead or already running
        return;
    } else {
        self->status = CoroutineStatus_RUNNING;
        Coroutine__resume(self);
        if (self->status != CoroutineStatus_DEAD) {
            self->status = CoroutineStatus_SUSPENDED;
        }
    }
}

Ptr Coroutine__grow_stack() {
    // Grows the coroutine stack, and keeps track of the next stack pointer.
    // Returns the pointer to the next stack.  Each function call has a section
    // that checks the stack to make sure it has enough remaining space.
    // Basically, if the stack doesn't have at least 512 bytes left, then a new
    // stack pointer will be allocated.  Note that this doesn't protect against
    // calls to native functions using more stack then they should. 
    if (!Coroutine__stack->next) {
        Coroutine__stack->next = calloc(sizeof(Coroutine_Stack), 1); 
        Coroutine__stack->next->next = 0;
    }
    if (Coroutine__current) {
        Coroutine__current->stack_size += COROUTINE_STACK_SIZE;
    }
    Coroutine__stack = Coroutine__stack->next;
    return Coroutine__stack->stack+COROUTINE_STACK_SIZE-2;
}

