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
	aphash_t *vars;
	aphash_t *funcs;
};

apsymtab_t *apsymtab_alloc(apsymtab_t *parent) {
	apsymtab_t *self = malloc(sizeof(apsymtab_t));

	self->parent = parent;
	self->vars = aphash_alloc((aphash_compfn_t)&strcmp, &aphash_string);
	self->funcs = aphash_alloc((aphash_compfn_t)&strcmp, &aphash_string);

	return self;
}

void apsymtab_var(apsymtab_t *self, const char *name, apvar_t *var) {
	aphash_put(self->vars, name, var);
}

void apsymtab_func(apsymtab_t *self, const char *name, apfunc_t *func) {
	aphash_put(self->funcs, name, func);
}

apvar_t *apsymtab_get_var(apsymtab_t *self, const char *name) {
	apvar_t *var = aphash_get(self->vars, name);
	if (!var && self->parent) {
		return apsymtab_get_var(self->parent, name);
	} else {
		return var;
	}
}

apfunc_t *apsymtab_get_func(apsymtab_t *self, const char *name) {
	apfunc_t *func = aphash_get(self->funcs, name);
	if (!func && self->parent) {
		return apsymtab_get_func(self->parent, name);
	} else {
		return func;
	}
}

apsymtab_t *apsymtab_get_parent(apsymtab_t *self) {
	return self->parent;
}

void apsymtab_free(apsymtab_t *self) {
	if (self) {
		aphash_free(self->vars);
		aphash_free(self->funcs);
		free(self);
	}

}
