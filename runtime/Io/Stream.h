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
#include "Io/Buffer.h"
#include "Io/Manager.h"

typedef struct Io_Stream* Io_Stream;
struct Io_Stream {
    VoidPtr _vtable;
    U64 _refcount;
    Int handle;
    Io_Buffer read_buf;
    Io_Buffer write_buf;
    Int status;
    Int mode;
    Int type;
    Int error;
#ifdef WINDOWS
    Io_Overlapped op;
#elif DARWIN
    Bool eof;
#endif
};

typedef Int Io_StreamMode;

extern Int Io_StreamStatus_OK;
extern Int Io_StreamStatus_ERROR;
extern Int Io_StreamStatus_EOF;

extern Int Io_StreamMode_ASYNC;
extern Int Io_StreamMode_SYNC;
extern Int Io_StreamMode_BLOCKING;

extern Int Io_StreamType_FILE;
extern Int Io_StreamType_CONSOLE;
extern Int Io_StreamType_SOCKET;

Io_Stream Io_Stream__init(Int handle, Int type);
void Io_Stream_read(Io_Stream self, Io_Buffer buffer);
void Io_Stream_write(Io_Stream self, Io_Buffer buffer);
Char Io_Stream_getc(Io_Stream self);
Char Io_Stream_peekc(Io_Stream self);
void Io_Stream_putc(Io_Stream self, Char ch);
Byte Io_Stream_getb(Io_Stream self);
Byte Io_Stream_peekb(Io_Stream self);
void Io_stream_putb(Io_Stream self, Byte byte);
String Io_Stream_scan(Io_Stream self, String delim);
void Io_Stream_print(Io_Stream self, String str);
void Io_Stream_pipe(Io_Stream self, Io_Stream input);
void Io_Stream_flush(Io_Stream self);
void Io_Stream_close(Io_Stream self);
void Io_Stream_end(Io_Stream self);
void Io_Stream_register_console(Io_Stream self);
void Io_Stream_resume(Io_Stream self);
void Io_Stream_mode__s(Io_Stream self, Io_StreamMode mode);
Int Io_Stream_result(Io_Stream self, Int bytes);
String Io_Stream_readall(Io_Stream self);
void Io_Stream__destroy(Io_Stream self);
Bool Io_Buffer_empty__g(Io_Buffer self);
extern void Io_Stream__vtable();

#endif
