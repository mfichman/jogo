
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

#include "Io/Manager.h"
#include "Coroutine.h"
#ifndef WINDOWS
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>


Io_Manager Io_Manager__init() {
    // Initialize an event manager, and allocate an I/O completion port.
    Io_Manager ret = calloc(sizeof(struct Io_Manager), 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Io_Manager__vtable;
    ret->_refcount = 1;
    ret->scheduled = Queue__init(0);
#ifdef WINDOWS
    ret->handle = (Int)CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);
#else
    ret->handle = 0;
#endif
    return ret;
}

void Io_Manager__destroy(Io_Manager self) {
    Queue__destroy(self->scheduled);
#ifdef WINDOWS
    CloseHandle((HANDLE)self->handle);
#else
#endif
    free(self);
}

void Io_Manager_poll(Io_Manager self) {
    // Poll for an I/O event, and then resume the coroutine associated with
    // that event.
#ifdef WINDOWS
    DWORD bytes = 0;
    ULONG_PTR coro = 0;
    OVERLAPPED* evt = 0;
    HANDLE handle = (HANDLE)self->handle;
    GetQueuedCompletionStatus(handle, &bytes, &coro, &evt, INFINITE);
    if (evt) {
        Coroutine__call((Coroutine)coro); // Will return later
    }
#endif
}

