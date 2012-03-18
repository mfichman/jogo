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

struct Coroutine Coroutine__main;
Coroutine Coroutine__current = &Coroutine__main;
Coroutine_Stack Coroutine__stack = 0;
Int Exception__current = 0;

Coroutine Coroutine__init(Object func) {
    // Initializes a function with a new stack and instruction pointer. When
    // the coroutine is resumed, it will begin executing at 'function' with its
    // own stack.
    Int stack_index = COROUTINE_STACK_SIZE - 2 - 1 - 15 - 1; 
    // -2 is for the two return addresses that are initially on the stack
    // -1 is for the top-of-stack pointer
    // -15 is for the initial values of RBP + caller regs
    // -1 is for initial arg to @call
    
    Coroutine ret = calloc(sizeof(struct Coroutine), 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->stack = calloc(sizeof(struct Coroutine_Stack), 1);
    if (!ret->stack) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    
    ret->_vtable = Coroutine__vtable;
    ret->_refcount = 1; 
    ret->function = func;
    ret->caller = 0;
    ret->sp = (Int)(ret->stack->data + stack_index); // ESP = R6

    Object__refcount_inc(func);
    if (func) {
        static struct String call_str = { String__vtable, 1, 5, "@call" };
        Int exit = (Int)Coroutine__exit;
        Int call = (Int)Object__dispatch(func, &call_str);
        ret->stack->data[COROUTINE_STACK_SIZE-1] = exit;
        ret->stack->data[COROUTINE_STACK_SIZE-2] = call;
        ret->stack->data[COROUTINE_STACK_SIZE-3] = (Int)ret->stack->data;
        ret->stack->data[COROUTINE_STACK_SIZE-4] = (Int)func;
        ret->status = CoroutineStatus_NEW; // New coroutine status code 
    } else {
        ret->status = CoroutineStatus_DEAD; 
    }

    return ret;
}

void Coroutine__destroy(Coroutine self) {
    Coroutine_Stack stack = 0;

	// Set the exception flag, then resume the coroutine and unwind the stack,
	// calling destructors for objects referenced by the coroutine.
	Int save_except = Exception__current;
	Exception__current = 1;

    if (self->status == CoroutineStatus_SUSPENDED) {
	    Coroutine__call(self);
    }
	
	// Free the stack, and the pointer to the closure object so that no memory
    // is leaked.
    for (stack = self->stack; stack;) {
        Coroutine_Stack temp = stack;
        stack = stack->next;
        free(temp);
    }

    Object__refcount_dec(self->function);
    free(self);

	Exception__current = save_except;
}

void Coroutine_resume(Coroutine self) {
    // Resumes a coroutine, and sets the 'caller' coroutine to the current 
    // coroutine.  Returns immediately if the coroutine is DEAD or nil.
    if (!self) { return; }
    if (self->status == CoroutineStatus_DEAD) { return; }
    if (self->status == CoroutineStatus_RUNNING) { return; }

    self->status = CoroutineStatus_RUNNING;
    self->caller = Coroutine__current;
    Coroutine__swap(Coroutine__current, self);
    self->caller = 0; 
}

void Coroutine__exit() {
    // Yields the the current coroutine to the coroutine's caller.  This is a
    // no-op if the coroutine is the main coroutine.
    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if(Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        Coroutine__swap(Coroutine__current, &Coroutine__main);
        // If there is no caller, then yield to the main coroutine.
    }
}

void Coroutine__yield() {
    // Yields the the current coroutine to the coroutine's caller.  This is a
    // no-op if the coroutine is the main coroutine.
    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_SUSPENDED;
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
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
        Coroutine__stack->next = calloc(sizeof(struct Coroutine_Stack), 1); 
        Coroutine__stack->next->next = 0;
    }
    Coroutine__stack = Coroutine__stack->next;
    return Coroutine__stack->data + COROUTINE_STACK_SIZE - 2;
}

