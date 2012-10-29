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

#ifndef JOGO_IO_MANAGER_H
#define JOGO_IO_MANAGER_H

#include "Primitives.h"
#include "Coroutine.h"
#include "Queue.h"
#ifdef WINDOWS
#include <windows.h>
#endif

typedef struct Io_Manager* Io_Manager;
struct Io_Manager {
    Ptr _vtable;
    U64 _refcount;
    Queue scheduled;
    Int handle;
    Int waiting;
    Int iobytes;
};

#ifdef WINDOWS
typedef struct Io_Overlapped {
    OVERLAPPED overlapped;
    Coroutine coroutine;
} Io_Overlapped;
#endif

Io_Manager Io_Manager__init();
void Io_Manager__destroy();
void Io_Manager_poll(Io_Manager self);
void Io_Manager_shutdown(Io_Manager self);
Int Io_manager_iobytes__g(Io_Manager self);
extern void Io_Manager__vtable();

void Io_wait();
Io_Manager Io_manager();

#endif
