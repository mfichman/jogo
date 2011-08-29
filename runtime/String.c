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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void String__destroy(String* self) {
    printf("Destroying string!");
    fflush(stdout);
    free(self);
}

Char String__index(String* self, Int index) {
    // All index operations are checked.  If the index is off the end of the
    // string, then return the NUL character.
    return index < self->length ? self->data[index] : '\0';
}

String* String__add(String* self, String* string) {
    // String data is allocated inline using the C "struct hack."  Since 
    // strings are immutable, the string array will never need to be resized,
    // and we get a performance boost from less indirection.

    // We have to manually initialize the string, because of the string 
    // optimization.  Copy the vtable from another string (FixMe: eventually
    // copy this from a static location)
    String* ret = malloc(sizeof(String) + self->length + string->length + 1); 
    if (!ret) {
        printf("Out of memory");
        fflush(stdout);
        abort();
    }
    ret->_vtable = self->_vtable;
    ret->_refcount = 1;
    ret->length = self->length + string->length;

    // Copy the data from the two strings.  Do this manually to take control
    // of security bugs. 
    Char *c = ret->data;
    for (Int i = 0; i < self->length; i++) {
        *c++ = self->data[i];
    }    
    for (Int i = 0; i < string->length; i++) {
        *c++ = string->data[i];
    }
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret; 
}

String* String_slice(String* self, Int begin, Int end) {
    // String data is allocated inline using the C "struct hack."  Since 
    // strings are immutable, the string array will never need to be resized,
    // and we get a performance boost from less indirection.
    end = (end > self->length) ? self->length : end;
    if (begin > end) { begin = end; }
    if (begin < 0) { begin = 0; }

    Int length = end - begin;
    String* ret = malloc(sizeof(String) + length + 1);
    if (!ret) {
        printf("Out of memory");
        fflush(stdout);
        abort();
    }
    ret->_vtable = self->_vtable;
    ret->_refcount = 1;
    ret->length = length; 
    
    // Copy the data from this string's substring, using range checking.
    Char *c = ret->data;
    for (Int i = begin; i < end; i++) {
        *c++ = self->data[i];
    } 
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

Int String_length__g(String* self) {
    // Simply return the length data member.
    return self->length;
}

Bool String__equal(String* self, String* string) {
    // Compare the two strings, return true if they are equal.
    if (self == string) {
        return 1;
    }
    if (self->length != string->length) {
        return 0;
    }
    for (Int i = 0; i < self->length; i++) {
        if (self->data[i] != string->data[i]) {
            return 0;
        }
    }
    return 1;
}

String* String_uppercase__g(String* self) {
    // Create a new string with all lowercase letters replaced by uppercase
    String* ret = malloc(sizeof(String) + self->length + 1);
    ret->length = self->length;

    Char *c = ret->data;
    for (Int i = 0; i < ret->length; i++) {
        *c++ = toupper(self->data[i]);
    }

    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

String* String_lowercase__g(String* self) {
    // Create a new string with all uppercase letters replaced by lowercase
    String* ret = malloc(sizeof(String) + self->length + 1);
    ret->length = self->length;

    Char *c = ret->data;
    for (Int i = 0; i < ret->length; i++) {
        *c++ = tolower(self->data[i]);
    }
    
    ret->data[ret->length] = '\0'; // Add nul-terminator for C usage
    return ret;
}

String* Int_str__g(Int self) {
    // Converts an integer into a string, by first calculating the amount of
    // space needed for the string, and then copying the characters into the
    // string.
    Int length = 0;
    Int val = self;

    if (self < 0) { length++; }
    while (val) { 
        val /= 10; 
        length++;
    }

    String* ret = malloc(sizeof(String) + length + 1); 
    if (!ret) {
        printf("Out of memory");
        fflush(stdout);
        abort();
    }
    //ret->_vtable = String__vtable 
    ret->_vtable = 0;
    ret->_refcount = 1;
    ret->length = length;
    
    // Now copy over the characters for each decimal place
    Char *c = ret->data + ret->length - 1;
    val = self < 0 ? -self : self;
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

String* Float_str__g(Float self) {
    abort();
}

