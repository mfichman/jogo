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

#include <symtab.h>
#include <var.h>
#include <hash.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct symtab {
	symtab_t *parent;
	hash_t *vars;
	hash_t *funcs;
};

symtab_t *symtab_alloc(symtab_t *parent) {
	symtab_t *self = malloc(sizeof(symtab_t));

	self->parent = parent;
	self->vars = hash_alloc((hash_compfn_t)&strcmp, &hash_string);
	self->funcs = hash_alloc((hash_compfn_t)&strcmp, &hash_string);

	return self;
}

void symtab_var(symtab_t *self, const char *name, var_t *var) {
	hash_put(self->vars, name, var);
}

void symtab_func(symtab_t *self, const char *name, func_t *func) {
	hash_put(self->funcs, name, func);
}

var_t *symtab_get_var(symtab_t *self, const char *name) {
	var_t *var = hash_get(self->vars, name);
	if (!var && self->parent) {
		return hash_get(self->vars, name);
	} else {
		return var;
	}
}

func_t *symtab_get_func(symtab_t *self, const char *name) {
	func_t *func = hash_get(self->funcs, name);
	if (!func && self->parent) {
		return hash_get(self->funcs, name);
	} else {
		return func;
	}
}
