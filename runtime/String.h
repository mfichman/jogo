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

#ifndef JOGO_STRING_H
#define JOGO_STRING_H

#include "Primitives.h"
#include <stdlib.h>

struct String {
    Ptr _vtable;
    U64 _refcount;
    Int length;
    Char data[];
};

String String_alloc(Int length);
Char String__index(String self, Int index);
String String__add(String self, String string);
String String_expand(String self, Int length);
String String_slice(String self, Int begin, Int end);
String String_prefix(String self, Int end);
String String_suffix(String self, Int begin);
Int String_length__g(String self);
Bool String__equal(String self, String string);
String String_uppercase__g(String self);
String String_lowercase__g(String self);
Int String_int__g(String self);
Float String_float_g(String self);
Int String_hash__g(String self);
extern Int String__vtable[];

// These functions are from the C standard library, duplicated here because
// String.h clashes with the C library string.h
int strerror_r(int, char*, size_t);
char* strncpy(char*, const char*, size_t);
size_t strlen(const char*);
void* memset(void*, int, size_t);
void* memcpy(void*, const void*, size_t);

#define String__static(x) { String__vtable, 1, sizeof((x))-1, (x) }
    

#endif
