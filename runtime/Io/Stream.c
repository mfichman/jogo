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

#include "Io/Stream.h"
#include "String.h"
#ifndef WINDOWS
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>

Io_Stream Io_Stream__init(Int desc) {
    // Initializes a new stream.
    Io_Stream ret = calloc(sizeof(struct Io_Stream), 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Io_Stream__vtable;
    ret->_refcount = 1;
    ret->handle = desc; 
    ret->read_buf = Io_Buffer__init(1024);
    ret->write_buf = Io_Buffer__init(1024);
    ret->status = Io_StreamStatus_OK;
    ret->mode = Io_StreamMode_BLOCKING;

#ifdef WINDOWS 
    // Associate the handle with an IO completion port.
    //CreateIoCompletionPort(ret->handle, Kernel_manager()->iocp, ret, 0);
#endif

    return ret;
}

void Io_Stream_read(Io_Stream self, Io_Buffer buffer) {
    // Read characters from the stream until 'buffer' is full, an I/O error 
    // is raised, or the end of the input is reached.

    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->capacity - buffer->end;
#ifdef WINDOWS
    OVERLAPPED* evt = &self->overlapped;
    DWORD read = 0;
    HANDLE handle = (HANDLE)self->handle;
    Int ret = 0;

    // Read from the file, async or syncronously
    if (!ReadFile(handle, buf, len, &read, evt)) {
        if (ERROR_IO_PENDING == GetLastError()) {
            if (Io_StreamMode_BLOCKING != self->mode) {
                // Yield to the event manager.  FIXME: Return immediately if
                // the mode is set to ASYNC.
                // Coroutine__yield_main();
            }
            SetLastError(ERROR_SUCCESS);
            GetOverlappedResult(handle, evt, &read, 1);
        }
        if (ERROR_HANDLE_EOF == GetLastError()) {
            // End-of-file has been reached.
            self->status = Io_StreamStatus_EOF;
            return;
        } else if (ERROR_SUCCESS != GetLastError()) {
            // Some other error; set the error code.
            self->status = Io_StreamStatus_ERROR;
            return;
        }
    }
    self->overlapped.Offset += read;
    buffer->end += read;
#else
    Int ret = read(self->handle, buf, len);
    if (ret == 0) {
        self->status = Io_StreamStatus_EOF;
    } else if (ret == -1) {
        self->status = Io_StreamStatus_ERROR;
    } else {
        buffer->end += ret;
    }
#endif
}

void Io_Stream_write(Io_Stream self, Io_Buffer buffer) {
    // Write characters from the buffer into the stream until 'buffer' is full,
    // an I/O error is raised, or the stream is closed.

    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->end - buffer->begin;
#ifdef WINDOWS
    OVERLAPPED* evt = &self->overlapped;
    DWORD written = 0;
    HANDLE handle = (HANDLE)self->handle;

    // Write to the file, async or synchronously
    if (!WriteFile(handle, buf, len, &written, evt)) {
        if (ERROR_IO_PENDING == GetLastError()) {
            if (Io_StreamMode_BLOCKING != self->mode) {
                // Yield to the event manager.  FIXME: Return immediately if
                // the mode is set to ASYNC.
                // Coroutine__yield_main();
            }
            SetLastError(ERROR_SUCCESS);
            GetOverlappedResult(handle, evt, &written, 1);
        }
        if (ERROR_SUCCESS != GetLastError()) {
            // Set the error flag
            self->status = Io_StreamStatus_ERROR;
            return;
        }
    }
    self->overlapped.Offset += written; 
    buffer->begin += written;
#else
    Int ret = write(self->handle, buf, len);
    if (ret == 0) {
        return;
    } else if (ret == -1) {
        return;
    } else {
        buffer->begin += ret;
    }
#endif
}

Int Io_Stream_get(Io_Stream self) {
    // Read a single character from the stream.  Returns EOF if the end of the
    // stream has been reached.  If an error occurs while reading from the
    // stream, then 'status' will be set to 'ERROR.'

    Io_Buffer buf = self->read_buf;
    if (buf->begin == buf->end) {
        buf->begin = 0;
        buf->end = 0;
        Io_Stream_read(self, buf);
    }
    if (buf->begin >= buf->end) {
        // EOF
        return -1;
    } else {
        return buf->data[buf->begin++];
    }
}

Int Io_Stream_peek(Io_Stream self) {
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
        return -1;
    } else {
        return buf->data[buf->begin];
    }
}

void Io_Stream_put(Io_Stream self, Char ch) {
    // Insert a single character into the stream.  If an error occurs while 
    // writing to the stream, then 'status' will be set to 'ERROR.'

    Io_Buffer buf = self->write_buf;
    if (buf->end == buf->capacity) {
        Io_Stream_flush(self);
    }
    if (buf->end >= buf->capacity) {
        return;
    } else {
        buf->data[buf->end++] = ch;
    }
}

String Io_Stream_scan(Io_Stream self, String delim) {
    // Read in characters until one of the characters in 'delim' is found,
    // then return all the characters read so far.

    Int length = 16;
    String ret = malloc(sizeof(struct String) + length + 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = String__vtable;
    ret->_refcount = 1;
    ret->length = 0;

    while (1) {
        // Loop until we find a delimiter somewhere in the input stream
        Char next = Io_Stream_get(self);
        Char* c = 0;
        // Resize the string if necessary
        if (ret->length >= length) {
            String exp = 0;
            Char* c = 0;
            Int i = 0;
            length *= 2;
            exp = malloc(sizeof(struct String) + length + 1);
            if (!exp) {
                fprintf(stderr, "Out of memory");
                fflush(stderr);
                abort();
            }
            exp->_vtable = String__vtable;
            exp->_refcount = 1;
            exp->length = ret->length;
            c = exp->data;
            for (i = 0; i < ret->length; i++) {
                *c++ = ret->data[i];
            } 
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
    Io_Stream_flush(self);
}

void Io_Stream_flush(Io_Stream self) {
    // Forces all buffered characters to be written to the stream.
    while (self->write_buf->end != self->write_buf->begin) {
        Io_Stream_write(self, self->write_buf); 
    }
    self->write_buf->begin = 0;
    self->write_buf->end = 0;
}

void Io_Stream_close(Io_Stream self) {
    // Forces all buffered characters to be written to the stream, and closes
    // the stream.

    Io_Stream_flush(self);
    self->write_buf->begin = self->write_buf->end = 0;
    self->read_buf->begin = self->read_buf->end = 0;
#ifdef WINDOWS
    CloseHandle((HANDLE)self->handle);
#else
    close(self->handle);
#endif
}

