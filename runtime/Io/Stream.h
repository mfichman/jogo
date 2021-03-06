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

#ifndef JOGO_IO_STREAM_H
#define JOGO_IO_STREAM_H

#include "Primitives.h"
#include "Coroutine.h"
#include "Buffer.h"
#include "Io/Manager.h"

typedef struct Io_Stream* Io_Stream;
struct Io_Stream {
    VoidPtr _vtable;
    U64 _refcount;
    Int handle;
    Buffer read_buf;
    Buffer write_buf;
    Int status;
    Int type;
    Int error;
    Int mode;
#ifdef WINDOWS
    Io_Overlapped read_op;
    Io_Overlapped write_op;
#elif defined(DARWIN)
    Bool eof;
#endif
};

typedef Int Io_StreamMode;

extern Int Io_StreamStatus_OK;
extern Int Io_StreamStatus_ERROR;
extern Int Io_StreamStatus_EOF;

extern Int Io_StreamMode_ASYNC;
extern Int Io_StreamMode_BLOCKING;

extern Int Io_StreamType_FILE;
extern Int Io_StreamType_CONSOLE;
extern Int Io_StreamType_SOCKET;

Io_Stream Io_Stream__init(Int handle, Int type);
void Io_Stream_read(Io_Stream self, Buffer buffer);
void Io_Stream_write(Io_Stream self, Buffer buffer);
Byte Io_Stream_get(Io_Stream self);
Byte Io_Stream_getbb(Io_Stream self);
Int Io_Stream_getib(Io_Stream self);
Float Io_Stream_getfb(Io_Stream self);
String Io_Stream_getsb(Io_Stream self);
Byte Io_Stream_peek(Io_Stream self);
void Io_Stream_put(Io_Stream self, Byte byte);
void Io_Stream_putbb(Io_Stream self, Byte byte);
void Io_Stream_putib(Io_Stream self, Int integer);
void Io_Stream_putfb(Io_Stream self, Float flt);
void Io_Stream_putsb(Io_Stream self, String str);
String Io_Stream_scan(Io_Stream self, String delim);
void Io_Stream_print(Io_Stream self, String str);
void Io_Stream_pipe(Io_Stream self, Io_Stream input);
void Io_Stream_flush(Io_Stream self);
void Io_Stream_close(Io_Stream self);
void Io_Stream_end(Io_Stream self);
void Io_Stream_register_console(Io_Stream self);
void Io_Stream_mode__s(Io_Stream self, Io_StreamMode mode);
String Io_Stream_readall(Io_Stream self);
void Io_Stream__destroy(Io_Stream self);
void Io_Stream_fillto(Io_Stream self, Int num);
void Io_Stream_emptyto(Io_Stream self, Int num);
extern void Io_Stream__vtable();

#endif
