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

#ifndef APOLLO_FILE_STREAM_H
#define APOLLO_FILE_STREAM_H

#include "../Primitives.h"
#include "../Buffer.h"

typedef struct File_Stream* File_Stream;
struct File_Stream {
    Ptr _vtable;
    U64 _refcount;
    Int handle;
    Buffer read_buf;
    Buffer write_buf;
    Int flags;
};

File_Stream File_Stream__init(Int handle);
void File_Stream_read(File_Stream self, Buffer buffer);
void File_Stream_write(File_Stream self, Buffer buffer);
Char File_Stream_get(File_Stream self);
void File_Stream_put(File_Stream self, Char ch);
String File_Stream_scan(File_Stream self, Char ch);
void File_Stream_print(File_Stream self, String str);
void File_Stream_flush(File_Stream self);
void File_Stream_close(File_Stream self);

#endif
