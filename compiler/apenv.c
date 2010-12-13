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

#include <apenv.h>
#include <apunit.h>
#include <aphash.h>

apenv_t *apenv_alloc(const char *root) {
    apenv_t *self = malloc(sizeof(apenv_t));
    
	aphash_compfn_t comp = (aphash_compfn_t)&strcmp;
	aphash_hashfn_t hash = (aphash_hashfn_t)&aphash_string;
    
    self->types = aphash_alloc(comp, hash);
    self->units = 0;
    self->root = strdup(root);

    return self;
}

void apenv_unit(apenv_t *self, apunit_t *unit) {
    aphash_put(self->types, unit->name, unit); 
    unit->next = self->units;
    self->units = unit;
}

void apenv_free(apenv_t *self) {
    aphash_free(self->types);
    apunit_free(self->units);
    free(self->root);
    free(self);
}
