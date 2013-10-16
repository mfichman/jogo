/*
 * Copyright (c) 2013 Matt Fichman
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

#if defined(WINDOWS)
#include <winsock2.h>
#include <windows.h>
#elif defined(LINUX)
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#elif defined(DARWIN)
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Io/Stream.h"
#include "Io/Manager.h"
#include "Os/Os.h"
#include "Coroutine.h"
#include "String.h"
#include "Object.h"
#include "Boot/Boot.h"

Io_Stream Io_Stream__init(Int desc, Int type) {
    // Initializes a new stream.  Streams are used to write to/from files,
    // sockets, and other I/O devices.  Every attempt has been made to unify
    // all I/O via the Stream interface (like Unix file descriptors).
#ifdef WINDOWS
    HANDLE iocp = (HANDLE)Io_manager()->handle; 
#endif
    Io_Stream ret = Boot_calloc(sizeof(struct Io_Stream));
    ret->_vtable = Io_Stream__vtable;
    ret->_refcount = 1;
    ret->handle = desc; 
    ret->read_buf = Io_Buffer__init(1024);
    ret->write_buf = Io_Buffer__init(1024);
    ret->status = Io_StreamStatus_OK;
    ret->mode = Io_StreamMode_ASYNC;
    ret->type = type;

#if defined(WINDOWS)
    // Associate the handle with an IO completion port.  The Io::Manager
    // expects the completion key below to be a pointer to an Io::Stream
    // object with an attached coroutine that will be resumed when an I/O
    // operation is complete. 
    if (ret->type != Io_StreamType_CONSOLE) {
        CreateIoCompletionPort((HANDLE)ret->handle, iocp, 0, 0);
        ret->read_op.overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        ret->write_op.overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        // A manual reset event is used because ReadFile and WriteFile set 
        // the event to the nonsignalled state automatically
    }
#elif defined(LINUX) 
    Bool is_console = (Io_StreamType_CONSOLE == ret->type);
    Bool is_file = (Io_StreamType_FILE == ret->type);
    if (!is_console && !is_file) {
        Int epfd = Io_manager()->handle;
        struct epoll_event ev;
        ev.events = 0;
        ev.data.ptr = 0; 
        epoll_ctl(epfd, EPOLL_CTL_ADD, ret->handle, &ev);
    }
#endif
    return ret;
}

void Io_Stream_handle_console(Io_Stream self) {
    // Handle a console I/O event by enqueuing an I/O completion packet to the
    // I/O completion port. 
#ifdef WINDOWS
    Byte* buf = self->read_buf->data + self->read_buf->begin;
    Int len = self->read_buf->capacity - self->read_buf->end;
    DWORD read = 0;
    HANDLE iocp = (HANDLE)Io_manager()->handle;
    ULONG_PTR key = (ULONG_PTR)self;
    HANDLE handle = (HANDLE)self->handle;
    ReadFile(handle, buf, len, &read, 0);

    //if(!WaitForSingleObject(handle, INFINITE)) {
    //    printf("fail\n");
    //}
    self->read_buf->end += read;
    PostQueuedCompletionStatus(iocp, read, key, 0);
#endif
}

void Io_Stream_register_console(Io_Stream self) {
    // Completion ports cannot be used with Windows; instead, we have to wait
    // on another thread for the console I/O to unblock, and then post a
    // completion packet to the I/O completion port from there.
#ifdef WINDOWS
    LPTHREAD_START_ROUTINE cb = (LPTHREAD_START_ROUTINE)Io_Stream_handle_console;
    QueueUserWorkItem(cb, self, WT_EXECUTEINPERSISTENTTHREAD);
//    HANDLE wait = INVALID_HANDLE_VALUE;
//    HANDLE handle = (HANDLE)self->handle;
//    WAITORTIMERCALLBACK cb = (WAITORTIMERCALLBACK)Io_Stream_handle_console;
//    ULONG flags = WT_EXECUTEONLYONCE|WT_EXECUTEINPERSISTENTTHREAD;
//    RegisterWaitForSingleObject(&wait, handle, cb, self, INFINITE, flags);
//    return (Int)wait;
#endif
}   

Int Io_Stream_result(Io_Stream self, Int bytes, OVERLAPPED* op) {
    // Gets the result of an asycnchronous I/O operation, and returns the
    // number of bytes read/written.  Yields the current coroutine to the event
    // loop while waiting for an event to complete.  The coroutine should
    // schedule some kind of event (I/O, timer, etc.) before calling this
    // method.  Otherwise, the coroutine will be stuck waiting indefinitely.
    // Calling this method also increments the reference count, to retain the
    // coroutine so that it won't be collected while an I/O event is
    // outstanding. 
#ifdef WINDOWS
    HANDLE handle = (HANDLE)self->handle;
	Bool is_console = (Io_StreamType_CONSOLE == self->type);
	Bool has_io = (Io_StreamMode_ASYNC == self->mode) && !is_console;
	DWORD err = GetLastError();
    while (ERROR_IO_PENDING == GetLastError() || has_io) {
        // Yield to the event manager if async mode is enabled; otherwise,
        // block the entire process.
        if (Io_StreamMode_BLOCKING == self->mode) {	
			DWORD count = 0;
            SetLastError(ERROR_SUCCESS);
            GetOverlappedResult(handle, op, &count, 1);
			bytes = count;
        } else {
            Coroutine__iowait();
            bytes = Io_Manager_iobytes__g(Io_manager());
			has_io = 0;
        }
    }
    if (ERROR_HANDLE_EOF == GetLastError()) {
        // End-of-file has been reached.
        if (Io_Buffer_empty__g(self->read_buf)) {
            self->status = Io_StreamStatus_EOF;
        }
        return 0;
    } else if (ERROR_SUCCESS != GetLastError()) {
        // Some other error; set the error code.
        self->status = Io_StreamStatus_ERROR;
        self->error = GetLastError();
        return 0;
    } else if (bytes == 0) {
        if (Io_Buffer_empty__g(self->read_buf)) {
		    self->status = Io_StreamStatus_EOF;
        }
        return 0;
        return bytes;
    } else {
	    return bytes;
	}
#else
    return 0;
#endif
}

#ifdef WINDOWS
void Io_Stream_read(Io_Stream self, Io_Buffer buffer) {
    // Read characters from the stream until 'buffer' is full, an I/O error 
    // is raised, or the end of the input is reached (Windows impl)
    Byte* buf = buffer->data + buffer->end;
    Int len = buffer->capacity - buffer->end;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    HANDLE handle = (HANDLE)self->handle;
    DWORD read = 0;
    DWORD ret = 0;
    if (self->status == Io_StreamStatus_EOF) { return; }

    // Read from the file, async or syncronously
    if (is_blocking && !is_console) {
        // Set the low-order bit of the event if the handle is in blocking
        // mode.  This prevents a completion port notification from being 
        // queued for the I/O operation.
        (Int)(self->read_op.overlapped.hEvent) |= 0x1;
    } else if (!is_console) {
        (Int)(self->read_op.overlapped.hEvent) &= ~0x1;
    }

    self->read_op.coroutine = Coroutine__current;
    SetLastError(ERROR_SUCCESS);
    ret = ReadFile(handle, buf, len, &read, &self->read_op.overlapped);
	read = Io_Stream_result(self, read, &self->read_op.overlapped);
    self->read_op.overlapped.Offset += read;
    buffer->end += read;
}
#endif

#ifdef LINUX
void Io_Stream_read(Io_Stream self, Io_Buffer buffer) {
    // Read characters from the stream until 'buffer' is full, an I/O error 
    // is raised, or the end of the input is reached (Linux impl)
    Byte* buf = buffer->data + buffer->end;
    Int len = buffer->capacity - buffer->end;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    Bool is_file = (Io_StreamType_FILE == self->type);
    
    if (self->status == Io_StreamStatus_EOF) { return; }

    if(!is_blocking && !is_console && !is_file) {
        struct epoll_event ev;
        Int epfd = Io_manager()->handle;
        Int fd = self->handle; 
        ev.events = EPOLLIN|EPOLLONESHOT;      
        ev.data.ptr = Coroutine__current;
        Int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        if (ret < 0) {
            self->status = Io_StreamStatus_ERROR;
            self->error = errno;
            return; 
        }
        Coroutine__iowait();
    }

    Int ret = read(self->handle, buf, len);
    if (ret == 0) {
        if (Io_Buffer_empty__g(self->read_buf)) {
            self->status = Io_StreamStatus_EOF;
        }
    } else if (ret == -1) {
        self->status = Io_StreamStatus_ERROR;
        self->error = errno;
    } else {
        buffer->end += ret;
    }
}
#endif

#ifdef DARWIN
void Io_Stream_read(Io_Stream self, Io_Buffer buffer) {
    // Read characters from the stream until 'buffer' is full, an I/O error 
    // is raised, or the end of the input is reached (Darwin impl)
    Byte* buf = buffer->data + buffer->end;
    Int len = buffer->capacity - buffer->end;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    Bool is_file = (Io_StreamType_FILE == self->type);
    
    if (self->eof) { self->status = Io_StreamStatus_EOF; }
    // Kqueue will hang if we attempt to wait on the fd again.  However, we
    // can't set the EOF status until the user has read all the bytes from the
    // buffer.  So, we have an internal 'eof' flag that tracks when kqueue has
    // notified us that the stream is done.
    if (self->status == Io_StreamStatus_EOF) { return; }

    // Wait for the fd to be ready, and then do the read.
    if(!is_blocking && !is_console) {
        struct kevent ev;
        Int kqfd = Io_manager()->handle;
        Int fd = self->handle;
        Int flags = EV_ADD|EV_ONESHOT|EV_EOF;
        EV_SET(&ev, fd, EVFILT_READ, flags, 0, 0, Coroutine__current); 
        Int ret = kevent(kqfd, &ev, 1, 0, 0, 0);
        if (ret < 0) {
            self->error = errno;
            return;
        }
        Coroutine__iowait();
    }

    Int ret = read(self->handle, buf, len);
    if (ret == 0) {
        if (Io_Buffer_empty__g(self->read_buf)) {
            self->status = Io_StreamStatus_EOF;
        }
    } else if (ret == -1) {
        self->status = Io_StreamStatus_ERROR;
        self->error = errno;
    } else {
        buffer->end += ret;
        if (is_file && Io_manager()->iobytes <= len) {
            if (Io_Buffer_empty__g(self->read_buf)) {
                self->status = Io_StreamStatus_EOF;
            } else {
                self->eof = 1;
            }
        }
    }
}
#endif

#ifdef WINDOWS
void Io_Stream_write(Io_Stream self, Io_Buffer buffer) {
    // Write characters from the buffer into the stream until 'buffer' is full,
    // an I/O error is raised, or the stream is closed (Windows impl).
    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->end - buffer->begin;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    HANDLE handle = (HANDLE)self->handle;
    DWORD written = 0;
    DWORD ret = 0;
    // Write to the file, async or synchronously
    
    if (is_blocking && !is_console) {
        // Set the low-order bit of the event if the handle is in blocking
        // mode.  This prevents a completion port notification from being 
        // queued for the I/O operation.
        (Int)(self->write_op.overlapped.hEvent) |= 0x1;
    } else if (!is_console) {
        (Int)(self->write_op.overlapped.hEvent) &= ~0x1;
    }

    self->write_op.coroutine = Coroutine__current;
    SetLastError(ERROR_SUCCESS);
    ret = WriteFile(handle, buf, len, &written, &self->write_op.overlapped);
    written = Io_Stream_result(self, written, &self->write_op.overlapped);
    self->write_op.overlapped.Offset += written; 
    buffer->begin += written;
}
#endif

#ifdef LINUX
void Io_Stream_write(Io_Stream self, Io_Buffer buffer) {
    // Write characters from the buffer into the stream until 'buffer' is full,
    // an I/O error is raised, or the stream is closed (Linux impl).
    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->end - buffer->begin;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    Bool is_file = (Io_StreamType_CONSOLE == self->type);
    if(!is_blocking && !is_console && !is_file) {
        struct epoll_event ev;
        Int epfd = Io_manager()->handle;
        Int fd = self->handle; 
        ev.events = EPOLLOUT|EPOLLONESHOT;      
        ev.data.ptr = Coroutine__current;
        Int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        if (ret < 0) {
            self->status = Io_StreamStatus_ERROR;
            self->error = errno;
            return; 
        }
        Coroutine__iowait();
    }
    
    Int ret = write(self->handle, buf, len);
    if (ret == -1) {
        self->status = Io_StreamStatus_ERROR;
        self->error = errno;
    } else {
        buffer->begin += ret;
    }
}
#endif

#ifdef DARWIN
void Io_Stream_write(Io_Stream self, Io_Buffer buffer) {
    // Write characters from the buffer into the stream until 'buffer' is full,
    // an I/O error is raised, or the stream is closed (Darwin impl).
    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->end - buffer->begin;
    Bool is_blocking = (Io_StreamMode_BLOCKING == self->mode);
    Bool is_console = (Io_StreamType_CONSOLE == self->type);
    if(!is_blocking && !is_console) {
        struct kevent ev;
        Int kqfd = Io_manager()->handle;
        Int fd = self->handle;
        Int flags = EV_ADD|EV_ONESHOT|EV_EOF;
        EV_SET(&ev, fd, EVFILT_WRITE, flags, 0, 0, Coroutine__current); 
        Int ret = kevent(kqfd, &ev, 1, 0, 0, 0);
        if (ret < 0) {
            self->status = Io_StreamStatus_ERROR;
            self->error = errno;
            return;
        }
        Coroutine__iowait();
    }

    Int ret = write(self->handle, buf, len);
    if (ret == -1) {
        self->status = Io_StreamStatus_ERROR;
        self->error = errno;
    } else {
        buffer->begin += ret;
    }
}
#endif

Byte Io_Stream_get(Io_Stream self) {
    // Read a single byte from the stream.  Returns EOF if the end of the
    // stream has been reached.  If an error occurs while reading from the
    // stream, then 'status' will be set to 'ERROR.'
    return Io_Stream_getbb(self);
}

Byte Io_Stream_getbb(Io_Stream self) {
    Io_Buffer buf = self->read_buf;
    Io_Stream_fillto(self, sizeof(Byte));
    if (self->status != Io_StreamStatus_OK) {
        return 0xff;
    }
    return buf->data[buf->begin++];
}

Int Io_Stream_getib(Io_Stream self) {
    // Write an int to the stream in network-byte order
    Io_Buffer buf = self->read_buf;
    Int ret = 0;
    Io_Stream_fillto(self, sizeof(ret));
    if (self->status != Io_StreamStatus_OK) {
        return -1;
    }
    ret |= ((Int)buf->data[buf->begin+0]) << 0; // LSB first
    ret |= ((Int)buf->data[buf->begin+1]) << 8;
    ret |= ((Int)buf->data[buf->begin+2]) << 16;
    ret |= ((Int)buf->data[buf->begin+3]) << 24;
    ret |= ((Int)buf->data[buf->begin+4]) << 32;
    ret |= ((Int)buf->data[buf->begin+5]) << 40;
    ret |= ((Int)buf->data[buf->begin+6]) << 48;
    ret |= ((Int)buf->data[buf->begin+7]) << 56; // MSB last
    buf->begin += sizeof(ret);
    return ret;
}

Float Io_Stream_getfb(Io_Stream self) {
    // Write an int to the stream in network-byte order
    Io_Buffer buf = self->read_buf;
    Float ret = 0;
    Io_Stream_fillto(self, sizeof(ret));
    if (self->status != Io_StreamStatus_OK) {
        return -1;
    }
    ret = *(Float*)(buf->data+buf->begin);
    buf->begin += sizeof(ret);
    return ret;
}

String Io_Stream_getsb(Io_Stream self) {
    // Write a string to the stream 
    Io_Buffer buf = self->read_buf;
    Int len = Io_Stream_getib(self);
    String ret = 0;
    if (self->status != Io_StreamStatus_OK) {
        return 0;
    }
    Io_Stream_fillto(self, len);
    if (self->status != Io_StreamStatus_OK) {
        return 0;
    }
    ret = String_alloc(len);
    ret->length = len;
    Boot_memcpy(ret->data, buf->data+buf->begin, len);
    buf->begin += len;
    return ret;
}

Byte Io_Stream_peek(Io_Stream self) {
    // Return the next character that would be read from the stream.  Returns
    // EOF if the end of the file has been reached.  If an error occurs while
    // reading from the stream, then 'status' will be set to 'ERROR.'
    Io_Buffer buf = self->read_buf;
    if (buf->begin == buf->end) {
        buf->begin = 0;
        buf->end = 0;
        Io_Stream_read(self, buf);
    }

    if (buf->begin >= buf->end) {
        // EOF
        return 0xff;
    } else {
        return buf->data[buf->begin];
    }
}

void Io_Stream_put(Io_Stream self, Byte byte) {
    // Insert a single character into the stream.  If an error occurs while 
    // writing to the stream, then 'status' will be set to 'ERROR.'
    Io_Stream_putbb(self, byte);
}

void Io_Stream_putbb(Io_Stream self, Byte byte) {
    // Write a byte in binary format to the stream
    Io_Buffer buf = self->write_buf;
    Io_Stream_emptyto(self, sizeof(byte));
    if (self->status != Io_StreamStatus_OK) {
        return;
    }
    buf->data[buf->end++] = byte;
}

void Io_Stream_putib(Io_Stream self, Int integer) {
    // Write an int in binary format to the stream
    Io_Buffer buf = self->write_buf;
    Io_Stream_emptyto(self, sizeof(integer));
    if (self->status != Io_StreamStatus_OK) {
        return;
    }
    buf->data[buf->end+0] = integer >> 0; // LSB first
    buf->data[buf->end+1] = integer >> 8;
    buf->data[buf->end+2] = integer >> 16;
    buf->data[buf->end+3] = integer >> 24;
    buf->data[buf->end+4] = integer >> 32;
    buf->data[buf->end+5] = integer >> 40;
    buf->data[buf->end+6] = integer >> 48;
    buf->data[buf->end+7] = integer >> 56; // MSB last
    buf->end += sizeof(integer);
}

void Io_Stream_putfb(Io_Stream self, Float flt) {
    // Write an int in binary format to the stream
    Io_Buffer buf = self->write_buf;
    Io_Stream_emptyto(self, sizeof(flt));
    if (self->status != Io_StreamStatus_OK) {
        return;
    }
    *(Float*)(buf->data+buf->end) = flt;
    buf->end += sizeof(flt);
}

void Io_Stream_putsb(Io_Stream self, String str) {
    // Write a string in binary format to the stream
    Io_Buffer buf = self->write_buf;
    Io_Stream_putib(self, str->length);
    if (self->status != Io_StreamStatus_OK) {
        return;
    }
    Io_Stream_emptyto(self, str->length);
    if (self->status != Io_StreamStatus_OK) {
        return;
    }
    Boot_memcpy(buf->data+buf->end, str->data, str->length); 
    buf->end += str->length;
}

void Io_Stream_emptyto(Io_Stream self, Int num) {
    // Write until there are at least 'num' empty bytes in the buffer, or there
    // is an error.
    Io_Buffer buf = self->write_buf;
    assert(num <= buf->capacity && "Buffer is too small");
    if ((buf->capacity-buf->end) < num) {
        Io_Stream_flush(self);
    }
}

void Io_Stream_fillto(Io_Stream self, Int num) {
    // Read until there are at least 'num' bytes in the buffer, or the stream
    // reaches end-of-file.
    Io_Buffer buf = self->read_buf;
    assert(num <= buf->capacity && "Buffer is too small");
    if ((buf->end-buf->begin) < num) { 
        // Unicode char may overflow the buffer.  
        Io_Buffer_compact(buf);
    }
    if (buf->begin == buf->end) {
        buf->begin = 0;
        buf->end = 0;
        Io_Stream_read(self, buf);
    } else if ((buf->end-buf->begin) < num) {
        // Read in more data if available
        Io_Stream_read(self, buf);
    }
    if ((buf->end-buf->begin) < num) {
        self->status = Io_StreamStatus_EOF;
    }
}

String Io_Stream_scan(Io_Stream self, String delim) {
    // Read in characters until one of the characters in 'delim' is found,
    // then return all the characters read so far.

    Int length = 32;
    String ret = String_alloc(length);

    while (1) {
        // Loop until we find a delimiter somewhere in the input stream
        Byte next = Io_Stream_get(self);
        Byte* c = 0;
        // Resize the string if necessary
        if (ret->length >= length) {
            String exp = String_expand(ret, length*2);            
            Boot_free(ret);
            ret = exp;
            length = length*2;
        }
        if (next == 0xff) {
            ret->data[ret->length] = '\0';
            return ret;
        }
        for (c = delim->data; *c; ++c) {
            if (*c == next) {
                ret->data[ret->length] = '\0';
                return ret;
            }
        }
        // Append a new character
        ret->data[ret->length++] = next;
    } 
    return 0;
}

void Io_Stream_print(Io_Stream self, String str) {
    // Print the characters of 'str' to the stream.  If an error occurs while
    // writing 'str', then 'status' will be set to ERROR.

    Int i = 0;
    for (; i < str->length; i++) {
        Io_Stream_put(self, str->data[i]);
    }
}

void Io_Stream_pipe(Io_Stream self, Io_Stream input) {
    // Pipes all input from 'input' to 'self'.  If 'input' is null, does
    // nothing.
    if (!input) { return; }

    // Flush the receiver's output buffer first, so that output has the correct
    // ordering.
    Io_Stream_flush(self);
    
    // Read from 'input'; then write directly from the input buffer buffer
    // without doing any byte-copies.
    while (input->status == Io_StreamStatus_OK) {
        while (input->read_buf->end != input->read_buf->begin) {
            if (self->status == Io_StreamStatus_ERROR) {
                return;
            }
            Io_Stream_write(self, input->read_buf); 
        }
        input->read_buf->begin = 0;
        input->read_buf->end = 0;
        Io_Stream_read(input, input->read_buf);   
    }
}

void Io_Stream_flush(Io_Stream self) {
    // Forces all buffered characters to be written to the stream.
    while (self->write_buf->end != self->write_buf->begin) {
        if (self->status == Io_StreamStatus_ERROR) {
            return;
        }
        Io_Stream_write(self, self->write_buf); 
    }
    self->write_buf->begin = 0;
    self->write_buf->end = 0;
}

void Io_Stream_close(Io_Stream self) {
    // Forces all buffered characters to be written to the stream, and closes
    // the stream.  Also, clears the input buffer.

    Io_Stream_flush(self);
    self->read_buf->begin = 0;
    self->read_buf->end = 0;
#ifdef WINDOWS
    if (Io_StreamType_SOCKET == self->type) {
        closesocket((SOCKET)self->handle);
    } else {
        CloseHandle((HANDLE)self->handle);
    }
    CloseHandle((HANDLE)self->write_op.overlapped.hEvent);
    CloseHandle((HANDLE)self->read_op.overlapped.hEvent);
#else
    close(self->handle);
#endif
}

void Io_Stream_end(Io_Stream self) {
    // Writes the EOF if this is a socket stream.
    Io_Stream_flush(self);
    if (Io_StreamType_SOCKET == self->type) {
#ifdef WINDOWS
        if (shutdown((SOCKET)self->handle, SD_SEND)) {
            Boot_abort();
        }
#else
        shutdown(self->handle, SHUT_WR);
#endif
    }
}

String Io_Stream_readall(Io_Stream self) {
    Io_Buffer buf = self->read_buf;
    String ret = 0;
    // Read any leftover bytes that are still in the read_buf
    while (self->status == Io_StreamStatus_OK) {
		Int size = buf->end-buf->begin;
        if (buf->end == buf->capacity) {
            Io_Buffer tmp = Io_Buffer__init(buf->capacity * 2);
            Boot_memcpy(tmp->data, buf->data+buf->begin, buf->end-buf->begin);
            tmp->end = buf->end-buf->begin;
            if (buf != self->read_buf) {
                Boot_free(buf);
            }
            buf = tmp;
        }
        Io_Stream_read(self, buf);
		if (size == (buf->end-buf->begin)) {
			break;
		}
    }
    ret = String_alloc(buf->end);
    if (buf->end) {
        Boot_memcpy(ret->data, buf->data+buf->begin, buf->end-buf->begin);
        ret->length = buf->end-buf->begin;
    }
    if (buf != self->read_buf) {
        Boot_free(buf);
    }
	self->read_buf->begin = 0;
	self->read_buf->end = 0;
    return ret;
}
