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

#include "Stream.h"
#include "../String.h"
#ifndef WINDOWS
#include <unistd.h>
#endif

void File_Stream_read(File_Stream self, Buffer buffer) {
#ifndef WINDOWS
    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->capacity - buffer->end;
    Int ret = read(self->handle, buf, len);
    if (ret == 0) {
        // EOF
        return;
    } else if (ret == -1) {
        // ERROR
        return;
    } else {
        buffer->end += ret;
    }
#endif
}

void File_Stream_write(File_Stream self, Buffer buffer) {
#ifndef WINDOWS
    Byte* buf = buffer->data + buffer->begin;
    Int len = buffer->end - buffer->begin;
    Int ret = write(self->handle, buf, len);
    if (ret == 0) {
        return;
    } else if (ret == -1) {
        // ERROR
        return;
    } else {
        buffer->begin += ret;
    }
#endif
}

Char File_Stream_get(File_Stream self) {
    Buffer buf = self->read_buf;
    if (buf->begin == buf->end) {
        buf->begin = 0;
        buf->end = 0;
        File_Stream_read(self, buf);
    }
    if (buf->begin >= buf->end) {
        return 0;
    } else {
        return buf->data[buf->begin++];
    }
}

void File_Stream_put(File_Stream self, Char ch) {
    Buffer buf = self->write_buf;
    if (buf->end == buf->capacity) {
        File_Stream_write(self, buf);
    }
    if (buf->end >= buf->capacity) {
        return;
    } else {
        buf->data[buf->end++] = ch;
    }
}

String File_Stream_scan(File_Stream self, Char ch) {
    return 0;
}

void File_Stream_print(File_Stream self, String str) {
    for (Int i = 0; i < str->length; i++) {
        File_Stream_put(self, str->data[i]);
    }
}

void File_Stream_flush(File_Stream self) {
    File_Stream_write(self, self->write_buf); 
}

void File_Stream_close(File_Stream self) {
    File_Stream_flush(self);
#ifndef WINDOWS
    Int ret = close(self->handle);
    if (ret == -1) {
        // Error
    }
#endif 
}

