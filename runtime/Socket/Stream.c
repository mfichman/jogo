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

#include "Socket/Stream.h"
#include "Io/Stream.h"
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
#include <fcntl.h>
#endif
#ifdef DARWIN
#include <sys/event.h>
#endif

void* memset(void* src, int value, size_t num);

void Socket_Stream_close(Socket_Stream self) {
    // Closes underlying I/O stream for the socket.
    Io_Stream_close(self->stream);    
}

void Socket_Stream_peer__s(Socket_Stream self, Socket_Addr addr) {
    // Connects this socket to the peer at the given address.  The connection
    // completion indication occurs at the time that the socket is first read
    // to or written from.
    if (self->stream && Socket_Addr__equals(self->addr, addr)) { return; }
    if (self->stream) {
        Io_Stream_close(self->stream);
        Object__refcount_dec((Object)self->stream);
        self->stream = 0;
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
    sin.sin_addr.s_addr = htonl(addr->ip);
    sin.sin_port = htons(addr->port);

    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        fprintf(stderr, "fcntl() failed\n");
        fflush(stderr);
        abort();
    }

    int ret = connect(sd, (struct sockaddr *)&sin, sizeof(sin));
    if (ret < 0 && errno != EINPROGRESS) {
        fprintf(stderr, "connect() failed\n");
        fflush(stderr);
        abort();
    }

    self->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    Io_Stream_mode__s(self->stream, Io_StreamMode_ASYNC);
    Object__refcount_inc((Object)self->stream); 
} 

