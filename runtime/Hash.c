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

#include "Boot/Module.h"
#include "Hash.h"
#include "String.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>

#define HASH_LOAD_FACTOR 0.7 
#define HASH_GROWTH_FACTOR 2
#define HASH_DEFAULT_CAPACITY 17

Hash Hash__init() {
    Hash ret = Boot_calloc(sizeof(struct Hash));
    ret->_vtable = Hash__vtable;
    ret->_refcount = 1;
    ret->capacity = HASH_DEFAULT_CAPACITY;
    ret->data = Boot_calloc(sizeof(HashBucket)*ret->capacity);
    ret->count = 0;
    return ret; 
}

void Hash__destroy(Hash self) {
    Int i = 0;
    for (; i < self->count; ++i) {
        Object__refcount_dec(self->data[i].key);
        Object__refcount_dec(self->data[i].value);
    }
    Boot_free(self->data);
    Boot_free(self);
}

Object Hash__index(Hash self, Object key) {
    Int i = 0;
    HashBucket* entry = 0;
    if (!key) {
        return 0;
    }

    i = Hash_hash(self, key) % self->capacity;
    entry = self->data + i;
    while (entry->key) {
        if (Hash_equal(self, key, entry->key)) {
            Object__refcount_inc(entry->value);
            return entry->value;
        }
        i = (i + 1) % self->capacity;
        entry = self->data + i; 
    }
    return 0;
}

void Hash__insert(Hash self, Object key, Object value) {
    Int i = 0;
    HashBucket* entry = 0;
    if (!key) {
        return;
    }

    i = Hash_hash(self, key) % self->capacity;
    entry = self->data + i;
    while (entry->key) {
        if (Hash_equal(self, key, entry->key)) {
            if (!entry->value) {
                self->count++;
            }
            Object__refcount_dec(entry->key);
            Object__refcount_dec(entry->value);
            entry->key = key; 
            entry->value = value;
            Object__refcount_inc(entry->key);
            Object__refcount_inc(entry->value);
            if (!entry->value) {
                self->count--;
            }
            return;
        }
        i = (i + 1) % self->capacity;
        entry = self->data + i;
    }

    if (!value) {
        return;
    }

    entry->key = key;
    entry->value = value;
    Object__refcount_inc(entry->key);
    Object__refcount_inc(entry->value);
    self->count++;

    Hash_rehash(self);
}

void Hash_rehash(Hash self) {
    // Only re-hash the hash table if it is too small
    HashBucket* old_data = 0;
    Int old_capacity = 0;
    Int i = 0;
    if ((Float)self->count/(Float)self->capacity < HASH_LOAD_FACTOR) {
        return;
    }

    old_data = self->data;
    old_capacity = self->capacity;     
    self->capacity *= HASH_GROWTH_FACTOR;
    self->data = Boot_calloc(sizeof(HashBucket)*self->capacity);
    self->count = 0;

    for (i = 0; i < old_capacity; i++) {
        if (old_data[i].key) {
            Hash__insert(self, old_data[i].key, old_data[i].value);
        }
    }

    Boot_free(old_data);
}

Int Hash_hash(Hash self, Object key) {
    static struct String hash_str = String__static("hash?");
    typedef Int (*HashFunc)(Object);  

    HashFunc func = Object__dispatch(key, &hash_str);
    return func(key); 
}

Bool Hash_equal(Hash self, Object first, Object second) {
    static struct String equal_str = String__static("@equal");
    typedef Bool (*EqualFunc)(Object, Object);

    EqualFunc func = Object__dispatch(first, &equal_str);
    return func(first, second);
}

Pair HashIter_next(HashIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        self->index++;
        if (entry->key && entry->value) {
            return Pair__init(entry->key, entry->value);
        }
    }
    return 0;
}

Bool HashIter_more__g(HashIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        if (entry->key && entry->value) {
            return 1;
        }
        self->index++;
    }
    return 0;
}

Object HashValueIter_next(HashValueIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        self->index++;
        if (entry->key && entry->value) {
            Object__refcount_inc(entry->value);
            return entry->value;
        }
    }
    return 0;
}

Bool HashValueIter_more__g(HashValueIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        if (entry->key && entry->value) {
            return 1;
        }
        self->index++;
    }
    return 0;
}

Object HashKeyIter_next(HashKeyIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        self->index++;
        if (entry->key && entry->value) {
            Object__refcount_inc(entry->key);
            return entry->key;
        }
    }
    return 0;
}

Bool HashKeyIter_more__g(HashKeyIter self) {
    while (self->index < self->hash->capacity) {
        HashBucket* entry = self->hash->data + self->index;
        if (entry->key && entry->value) {
            return 1;
        }
        self->index++;
    }
    return 0;
}

