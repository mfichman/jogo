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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

String Int_str__g(Int self) {
    // Converts an integer into a string, by first calculating the amount of
    // space needed for the string, and then copying the characters into the
    // string.
    Int length = 0;
    Int val = self;
    String ret = 0;
    Char *c = 0;

    if (self < 0) { length++; }
    while (val) { 
        val /= 10; 
        length++;
    }
    if (self == 0) {
        length++;
    }

    ret = malloc(sizeof(struct String) + length + 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = String__vtable;
    ret->_refcount = 1;
    ret->length = length;
    
    // Now copy over the characters for each decimal place
    c = ret->data + ret->length - 1;
    val = self < 0 ? -self : self;
    if (!val) {
        *c-- = '0';
    }
    while (val) {
        *c-- = (val % 10) + '0';
        val /= 10;
    }
    if (self < 0) { 
        *c-- = '-';
        val = -val;
    }
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

Int Int__init() {
    return 0;
}

Int Int__add(Int self, Int other) {
    return self + other;
}

Int Int__sub(Int self, Int other) {
    return self - other;
}

Int Int__mul(Int self, Int other) {
    return self * other;
}

Int Int__div(Int self, Int other) {
    return self / other;
}

Int Int__neg(Int self) {
    return -self;
}

Int Int__less(Int self, Int other) {
    return self < other;
}

Int Int__mod(Int self, Int other) {
    return self % other;
}

Int Int__compl(Int self) {
    return ~self;
}

Bool Int__equal(Int self, Int other) {
    return self == other;
}

Int Int__abs__g(Int self) {
    return self >= 0 ? self : -self;
}

Float Float__init() {
    return 0;
}

Float Float__add(Float self, Float other) {
    return self + other;
}

Float Float__sub(Float self, Float other) {
    return self - other;
}

Float Float__mul(Float self, Float other) {
    return self * other;
}

Float Float__div(Float self, Float other) {
    return self / other;
}

Float Float__exp__g(Float self) {
    return exp(self);
}

Float Float__ln__g(Float self) {
    return log(self);
}

Float Float__log__g(Float self) {
    return log10(self);
}

Float Float__sqrt__g(Float self) {
    return sqrt(self);
}

Float Float__ceil__g(Float self) {
    return ceil(self);
}

Float Float__floor__g(Float self) {
    return floor(self);
}

Float Float__abs__g(Float self) {
    return abs(self);
}

Float Float__sin__g(Float self) {
    return sin(self);
}

Float Float__cos(Float self) {
    return cos(self);
}

Float Float__tan(Float self) {
    return tan(self);
}

Float Float__arcsin(Float self) {
    return asin(self);
}

Float Float__arccos(Float self) {
    return acos(self);
}

Float Float__arctan(Float self) {
    return atan(self);
}

Float Float__sinh(Float self) {
    return sinh(self);
}

Float Float__cosh(Float self) {
    return cosh(self);
}

Float Float__tanh(Float self) {
    return tanh(self);
}

Bool Float__equal(Float self, Float other) {
    return self == other;
}

Bool Float__less(Float self, Float other) {
    return self < other;
}

String Float_str__g(Float self) {
    abort();
    return 0;
}

Char Char__init() {
    return '\0';
}

String Char_str__g(Char self) {
    // Returns the string representation of the character.
    
    String ret = malloc(sizeof(struct String) + 1 + 1); 
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = String__vtable;
    ret->_refcount = 1;
    ret->length = 1;
    ret->data[0] = self;
    return ret;
}

Bool Char__equal(Char self, Char other) {
    return self == other;
}


Bool Char__less(Char self, Char other) {
    return self < other;
}

Bool Char__is_alnum__g(Char self) {
    return isalnum(self);
}

Bool Char__is_alpha__g(Char self) {
    return isalpha(self);
}

Bool Char__is_ctrl__g(Char self) {
    return iscntrl(self);
}

Bool Char__is_digit__g(Char self) {
    return isdigit(self);
}

Bool Char__is_lower__g(Char self) {
    return islower(self);
}

Bool Char__is_upper__g(Char self) {
    return isupper(self);
}

Bool Char__is_print__g(Char self) {
    return isprint(self);
}

Bool Char__is_graph__g(Char self) {
    return isgraph(self);
}

Bool Char__is_xdigit__g(Char self) {
    return isxdigit(self);
}

Bool Char__lower__g(Char self) {
    return tolower(self);
}

Bool Char__upper__g(Char self) {
    return toupper(self);
}

Bool Bool__init() {
    return 0;
}

String Bool_str__g(Bool self) {
    // Returns the string representation of a Bool value, namely "true" or
    // "false".
    static struct String true_str = { String__vtable, 1, 4, "true" };
    static struct String false_str = { String__vtable, 1, 5, "false" };
    if (self) {
        true_str._refcount++;
        return &true_str;
    } else {
        false_str._refcount++;
        return &false_str;
    }
}

