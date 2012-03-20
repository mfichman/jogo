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
#include <mswsock.h>
#endif

#include "Socket/Stream.h"
#include "Io/Stream.h"
#include "Io/Manager.h"
#include "Object.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
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
    // to or written from.  This function is tricky because Windows expects
    // a call to ConnectEx before the wait() on the I/O completion port,
    // whereas the wait() happens before the call to connect() on Unix systems.
    int blocking = 1;
    int sd = 0;
    int ret = 0;
    struct sockaddr_in sin;
#ifdef WINDOWS
    // Initialize a bunch of Windows-specific crap needed to load a pointer to
    // the ConnectEx function...
    DWORD code = SIO_GET_EXTENSION_FUNCTION_POINTER;
    GUID guid = WSAID_CONNECTEX;
    LPFN_CONNECTEX ConnectEx = 0;
    DWORD bytes = 0; 
    DWORD len = sizeof(ConnectEx);
    Io_Overlapped op;
    OVERLAPPED* evt = &op.overlapped;
#endif

    // Check to make sure that the socket isn't already connected to the given
    // address.  If it's connected to a different address, then close the 
    // existing file descriptor and reopen a new connection.
    if (self->stream && Socket_Addr__equals(self->addr, addr)) { return; }
    if (self->stream) {
        Io_Stream_close(self->stream);
        Object__refcount_dec((Object)self->stream);
        self->stream = 0;
    }
    if (!addr) { return; }

    // Allocate a socket
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        fprintf(stderr, "socket() failed\n");
        fflush(stderr);
        abort();
    }
#ifdef WINDOWS
    // Get a pointer to the ConnectEx() function.  Sigh.  Windows.
    WSAIoctl(sd, code, &guid, sizeof(guid), &ConnectEx, len, &bytes, 0, 0);

    // Call the Io::Stream constructor.  This will associate the socket with an
    // I/O completion port, which is necessary for the ConnectEx call below.
    self->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    Io_Stream_mode__s(self->stream, Io_StreamMode_ASYNC);

    // Initialize the OVERLAPPED structure that contains the user I/O data used
    // to resume the coroutine when ConnectEx completes.
    memset(&op, 0, sizeof(op));
    op.coroutine = Coroutine__current;

    // To use ConnectEx, bind() must be called first to assign a socket a port
    // number.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = 0;
    if (bind(sd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "bind() failed\n");
        fflush(stderr);
        abort();
    }

    // Now call ConnectEx to begin connecting the socket.  The call will return
    // immediately, allowing this function to yield to the I/O manager.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(addr->ip);
    sin.sin_port = htons(addr->port);
    if (!ConnectEx(sd, (struct sockaddr*)&sin, sizeof(sin), 0, 0, 0, evt)) { 
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

#else
    self->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    Io_Stream_mode__s(self->stream, Io_StreamMode_ASYNC);

    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        fprintf(stderr, "fcntl() failed\n");
        fflush(stderr);
        abort();
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(addr->ip);
    sin.sin_port = htons(addr->port);
    ret = connect(sd, (struct sockaddr*)&sin, sizeof(sin));
    if (ret < 0 && errno != EINPROGRESS) {
        fprintf(stderr, "%d\n", WSAGetLastError());
        fprintf(stderr, "connect() failed\n");
        fflush(stderr);
        abort();
    }
#endif
} 

