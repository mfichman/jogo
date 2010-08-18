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

#include <unit.h>
#include <import.h>
#include <def.h>
#include <func.h>
#include <var.h>
#include <assert.h>
#include <symtab.h>
#include <stdlib.h>

unit_t *unit_alloc(int type) {
	unit_t *self = malloc(sizeof(unit_t));

	self->name = 0;
	self->type = type;
	self->imports = 0;
	self->defs = 0;
	self->vars = 0;
	self->ctors = 0;
	self->dtors = 0;
	self->funcs = 0;
	self->symtab = symtab_alloc(0); 
	self->next = 0;

	return self;
}

void unit_name(unit_t *self, type_t *name) {
	assert(!self->name);
	self->name = name;
}

void unit_import(unit_t *self, import_t *import) {
	import->next = self->imports;
	self->imports = import;
}

void unit_def(unit_t *self, def_t *def) {
	def->next = self->defs;
	self->defs = def;
}

void unit_var(unit_t *self, var_t *var) {
	var->next = self->vars;
	self->vars = var;
	symtab_var(self->symtab, var->name, var);
}

void unit_ctor(unit_t *self, func_t *ctor) {
	ctor->next = self->ctors;
	self->ctors = ctor;
	symtab_func(self->symtab, ctor->name, ctor);
}

void unit_dtor(unit_t *self, func_t *dtor) {
	dtor->next = self->dtors;
	self->dtors = dtor;
}

void unit_func(unit_t *self, func_t *func) {
	func->next = self->funcs;
	self->funcs = func;
	symtab_func(self->symtab, func->name, func);
}

void unit_free(unit_t *self) {
	if (self) {
		type_free(self->name);
		import_free(self->imports);
		def_free(self->defs);
		var_free(self->vars);
		func_free(self->ctors);
		func_free(self->dtors);
		func_free(self->funcs);
		unit_free(self->next);
		symtab_free(self->symtab);
		free(self);
	}
}
