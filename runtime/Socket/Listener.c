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

#include "Socket/Listener.h"
#include "Io/Manager.h"
#include "Object.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef WINDOWS
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#endif
#ifdef DARWIN
#include <sys/event.h>
#endif

void* memset(void* src, int value, size_t num);

Socket_Stream Socket_Listener_accept(Socket_Listener self) {
    // Waits for a client to connect, then returns a pointer to the established
    // connection.

#if defined(WINDOWS)
#elif defined(DARWIN)
    struct kevent ev;
    Int kqfd = Io_manager()->handle;  
    Int fd = self->handle;
    Int flags = EV_ADD|EV_ONESHOT;
    EV_SET(&ev, fd, EVFILT_READ, flags, 0, 0, self);
    int ret = kevent(kqfd, &ev, 1, 0, 0, 0);
    if (ret < 0) {
        fprintf(stderr, "kevent: %d\n", errno);
        fflush(stderr);
        abort();
    }

    assert(!self->coroutine);
    self->coroutine = Coroutine__current;
    Object__refcount_inc((Object)self->coroutine);
    Io_manager()->waiting++;
    Coroutine__swap(Coroutine__current, &Coroutine__main);
    Io_manager()->waiting--;
    Object__refcount_dec((Object)self->coroutine);
    self->coroutine = 0;
#else
#endif

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    int sd = accept(self->handle, (struct sockaddr*)&sin, &len);
    if (sd < 0) {
        fprintf(stderr, "accept() failed\n");
        fflush(stderr);
        abort();
    }  

    Socket_Stream stream = Socket_Stream__init();
    stream->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    return stream;
}

void Socket_Listener_addr__s(Socket_Listener self, Socket_Addr addr) {
    // Listens for connections using the given address.  If the socket fails to
    // open, or the socket cannot listen on the specified port, or the socket
    // is unable to bind to a port, then the status of the socket listener will
    // be updated to indicate the error code.
    if (self->handle && Socket_Addr__equals(self->addr, addr)) { return; }
    if (self->handle) {
        close(self->handle);
        self->handle = 0;
    }
    if (!addr) { return; }
    
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) { 
        fprintf(stderr, "socket() failed\n");
        fflush(stderr);
        abort();
    }
    
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(addr->ip);
    sin.sin_port = htons(addr->port);

    self->handle = sd;
    Socket_Listener_reuse_addr__s(self, 1);
    
    if (bind(sd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "bind() failed\n");
        fflush(stderr);
        abort();
    } 

    if (listen(sd, self->backlog) < 0) {
        fprintf(stderr, "listen() failed\n");
        fflush(stderr);
        abort();
    } 

    Object__refcount_dec((Object)self->addr);
    self->addr = addr;
    Object__refcount_inc((Object)self->addr);
}

void Socket_Listener_close(Socket_Listener self) {
    close(self->handle);
    self->handle = 0;
}

void Socket_Listener_resume(Socket_Listener self) {
    assert(self->coroutine);
    Coroutine__swap(Coroutine__current, self->coroutine);
}

void Socket_Listener_reuse_addr__s(Socket_Listener self, Bool flag) {
    if (!self->handle) {
        return;
    } 
    Int sd = self->handle;

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
        fprintf(stderr, "setsockopt() failed\n");
        fflush(stderr);
        abort();
    }
}
