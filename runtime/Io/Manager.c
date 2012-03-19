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
#include "Io/Stream.h"
#include "Coroutine.h"
#include "String.h"
#include "Object.h"
#ifndef WINDOWS
#include <unistd.h>
#include <errno.h>
#else
#include <windows.h>
#endif
#ifdef DARWIN
#include <sys/event.h>
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
    ret->waiting = 0;
#if defined(WINDOWS)
    ret->handle = (Int)CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
#elif defined(DARWIN)
    ret->handle = (Int)kqueue();
#else
    ret->handle = 0;
#endif
    return ret;
}

void Io_Manager_shutdown(Io_Manager self) {
#ifdef WINDOWS
    CloseHandle((HANDLE)self->handle);
#else
    close(self->handle);
#endif
}

void Io_Manager__destroy(Io_Manager self) {
    Io_Manager_shutdown(self);
    Queue__destroy(self->scheduled);
    free(self);
}

void Io_Manager_poll(Io_Manager self) {
    // Poll for an I/O event, and then resume the coroutine associated with
    // that event.
    static struct String resume_str = { String__vtable, 1, 6, "resume" };
    typedef void (*ResumeFunc)(Object);

#ifdef WINDOWS
    DWORD bytes = 0;
    ULONG_PTR udata = 0;
    OVERLAPPED* evt = 0;
    HANDLE handle = (HANDLE)self->handle;
#endif

    if (Coroutine__current != &Coroutine__main) {
        fprintf(stderr, "Io::Manager::poll() called by user coroutine");
        fflush(stderr);
        abort();
    }

#if defined(WINDOWS)
    GetQueuedCompletionStatus(handle, &bytes, &udata, &evt, INFINITE);
    ResumeFunc func = Object__dispatch((Object)udata, &resume_str);
    func((Object)udata);

#elif defined(DARWIN)
    struct kevent event;
    int res = kevent(self->handle, 0, 0, &event, 1, NULL);
    if (res < 0) {
        fprintf(stderr, "%d\n", errno);
        fflush(stderr);
        abort();
    } else if (res == 0) {
        return;
    }
    ResumeFunc func = Object__dispatch(event.udata, &resume_str);
    func((Object)event.udata);
#else
#endif
}
