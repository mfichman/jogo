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
#include "Boot/Module.h"
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
    Io_Manager ret = Boot_calloc(sizeof(struct Io_Manager));
#ifdef WINDOWS
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) != 0) {
        Boot_abort();
    }
#endif
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
    HANDLE handle = (HANDLE)self->handle;
    DWORD bytes = 0;
    ULONG_PTR udata = 0;
    Io_Overlapped* op = 0;
    OVERLAPPED** evt = (OVERLAPPED**)&op;
    CloseHandle((HANDLE)self->handle);
    exit(0);
#else
    close(self->handle);
#endif
}

void Io_Manager__destroy(Io_Manager self) {
    Io_Manager_shutdown(self);
    Queue__destroy(self->scheduled);
    Boot_free(self);
#ifdef WINDOWS
    WSACleanup();
#endif
}

void Io_Manager_poll(Io_Manager self) {
    // Poll for an I/O event, and then resume the coroutine associated with
    // that event.
#ifdef WINDOWS
    HANDLE handle = (HANDLE)self->handle;
    DWORD bytes = 0;
    ULONG_PTR udata = 0;
    Io_Overlapped* op = 0;
    OVERLAPPED** evt = (OVERLAPPED**)&op;
#endif

    if (Coroutine__current != &Coroutine__main) {
        fprintf(stderr, "Io::Manager::poll() called by user coroutine");
        fflush(stderr);
        abort();
    }

#if defined(WINDOWS)
    SetLastError(ERROR_SUCCESS);
    self->iobytes = 0;
    GetQueuedCompletionStatus(handle, &bytes, &udata, evt, INFINITE);
    self->iobytes = bytes;
    Coroutine__ioresume(op->coroutine);
#elif defined(DARWIN)
    struct kevent event;
    int res = kevent(self->handle, 0, 0, &event, 1, NULL);
    self->iobytes = event.data;
    if (res < 0) {
        Boot_abort();
    } else if (res == 0) {
        return;
    }
    Coroutine__ioresume((Coroutine)event.udata);
#else
#endif
}
