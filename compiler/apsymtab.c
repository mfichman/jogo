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

#include <apsymtab.h>
#include <apvar.h>
#include <aphash.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct apsymtab {
	apsymtab_t *parent;
	aphash_t *symbols;
};

apsymtab_t *apsymtab_alloc(apsymtab_t *parent) {
	apsymtab_t *self = malloc(sizeof(apsymtab_t));

	self->parent = parent;
	self->symbols = aphash_alloc((aphash_compfn_t)&strcmp, &aphash_string);

	return self;
}

void apsymtab_put(apsymtab_t *self, const char *name, void *symbol) {
	aphash_put(self->symbols, name, symbol);
}

void *apsymtab_get(apsymtab_t *self, const char *name) {
	void *symbol = aphash_get(self->symbols, name);
	if (!symbol && self->parent) {
		return apsymtab_get(self->parent, name);
	} else {
		return symbol;
	}
}

apsymtab_t *apsymtab_get_parent(apsymtab_t *self) {
	return self->parent;
}

apsymtab_iter_t apsymtab_iter(apsymtab_t *self) {
	return aphash_iter(self->symbols);
}

void *apsymtab_next(apsymtab_t* self, apsymtab_iter_t *iter) {
	return aphash_next(self->symbols, iter);
}


void apsymtab_free(apsymtab_t *self) {
	if (self) {
		aphash_free(self->symbols);
		free(self);
	}

}
