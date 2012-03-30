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

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#endif

#include "Socket/Listener.h"
#include "Io/Manager.h"
#include "Object.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
extern void* memset(void*, int, size_t);
#endif
#ifdef DARWIN
#include <sys/event.h>
#endif

Socket_Stream Socket_Listener_accept(Socket_Listener self) {
    // Waits for a client to connect, then returns a pointer to the established
    // connection.  The code below is a bit tricky, because Windows expects the
    // call to accept() to happen before the I/O event can be triggered.  For
    // Unix systems, the wait happens first, and then accept() is used to
    // receive the incoming socket afterwards.
    Socket_Stream stream = 0;

#if defined(WINDOWS)
    // Begin the call to accept() by creating a new socket and issuing a call 
    // to AcceptEx.
    char buffer[(sizeof(struct sockaddr_in)+16)*2];
    DWORD socklen = sizeof(struct sockaddr_in)+16;
    DWORD read = 0;
    DWORD code = SIO_GET_EXTENSION_FUNCTION_POINTER;
    GUID guid = WSAID_ACCEPTEX;
    LPFN_ACCEPTEX AcceptEx = 0;
    DWORD bytes = 0;
    DWORD len = sizeof(AcceptEx);
    Io_Overlapped op;
    OVERLAPPED* evt = &op.overlapped;

    // Create a new socket for AcceptEx to use when a peer connects.
    SOCKET ls = self->handle;
    SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        fprintf(stderr, "socket() failed\n");
        fflush(stderr);
        abort();
    }

    // Get a pointer to the AcceptEx() function.
    WSAIoctl(sd, code, &guid, sizeof(guid), &AcceptEx, len, &bytes, 0, 0);
    
    // Initialize the OVERLAPPED structure that contains the user I/O data used
    // to resume the coroutine when AcceptEx completes. 
    memset(&op, 0, sizeof(op));
    op.coroutine = Coroutine__current;

    // Now call ConnectEx to begin accepting peer connections.  The call will
    // return immediately, allowing this function to yield to the I/O manager.
    if (!AcceptEx(ls, sd, buffer, 0, socklen, socklen, &read, evt)) {
        while (ERROR_IO_PENDING == GetLastError()) {
            // Wait for the I/O manager to yield after polling the I/O
            // completion port, and then get the result.
            Coroutine__iowait();
            SetLastError(ERROR_SUCCESS);
            GetOverlappedResult((HANDLE)sd, evt, &bytes, 1);
        } 
        if (ERROR_SUCCESS != GetLastError()) {
           fprintf(stderr, "%d\n", GetLastError());
           fprintf(stderr, "AcceptEx() failed\n");
           fflush(stderr);
           abort();
        }
    }

#elif defined(DARWIN)
    // Register to wait for a READ event, which signals that we can call 
    // accept() without blocking.
    struct kevent ev;
    int kqfd = Io_manager()->handle;  
    int fd = self->handle;
    int flags = EV_ADD|EV_ONESHOT;
    EV_SET(&ev, fd, EVFILT_READ, flags, 0, 0, Coroutine__current);
    int ret = kevent(kqfd, &ev, 1, 0, 0, 0);
    if (ret < 0) {
        fprintf(stderr, "kevent() failed %d\n", errno);
        fflush(stderr);
        abort();
    }
#else
#endif

#ifndef WINDOWS
    // Wait until the socket becomes readable.  At that point, there will be
    // a peer waiting in the accept queue.
    Coroutine__iowait();

    // Accept the peer, and create a new stream socket.
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    int sd = accept(self->handle, (struct sockaddr*)&sin, &len);
    if (sd < 0) {
        fprintf(stderr, "accept() failed\n");
        fflush(stderr);
        abort();
    }  
#endif

    stream = Socket_Stream__init();
    stream->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    return stream;
}

void Socket_Listener_addr__s(Socket_Listener self, Socket_Addr addr) {
    // Listens for connections using the given address.  If the socket fails to
    // open, or the socket cannot listen on the specified port, or the socket
    // is unable to bind to a port, then the status of the socket listener will
    // be updated to indicate the error code.
    struct sockaddr_in sin;
#ifdef WINDOWS
    HANDLE iocp = (HANDLE)Io_manager()->handle;
#endif

    if (self->handle && Socket_Addr__equals(self->addr, addr)) { return; }
    if (self->handle) {
        close(self->handle);
        self->handle = 0;
    }
    if (!addr) { return; }
    
    self->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (self->handle < 0) { 
        fprintf(stderr, "socket() failed\n");
        fflush(stderr);
        abort();
    }
    
#ifdef WINDOWS
    CreateIoCompletionPort((HANDLE)self->handle, iocp, 0, 0); 
#endif
    Socket_Listener_reuse_addr__s(self, 1);
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(addr->port);

    if (bind(self->handle, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "bind() failed\n");
        fflush(stderr);
        abort();
    } 

    if (listen(self->handle, self->backlog) < 0) {
        fprintf(stderr, "listen() failed\n");
        fflush(stderr);
        abort();
    } 

    Object__refcount_dec((Object)self->addr);
    self->addr = addr;
    Object__refcount_inc((Object)self->addr);
}

void Socket_Listener_close(Socket_Listener self) {
#ifdef WINDOWS
    CloseHandle((HANDLE)self->handle);
#else
    close(self->handle);
#endif
    self->handle = 0;
}

void Socket_Listener_reuse_addr__s(Socket_Listener self, Bool flag) {
    Int sd = self->handle;
    int len = sizeof(flag);
    if (!sd) { return; } 
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, len) < 0) {
        fprintf(stderr, "setsockopt() failed\n");
        fflush(stderr);
        abort();
    }
}
