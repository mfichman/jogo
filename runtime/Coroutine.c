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
#include "Io/Manager.h"
#include "Boot/Module.h"
#include "Os/Module.h"
#include "Object.h"
#include "String.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define COROUTINE_BUFFER 4

struct Coroutine Coroutine__main;
Coroutine Coroutine__current = &Coroutine__main;
Coroutine_Stack Coroutine__stack = 0;
Int Exception__current = 0;


void Coroutine__marshal(Coroutine self, Int ra) {
    // Marshals a function call on the coroutine stack
#ifdef WINDOWS
    Int stack_arg_regs = 4;
    Int i = 0;
    for(i = 0; i < stack_arg_regs; i++) {
        --self->sp;
    }
#endif
    self->stack->data[--self->sp] = ra;

    // Stack diagram:
    // low memory 
    // [+5] arg3 (Windows only)
    // [+4] arg2 (Windows only)
    // [+3] arg1 (Windows only)
    // [+2] arg0 (Windows only)
    // [+1] return address
    // high memory
}

Coroutine Coroutine__init(Object func) {
    // Initializes a function with a new stack and instruction pointer. When
    // the coroutine is resumed, it will begin executing at 'function' with its
    // own stack.
    Coroutine ret = Boot_calloc(sizeof(struct Coroutine)); 
    ret->stack = Boot_calloc(sizeof(struct Coroutine_Stack));
    ret->_vtable = Coroutine__vtable;
    ret->_refcount = 1; 
    ret->function = func;
    ret->caller = 0;
    ret->sp = COROUTINE_STACK_SIZE;

    Object__refcount_inc(func);
    if (func) {
        static struct String call_str = String__static("@call");
        Int exit = (Int)Coroutine__exit;
        Int call = (Int)Object__dispatch(func, &call_str);
        Int bogus = 0; // Bogus return addr for exit's stack frame
        Coroutine__marshal(ret, bogus); // Simulate a fn call to exit()
        ret->stack->data[--ret->sp] = exit;
        ret->stack->data[--ret->sp] = call;
        ret->stack->data[--ret->sp] = (Int)ret->stack->data;
        // This is the top-of-stack pointer, which Coroutine__swap restores.
        // It is a pointer to the end of the stack space -- when this is
        // reached, a new stack segment is allocated.
        ret->stack->data[--ret->sp] = (Int)func; 
        // ARG0 in the .asm file.  This is the 'self' pointer for the closure
        // that gets invoked when the coroutine starts for the first time.
        ret->sp -= 15; // For the 15 registers pushed in the .asm file
        ret->sp = (Int)(ret->stack->data + ret->sp); 
        // Make relative to the top-of-stack
        ret->status = CoroutineStatus_NEW; // New coroutine status code 
        // At the time Coroutine__swap is called for the first time, the call
        // stack looks like this:
        //
        // arg3 arg2 arg1 arg0 bogus exit call tos ARG0 regs...+15
        //
        // Coroutine__swap pops regs, ARG0, and tos for its own use.  When
        // Coroutine__swap's ret instruction is executed, the coroutine will
        // jump to the function 'call.' Upon entering 'call', the stack looks
        // like this:
        //
        // arg3 arg2 arg1 arg0 bogus exit 
        //
        // From here, 'call' can build its stack frame.  When 'call' exits, it
        // pops the return address for 'exit' off the stack, and 'exit' begins
        // executing.  Upon entering exit, the stack looks like this:
        //
        // arg3 arg2 arg1 arg0 bogus 
        //
        // Coroutine__exit never returns, so 'bogus' is never jumped to.
        // Instead, the coroutine swaps to another coroutine before it falls
        // off the end of 'exit'.  arg3-0 are needed on Windows as a backing
        // store for the arg regs (according to the Win64 calling convention),
        // even though Coroutine__exit has no parameters.
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
        // Temporarily re-increment the refcount so that there's no double 
        // free; otherwise, the reference counting in resume() would cause
        // the coroutine to be destroyed twice.
        self->_refcount++;
        Coroutine__call(self);
        self->_refcount--;
    } else if (self->status == CoroutineStatus_RUNNING) {
        Os_cpanic("Coroutine freed while running");
    } else if (self->status == CoroutineStatus_IO) {
        Os_cpanic("Coroutine freed while waiting for I/O");
    } else if (self->status == CoroutineStatus_DEAD) {
        // Stack references are already freed.
    }
    
    // Free the stack, and the pointer to the closure object so that no memory
    // is leaked.
    for (stack = self->stack; stack;) {
        Coroutine_Stack temp = stack;
        stack = stack->next;
        Boot_free(temp);
    }

    Object__refcount_dec(self->function);
    Boot_free(self);

    Exception__current = save_except;
}

void Coroutine_resume(Coroutine self) {
    // Resumes a coroutine, and sets the 'caller' coroutine to the current 
    // coroutine.  Returns immediately if the coroutine is DEAD or nil.
    //printf("resuming coroutine %p\n", self);
    if (!self) { return; }
    if (self->status == CoroutineStatus_DEAD) { return; }
    if (self->status == CoroutineStatus_RUNNING) { return; }
    // If the user tries to resume a coroutine that is in the 'IO' state, then
    // it will just immediately block again.  So, to avoid that tricky case,
    // just return immediately from resume().
    if (self->status == CoroutineStatus_IO) { return; }

    // Note: The coroutine's refcount gets incremented by one while the 
    // coroutine is running, so that the coroutine won't get freed while its
    // stack is active.
    self->status = CoroutineStatus_RUNNING;
    Object__refcount_inc((Object)self);
    self->caller = Coroutine__current;
    Coroutine__swap(Coroutine__current, self);
    self->caller = 0; 
    Object__refcount_dec((Object)self);
}

void Coroutine__exit() {
    // Yields the the current coroutine to the coroutine's caller.
    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        Coroutine__swap(Coroutine__current, &Coroutine__main);
        // If there is no caller, then yield to the main coroutine.
    } else {
        Os_cpanic("No coroutine to yield to");
    }
}

void Coroutine__yield() {
    // Yields the the current coroutine to the coroutine's caller.  This is a
    // no-op if the coroutine is the main coroutine.
    //printf("yielding coroutine %p\n", Coroutine__current);
    if (Coroutine__current == &Coroutine__main) {
        Os_cpanic("Coroutine::yield() called by main coroutine");
    } else if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_SUSPENDED;
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_SUSPENDED;
        Coroutine__swap(Coroutine__current, &Coroutine__main);
    } else {
        Os_cpanic("No coroutine to yield to");
    }
}

VoidPtr Coroutine__grow_stack() {
    // Grows the coroutine stack, and keeps track of the next stack pointer.
    // Returns the pointer to the next stack.  Each function call has a section
    // that checks the stack to make sure it has enough remaining space.
    // Basically, if the stack doesn't have at least 512 bytes left, then a new
    // stack pointer will be allocated.  Note that this doesn't protect against
    // calls to native functions using more stack then they should. 
    if (!Coroutine__stack->next) {
        Coroutine__stack->next = Boot_calloc(sizeof(struct Coroutine_Stack)); 
        Coroutine__stack->next->next = 0;
    }
    Coroutine__stack = Coroutine__stack->next;
    return Coroutine__stack->data + COROUTINE_STACK_SIZE - 2;
}

void Coroutine__iowait() {
    // Causes this coroutine to wait until I/O is available.  Note: calling
    // this function if no I/O is pending will cause the Coroutine to block
    // indefinitely. 
    //printf("waiting on I/O for %p\n", Coroutine__current);
    Object__refcount_inc((Object)Coroutine__current);
    Io_manager()->waiting++;

    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_IO;
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_IO;
        Coroutine__swap(Coroutine__current, &Coroutine__main);
    } else {
        Os_cpanic("Coroutine::iowait() called by main coroutine");
    }
    // Same as the impl of yield, except it sets the status to 'IO' rather than
    // 'SUSPENDED'

    Object__refcount_dec((Object)Coroutine__current);
    Io_manager()->waiting--;
}

void Coroutine__ioresume(Coroutine self) {
    // Resume the coroutine, but preserve the caller.  This function is called
    // by the I/O manager when a pending I/O request completes.
    //printf("I/O complete for %p\n", self);
    if (!self) { return; }
    if (self->status == CoroutineStatus_DEAD) { return; }
    if (self->status == CoroutineStatus_RUNNING) { return; }
    if (self->status != CoroutineStatus_IO) {
        Os_cpanic("Illegal coroutine state");
    }

    self->status = CoroutineStatus_RUNNING;
    Object__refcount_inc((Object)self);
    Coroutine__swap(Coroutine__current, self);
    Object__refcount_dec((Object)self);
}

