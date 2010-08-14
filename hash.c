/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include <hash.h>
#include <stdlib.h>
#include <string.h>

typedef struct hash_entry hash_entry_t;
struct hash_entry {
	const void *key;
	void *value;
	int present;
};

struct hash {
	hash_entry_t *entries;
	int capacity;
	int count;
	hash_compfn_t compare;
	hash_hashfn_t hash;
};
	
hash_t *hash_alloc(hash_compfn_t comp, hash_hashfn_t hash) {
	hash_t *self = malloc(sizeof(hash_t));
	
	self->capacity = 17;
	self->count = 0;
	self->compare = comp;
	self->hash = hash; 
	self->entries = calloc(self->capacity, sizeof(hash_entry_t));
	
	return self;
}

void hash_rehash(hash_t *self, int capacity) {
	hash_entry_t *old = self->entries;
	self->entries = calloc(capacity, sizeof(hash_entry_t));
	
	for (int i = 0; i < self->capacity; i++) {
		if (self->entries[i].present) {
			hash_put(self, self->entries[i].key, self->entries[i].value);
		}
	}

	free(old);
	self->capacity = capacity;
}

void *hash_put(hash_t *self, const void *key, void *value) {

	if (10 * (self->count + 1) > 8 * (self->capacity)) {
		hash_rehash(self, self->capacity * 2);	
	}

	int index = self->hash(key) % self->capacity;
	hash_entry_t *entry = &self->entries[index];
	while (entry->present) {
		if (!self->compare(entry->key, key)) {
			entry->key = key;
			entry->value = value;
			return value;
		}
	
		index = (index + 1) % self->capacity;
		entry = &self->entries[index];
	}

	entry->key = key;
	entry->value = value;
	entry->present = 1;
	self->count++;

	return value;
}

void *hash_get(hash_t *self, const void *key) {

	int index = self->hash(key) % self->capacity;
	hash_entry_t *entry = &self->entries[index];
	while (entry->present) {
		if (!self->compare(entry->key, key)) {
			return entry->value;
		}
		index = (index + 1) % self->capacity;
		entry = &self->entries[index];
	}

	return 0;
}

void *hash_remove(hash_t *self, const void *key) {

	int index = self->hash(key) % self->capacity;
	hash_entry_t *entry = &self->entries[index];
	while (entry->present) {
		if (!self->compare(entry->key, key)) {
			self->count--;
			entry->present = 0;
			return entry->value;
		}
		index = (index + 1) % self->capacity;
		entry = &self->entries[index];
	}

	return 0;
}

unsigned int hash_string(const void *key) {
	const char *string = (const char *)key;

	/* FNV-1 hash function */
	unsigned int offset_basis = 2166136261;
	unsigned int fnv_prime = 16777619;

	unsigned int hash = offset_basis;
	unsigned int length = strlen(string);
	for (unsigned int i = 0; i < length; i++) {
		hash = hash * fnv_prime;
		hash = hash ^ string[i];
	}

	return hash;
}

unsigned int hash_pointer(const void *key) {
	unsigned int a = (unsigned int)key;

    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) ^ (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a + 0xd3a2646c) ^ (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) ^ (a >> 16);

	return a;
}


void hash_free(hash_t *self) {
	free(self->entries);
	free(self);
}
