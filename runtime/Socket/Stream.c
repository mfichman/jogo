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
// These need to come first to avoid conflicts with windows.h

#include "Socket/Stream.h"
#include "Io/Stream.h"
#include "Io/Manager.h"
#include "Os/Module.h"
#include "Object.h"
#include "String.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#if defined(WINDOWS)
#include <windows.h>
#elif defined(LINUX)
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#elif defined(DARWIN)
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#endif

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
    int sd = 0;
    assert(addr && "Invalid null argument");

    // Check to make sure that the socket isn't already connected to the given
    // address.  If it's connected to a different address, then close the 
    // existing file descriptor and reopen a new connection.
    if (self->stream && Socket_Addr__equals(&self->peer, addr)) { return; }
    if (self->stream) {
        Io_Stream_close(self->stream);
        Object__refcount_dec((Object)self->stream);
        self->stream = 0;
    }
    Socket_Addr__copy(&self->peer, addr);

    // Allocate a socket
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Call the Io::Stream constructor.  This will associate the socket with an
    // I/O completion port, which is necessary for the ConnectEx call.
    self->stream = Io_Stream__init(sd, Io_StreamType_SOCKET);
    Io_Stream_mode__s(self->stream, Io_StreamMode_ASYNC);
    if (sd < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = Os_error();
        return;
    }
    Socket_Stream_connect(self);
} 

#ifdef WINDOWS
void Socket_Stream_connect(Socket_Stream self) {
    // Windows version of connect() 

    // Initialize a bunch of Windows-specific crap needed to load a pointer to
    // the ConnectEx function...
    DWORD code = SIO_GET_EXTENSION_FUNCTION_POINTER;
    GUID guid = WSAID_CONNECTEX;
    LPFN_CONNECTEX ConnectEx = 0;
    DWORD bytes = 0; 
    DWORD len = sizeof(ConnectEx);
    Io_Overlapped op;
    OVERLAPPED* evt = &op.overlapped;
    SOCKET sd = (SOCKET)self->stream->handle;
    int ret = 0;
    struct sockaddr_in sin;

    // Get a pointer to the ConnectEx() function.  Sigh.  Windows.  This 
    // function never blocks, however, so we don't have to worry about I/O 
    // completion ports.
    WSAIoctl(sd, code, &guid, sizeof(guid), &ConnectEx, len, &bytes, 0, 0);

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
    if (bind(sd, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = GetLastError();
        return;
    }

    // Now call ConnectEx to begin connecting the socket.  The call will return
    // immediately, allowing this function to yield to the I/O manager.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(self->peer.ip);
    sin.sin_port = htons(self->peer.port);
    if (!ConnectEx(sd, (struct sockaddr*)&sin, sizeof(sin), 0, 0, 0, evt)) { 
        if (ERROR_IO_PENDING == GetLastError()) {
            // Wait for the I/O manager to yield after polling the I/O
            // completion port, and then get the result.
            Coroutine__iowait();
        } 
        if (ERROR_SUCCESS != GetLastError()) {
            self->stream->status = Io_StreamStatus_ERROR;
            self->stream->error = GetLastError();
        }
    }

    // The following setsockopt() call is needed when calling ConectEx.  From
    // the MSDN documentation: 
    //
    // When the ConnectEx function returns TRUE, the socket s is in the default
    // state for a connected socket. The socket s does not enable previously
    // set properties or options until SO_UPDATE_CONNECT_CONTEXT is set on the
    // socket. Use the setsockopt function to set the SO_UPDATE_CONNECT_CONTEXT
    // option.
    if (ERROR_SUCCESS == GetLastError()) {
       if (setsockopt(sd, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0)) {
           Boot_abort();
       }
    }
}
#endif

#ifdef LINUX
void Socket_Stream_connect(Socket_Stream self) {
    // Linux version of connect()
    int sd = (int)self->stream->handle;
    int ret = 0;
    struct sockaddr_in sin;

    // Set the socket in non-blocking mode, so that the call to connect() below
    // does not block.
    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    // Call connect() asynchronously.  The kqueue or epoll call will indicate
    // when the connection is complete.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(self->peer.ip);
    sin.sin_port = htons(self->peer.port);
    ret = connect(sd, (struct sockaddr*)&sin, sizeof(sin));
    if (ret < 0 && errno != EINPROGRESS) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    struct epoll_event ev;
    Int epfd = Io_manager()->handle;
    ev.events = EPOLLOUT|EPOLLERR|EPOLLONESHOT; 
    ev.data.ptr = Coroutine__current;
    ret = epoll_ctl(epfd, EPOLL_CTL_MOD, sd, &ev); 
    if (ret < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    // Need to do this to get the error code
    Coroutine__iowait();
    int res = 0;
    socklen_t len = sizeof(res);
    ret = getsockopt(sd, SOL_SOCKET, SO_ERROR, &res, &len);
    if (ret < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
    } else if(res) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = res;
    }
}
#endif

#ifdef DARWIN
void Socket_Stream_connect(Socket_Stream self) {
    // OS X version of connect
    int sd = (int)self->stream->handle;
    int ret = 0;
    struct sockaddr_in sin;

    // Set the socket in non-blocking mode, so that the call to connect() below
    // does not block.
    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    // Call connect() asynchronously.  The kqueue or epoll call will indicate
    // when the connection is complete.
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(self->peer.ip);
    sin.sin_port = htons(self->peer.port);
    ret = connect(sd, (struct sockaddr*)&sin, sizeof(sin));
    if (ret < 0 && errno != EINPROGRESS) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    struct kevent ev;
    Int kqfd = Io_manager()->handle;
    Int flags = EV_ADD|EV_ONESHOT|EV_EOF;
    EV_SET(&ev, sd, EVFILT_WRITE, flags, 0, 0, Coroutine__current); 
    ret = kevent(kqfd, &ev, 1, 0, 0, 0);
    if (ret < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
        return;
    }

    // Need to do this to get the error code
    Coroutine__iowait();
    ret = read(sd, 0, 0);
    if (ret < 0) {
        self->stream->status = Io_StreamStatus_ERROR;
        self->stream->error = errno;
    }
}
#endif
