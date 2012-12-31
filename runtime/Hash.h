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

#ifndef JOGO_HASH_H
#define JOGO_HASH_H

#include "Primitives.h"
#include "Pair.h"

typedef struct HashBucket {
    Object key;
    Object value;
} HashBucket;

typedef struct Hash* Hash;
struct Hash {
    Ptr _vtable;
    U64 _refcount;
    Int capacity;
    Int count;
    HashBucket* data;    
};

Hash Hash__init();
void Hash__destroy(Hash self);
Object Hash__index(Hash self, Object key);
void Hash__insert(Hash self, Object key, Object value);
void Hash_rehash(Hash self);
Int Hash_hash(Hash self, Object obj);
Bool Hash_equal(Hash self, Object first, Object second);
extern void Hash__vtable();

typedef struct HashIter* HashIter;
struct HashIter {
    Ptr _vtable;
    U64 _refcount;
    Hash hash;
    Int index;
};

Pair HashIter_next(HashIter self);
Bool HashIter_more__g(HashIter self);

typedef struct HashValueIter* HashValueIter;
struct HashValueIter {
    Ptr _vtable;
    U64 _refcount;
    Hash hash;
    Int index;
};

Object HashValueIter_next(HashValueIter self);
Bool HashValueIter_more__g(HashValueIter self);

typedef struct HashKeyIter* HashKeyIter;
struct HashKeyIter {
    Ptr _vtable;
    U64 _refcount;
    Hash hash;
    Int index;
};

Object HashKeyIter_next(HashKeyIter self);
Bool HashKeyIter_more__g(HashKeyIter self);

#endif
