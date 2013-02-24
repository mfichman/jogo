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

#ifndef JOGO_PRIMITIVES_H
#define JOGO_PRIMITIVES_H

#include <stdint.h>

typedef void* VoidPtr;

typedef int64_t Int;
typedef int64_t Bool;
typedef uint32_t Char; // Unicode code point
typedef uint8_t Byte;
typedef double Float;

typedef uint64_t U64;
typedef uint32_t U32;

typedef struct String* String;
typedef struct Object* Object;

Int Int__init();
String Int_str__g(Int self);
Int Int__add(Int self, Int other);
Int Int__sub(Int self, Int other);
Int Int__mul(Int self, Int other);
Int Int__div(Int self, Int other);
Int Int__neg(Int self);
Int Int__mod(Int self, Int other);
Int Int__compl(Int self);
Int Int_max(Int self, Int other);
Bool Int__less(Int self, Int other);
Bool Int__equal(Int self, Int other);

Float Float__init();
String Float_str__g(Float self);
Float Float__add(Float self, Float other);
Float Float__sub(Float self, Float other);
Float Float__mul(Float self, Float other);
Float Float__div(Float self, Float other);
Bool Float__less(Float self, Float other);
Bool Float__equal(Float self, Float other);

Bool Bool__init();
String Bool_str__g(Bool self);

Char Char__init();
String Char_str__g(Char self);
Bool Char__less(Char self, Char other);
Bool Char__equal(Char self, Char other);
Char Char__getutf8(Byte** begin, Byte* end);
void Char__pututf8(Char self, Byte** begin, Byte* end);

Byte Byte__init();

#endif
