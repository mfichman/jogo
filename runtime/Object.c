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

#include "Object.h"
#include "String.h"
#include <stdlib.h>

// This file provides refcount management and vtable lookup for objects.
// The vtable layout used by these functions is:
// 
// _vtable[0] = Destructor
// _vtable[1] = Table length..
// _vtable[2..n+1] = Jump table 1 (hash mixing values) 
// _vtable[n+2..2n+1] = Method pointers

void Object__refcount_inc(Object self) {
     // Increment the object's refcount;
    if (self) {
        self->_refcount++;
    }
}

void Object__refcount_dec(Object self) {
    // Decrement the object's refcount if the object is non-null.  Free the 
    // object if the refcount is below 0
    if (self) {
        self->_refcount--;
        if (self->_refcount <= 0) {
            // The second entry in the vtable is always the destructor. Call
            // the destructor and then release the memory.
            typedef void (*Destructor)(Object);  
            Destructor dtor = ((Ptr*)self->_vtable)[0];
            dtor(self);
        } 
    }
}

Ptr Object__dispatch(Object self, String id) {
    // Dispatch using the vtable to the correct method implementation.
    // FixMe: Investigate a more efficient way to do this; possibly using some
    // form of caching.  The method dispatch performance penalty is only
    // invoked when calling through an interface.
    U64 n = ((U64*)self->_vtable)[1];
    U64* jump1 = ((U64*)self->_vtable) + 2;
    Ptr* jump2 = ((Ptr*)self->_vtable) + 2 + n;
    U64 d = 0;
    U64 hash = 0;
    Char* c = 0;
    for (c = id->data; *c; ++c) {
        hash = ((hash * 0x01000193) ^ (*c)); 
    }

    d = jump1[hash % n];        
    for (c = id->data; *c; ++c) {
        d = ((d * 0x01000193) ^ (*c));
    }
    return jump2[d % n]; 
}

Int Object_hash__g(Object self) {
    // This function hashes each byte of the pointer value using the Java
    // String hash function.
    Char* array = (Char*)self;
    Int hash = 0;
    Int i = 0;
    for (; i < sizeof(Int); ++i) {
        hash = 31 * hash + array[i]; 
    }
    return hash;
}

Bool Object_same(Object self, Object other) {
    // This function returns true if the two pointers are equivalent.
    return self == other;
}
