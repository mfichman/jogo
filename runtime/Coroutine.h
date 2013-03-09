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

#ifndef JOGO_COROUTINE_H
#define JOGO_COROUTINE_H

#ifdef WINDOWS
#include <windows.h>
#else
#include <signal.h>
#endif

#include "Primitives.h"

typedef struct CoroutineStack* CoroutineStack;
struct CoroutineStack {
    Int data[COROUTINE_STACK_SIZE];
};

extern Int CoroutineStatus_NEW;
extern Int CoroutineStatus_RUNNING;
extern Int CoroutineStatus_SUSPENDED;
extern Int CoroutineStatus_DEAD;
extern Int CoroutineStatus_IO;

typedef struct Coroutine* Coroutine;
struct Coroutine {
    VoidPtr _vtable; // 0
    U64 _refcount; // 8
    Object function;  //16
    Int status; // 24

    // NOTE: If the offset of this field changes, the assembly in
    // Coroutine*.asm must be modified accordingly. 
    Int sp; // 32 

    Coroutine caller; // 40
    CoroutineStack stack; // 48
    U64 stack_end;
};

Coroutine Coroutine__init(Object function);
void Coroutine_resume(Coroutine self); 
void Coroutine__start(Coroutine self);
void Coroutine__swap(Coroutine from, Coroutine to);
void Coroutine__yield();
void Coroutine__exit();
void Coroutine__call(Coroutine self);
void Coroutine__iowait();
void Coroutine__ioresume(Coroutine self);
void Coroutine__commit_page(Coroutine self, U64 addr);
void Coroutine__set_signals();
extern void Coroutine__vtable();
CoroutineStack CoroutineStack__init();

extern Coroutine Coroutine__current;
extern struct Coroutine Coroutine__main;
extern Int Exception__current;

#ifdef WINDOWS
// This function catches segfaults and grows the coroutine stack.
LONG WINAPI Coroutine__fault(LPEXCEPTION_POINTERS info);
#else
void Coroutine__fault(int signo, siginfo_t* info, void* context);
#endif

#endif
