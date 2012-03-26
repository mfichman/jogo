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

#include "String.h"
#include "Boot\Module.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

String String_alloc(Int length) {
    // Allocates a string with buffer size 'length+1', but with length zero.
    // The string has enough storage allocated to store 'length' characters.
    String ret = Boot_calloc(sizeof(struct String) + length + 1);
    ret->_vtable = String__vtable;
    ret->_refcount = 1;
    ret->length = 0;
    return ret;
}

Char String__index(String self, Int index) {
    // All index operations are checked.  If the index is off the end of the
    // string, then return the NUL character.
    if (index < 0) {
        return -index <= self->length ? self->data[self->length+index] : '\0'; 
    } else {
        return index < self->length ? self->data[index] : '\0';
    }
}

String String_expand(String self, Int length) {
    // Creates a copy of 'self', expending the underlying buffer to the maximum
    // of 'length' and 'self->length'.
    Int len = sizeof(struct String) + max(length, self->length) + 1;
    String ret = Boot_malloc(len);
    Char* c = ret->data;
    Int i = 0;
    ret->_vtable = String__vtable;
    ret->_refcount = 1;
    ret->length = self->length;
    for (i = 0; i < self->length; ++i) {
        *c++ = self->data[i];
    }
    ret->data[ret->length] = '\0';
    return ret;
}

String String__add(String self, String string) {
    // String data is allocated inline using the C "struct hack."  Since 
    // strings are immutable, the string array will never need to be resized,
    // and we get a performance boost from less indirection.

    // We have to manually initialize the string, because of the string 
    // optimization.  Copy the vtable from another string (FixMe: eventually
    // copy this from a static location)
    Int length = self->length + string->length;
    Char* c = 0;
    Int i = 0;

    String ret = Boot_malloc(sizeof(struct String) + length + 1); 
    ret->_vtable = self->_vtable;
    ret->_refcount = 1;
    ret->length = length;

    // Copy the data from the two strings.  Do this manually to take control
    // of security bugs. 
    c = ret->data;
    for (i = 0; i < self->length; ++i) {
        *c++ = self->data[i];
    }    
    for (i = 0; i < string->length; ++i) {
        *c++ = string->data[i];
    }
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret; 
}

String String_slice(String self, Int begin, Int end) {
    // String data is allocated inline using the C "struct hack."  Since 
    // strings are immutable, the string array will never need to be resized,
    // and we get a performance boost from less indirection.
    Int length = 0;
    String ret = 0;
    Char* c = 0;
    Int i = 0;

    end = (end > self->length) ? self->length : end;
    if (begin > end) { begin = end; }
    if (begin < 0) { begin = 0; }

    length = end - begin;
    ret = Boot_malloc(sizeof(struct String) + length + 1);
    ret->_vtable = self->_vtable;
    ret->_refcount = 1;
    ret->length = length; 
    
    // Copy the data from this string's substring, using range checking.
    c = ret->data;
    for (i = begin; i < end; ++i) {
        *c++ = self->data[i];
    } 
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

Int String_length__g(String self) {
    // Simply return the length data member.
    return self->length;
}

Bool String__equal(String self, String string) {
    // Compare the two strings, return true if they are equal.
    Int i = 0;

    if (self == string) {
        return 1;
    }
    if (self->length != string->length) {
        return 0;
    }
    for (i = 0; i < self->length; ++i) {
        if (self->data[i] != string->data[i]) {
            return 0;
        }
    }
    return 1;
}

String String_uppercase__g(String self) {
    // Create a new string with all lowercase letters replaced by uppercase
    Char *c = 0;
    Int i = 0;

    String ret = Boot_malloc(sizeof(struct String) + self->length + 1);
    ret->length = self->length;

    c = ret->data;
    for (i = 0; i < ret->length; ++i) {
        *c++ = toupper(self->data[i]);
    }

    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

String String_lowercase__g(String self) {
    // Create a new string with all uppercase letters replaced by lowercase
    Char* c = 0;
    Int i = 0;

    String ret = Boot_malloc(sizeof(struct String) + self->length + 1);
    ret->length = self->length;

    c = ret->data;
    for (i = 0; i < ret->length; ++i) {
        *c++ = tolower(self->data[i]);
    }
    
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

Int String_int__g(String self) {
    // Converts a base-10 representation of a string into an integer.
    Int ret = 0;
    Int neg = 0;
    Char* c = 0;
    if (self->length <= 0) {
        return 0;
    }
    for (c = self->data; *c; ++c) {
        if (*c == '-' && c == self->data) {
            neg = 1;
        } else if (isdigit(*c)) {
            ret = ret * 10 + *c - '0';
        } else {
            return 0;
        }
    }    
    return neg ? -ret : ret;
}

Int String_float__g(String self) {
    assert(!"Not supported");
    return 0;
}

Int String_hash__g(String self) {
    // Returns the hash code of this string.  This function uses the Java hash
    // code implementation, i.e., s[0]*31^(n-1) + s[1]*32^(n-2) ...
    Int hash = 0;
    Char* c = 0;
    for (c = self->data; *c; ++c) {
        hash = 31 * hash + (*c);
    }
    return hash;
}
