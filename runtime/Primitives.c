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
#include "Boot/Module.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#ifdef WINDOWS
#define snprintf _snprintf
#endif

String Int_str__g(Int self) {
    // Converts an integer into a string, by first calculating the amount of
    // space needed for the string, and then copying the characters into the
    // string.
    Int length = 0;
    Int val = self;
    String ret = 0;
    Byte *c = 0;

    if (self < 0) { length++; }
    while (val) { 
        val /= 10; 
        length++;
    }
    if (self == 0) {
        length++;
    }

    ret = Boot_malloc(sizeof(struct String) + length + 1); 
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

Int Int_abs__g(Int self) {
    return self >= 0 ? self : -self;
}

Float Int_exp__g(Float self) {
    return exp(self);
}

Float Int_ln__g(Float self) {
    return log(self);
}

Float Int_log__g(Float self) {
    return log10(self);
}

Float Int_sqrt__g(Float self) {
    return sqrt(self);
}

Float Int_ceil__g(Float self) {
    return ceil(self);
}

Float Int_floor__g(Float self) {
    return floor(self);
}

Float Int_sin__g(Float self) {
    return sin(self);
}

Float Int_cos__g(Float self) {
    return cos(self);
}

Float Int_tan__g(Float self) {
    return tan(self);
}

Float Int_arcsin__g(Float self) {
    return asin(self);
}

Float Int_arccos__g(Float self) {
    return acos(self);
}

Float Int_arctan__g(Float self) {
    return atan(self);
}

Float Int_sinh__g(Float self) {
    return sinh(self);
}

Float Int_cosh__g(Float self) {
    return cosh(self);
}

Float Int_tanh__g(Float self) {
    return tanh(self);
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

Float Float_exp__g(Float self) {
    return exp(self);
}

Float Float_ln__g(Float self) {
    return log(self);
}

Float Float_log__g(Float self) {
    return log10(self);
}

Float Float_sqrt__g(Float self) {
    return sqrt(self);
}

Float Float_ceil__g(Float self) {
    return ceil(self);
}

Float Float_floor__g(Float self) {
    return floor(self);
}

Float Float_abs__g(Float self) {
    return abs(self);
}

Float Float_sin__g(Float self) {
    return sin(self);
}

Float Float_cos__g(Float self) {
    return cos(self);
}

Float Float_tan__g(Float self) {
    return tan(self);
}

Float Float_arcsin__g(Float self) {
    return asin(self);
}

Float Float_arccos__g(Float self) {
    return acos(self);
}

Float Float_arctan__g(Float self) {
    return atan(self);
}

Float Float_sinh__g(Float self) {
    return sinh(self);
}

Float Float_cosh__g(Float self) {
    return cosh(self);
}

Float Float_tanh__g(Float self) {
    return tanh(self);
}

Bool Float__equal(Float self, Float other) {
    return self == other;
}

Bool Float__less(Float self, Float other) {
    return self < other;
}

String Float_str__g(Float self) {
    String ret = String_alloc(64);
    snprintf(ret->data, 64, "%g", self); 
    ret->length = strlen(ret->data);
    return ret;
}

Char Char__init() {
    return '\0';
}

String Char_str__g(Char self) {
    // Returns the string representation of the character.
    
    String ret = String_alloc(2);
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

Bool Char_is_alnum__g(Char self) {
    return isalnum(self);
}

Bool Char_is_alpha__g(Char self) {
    return isalpha(self);
}

Bool Char_is_ctrl__g(Char self) {
    return iscntrl(self);
}

Bool Char_is_digit__g(Char self) {
    return isdigit(self);
}

Bool Char_is_lower__g(Char self) {
    return islower(self);
}

Bool Char_is_upper__g(Char self) {
    return isupper(self);
}

Bool Char_is_print__g(Char self) {
    return isprint(self);
}

Bool Char_is_graph__g(Char self) {
    return isgraph(self);
}

Bool Char_is_xdigit__g(Char self) {
    return isxdigit(self);
}

Bool Char_lower__g(Char self) {
    return tolower(self);
}

Bool Char_upper__g(Char self) {
    return toupper(self);
}

Bool Bool__init() {
    return 0;
}

String Bool_str__g(Bool self) {
    // Returns the string representation of a Bool value, namely "true" or
    // "false".
    static struct String true_str = String__static("true");
    static struct String false_str = String__static("false");
    if (self) {
        true_str._refcount++;
        return &true_str;
    } else {
        false_str._refcount++;
        return &false_str;
    }
}

