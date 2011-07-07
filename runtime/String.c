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

Char String__index(String* self, Int index) {
    // All index operations are checked.  If the index is off the end of the
    // string, then return the NUL character.
    return index < self->length ? self->data[index] : '\0';
}

String* String__plus(String* self, String* string) {
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
    for (int i = 0; i < self->length; i++) {
        *c++ = self->data[i];
    }    
    for (int i = 0; i < string->length; i++) {
        *c++ = self->data[i];
    }
    return ret; 
}

String* String_slice(String* self, Int begin, Int end) {
    // String data is allocated inline using the C "struct hack."  Since 
    // strings are immutable, the string array will never need to be resized,
    // and we get a performance boost from less indirection.
    end = (end >= self->length) ? self->length - 1 : end;
    begin = (begin > end) ? end : begin;

    Int length = end - begin;
    String* ret = malloc(sizeof(String) + length);
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
    for (int i = begin; i <= end; i++) {
        *c++ = self->data[i];
    } 
    return ret;
}

Int String__length__g(String* self) {
    // Simply return the length data member.
    return self->length;
}

void print(String* string) {
    // Write string to stdout.  This is function is here for convenience's 
    // sake.  Once a full-fledged IO framework has been written, this function
    // will really only be useful for simple output.
    fwrite(string->data, 1, string->length, stdout);
    fflush(stdout);
}

/*
String* String__slice(Range range) {

}
*/
