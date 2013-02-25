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

#include "Primitives.h"
#include "String.h"
#include "Utf8/Utf8.h"
#include <assert.h>

Bool is_utf8_start(Byte self) {
    Byte const mask = 0xc0;
    return (self & mask) != 0x80;
}

Char get_utf8(Byte** begin, Byte* end) {
    // Returns the next Unicode code point starting at *begin.  Increments
    // *begin to point at the next character after the Unicode code point that
    // was read.
    static const U32 offset[6] = {
        0x00000000UL, 0x00003080UL, 0x000E2080UL,
        0x03C82080UL, 0xFA082080UL, 0x82082080UL
    };
    U32 ch = 0;
    U32 size = 0;
    do {
        if (*begin >= end) {
            // Expected another char, but got nothing.  Return the EOF marker.
            return -1;
        }
        ch <<= 6;
        ch += (U32)(*((*begin)++));
        size++;
    } while (!is_utf8_start(**begin));
    return ch-offset[size-1];
}

void put_utf8(Char ch, Byte** begin, Byte* end) {
    // Writes 'self' as a Unicode byte sequence to the buffer beginning at
    // '*begin' and ending at 'end'.  Increments *begin to the end of the byte
    // sequence written.
    if (ch < 0x80) { // 1 byte
        if (((*begin)+1) > end) { *begin = end; return; }
        (*begin)[0] = ch & 0x7f;
        *begin += 1;
    } else if (ch < 0x800) { // 2 bytes
        if (((*begin)+2) > end) { *begin = end; return; }
        (*begin)[0] = ((ch >> 6) & 0x1f) | 0xc0;
        (*begin)[1] = (ch & 0x3f) | 0x80;
        *begin += 2;
    } else if (ch < 0x10000) { // 3 bytes
        if (((*begin)+3) > end) { *begin = end; return; }
        (*begin)[0] = ((ch >> 12) & 0xf) | 0xe0;
        (*begin)[1] = ((ch >> 6) & 0x3f) | 0x80;
        (*begin)[2] = (ch & 0x3f) | 0x80;
        *begin += 3;
    } else if (ch < 0x200000) { // 4 bytes
        if (((*begin)+4) > end) { *begin = end; return; }
        (*begin)[0] = ((ch >> 18) & 0x7) | 0xf0;
        (*begin)[1] = ((ch >> 12) & 0x3f) | 0x80;
        (*begin)[2] = ((ch >> 6) & 0x3f) | 0x80;
        (*begin)[3] = (ch & 0x3f) | 0x80;
        *begin += 4;
    } else {
        assert(!"Invalid Unicode sequence");
    }
}

Int Utf8_len(String str) {
    struct Utf8_Iter i = { 0, 0, str, 0 };
    Int count = 0;
    while (Utf8_Iter_more__g(&i)) {
        Utf8_Iter_next(&i);
        ++count;
    }
    return count;
}

Bool Utf8_valid(String str) {
    assert(!"Not implemented");
    return 0;
}

Char Utf8_char(String str, Int index) {
    struct Utf8_Iter i = { 0, 0, str, 0 };
    Int count = 0;
    while (Utf8_Iter_more__g(&i)) {
        Char ret = Utf8_Iter_next(&i);
        if (count == index) {
            return ret;
        }
        ++count;
    }
    return 0;
}


Char Utf8_Iter_next(Utf8_Iter self) {
    Byte* data = self->string->data;
    Byte* start = data+self->index;
    Char ret = get_utf8(&start, data+self->string->length);       
    self->index = start-data;
    return ret;
}

