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

String Float_str__g(Float self) {
    abort();
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
