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

#include <apunit.h>
#include <apimport.h>
#include <apdef.h>
#include <apfunc.h>
#include <apvar.h>
#include <apsymtab.h>
#include <assert.h>
#include <stdlib.h>

apunit_t *apunit_alloc(int type) {
	apunit_t *self = malloc(sizeof(apunit_t));

	self->name = 0;
	self->type = type;
	self->imports = 0;
	self->defs = 0;
	self->vars = 0;
	self->ctors = 0;
	self->dtors = 0;
	self->funcs = 0;
	self->symbols = 0;
	self->next = 0;

	return self;
}

void apunit_name(apunit_t *self, aptype_t *name) {
	assert(!self->name);
	self->name = name;
}

void apunit_import(apunit_t *self, apimport_t *import) {
	import->next = self->imports;
	self->imports = import;
}

void apunit_def(apunit_t *self, apdef_t *def) {
	def->next = self->defs;
	self->defs = def;
}

void apunit_var(apunit_t *self, apvar_t *var) {
	var->next = self->vars;
	self->vars = var;
	apsymtab_var(self->symbols, var->name, var);
}

void apunit_ctor(apunit_t *self, apfunc_t *ctor) {
	ctor->next = self->ctors;
	self->ctors = ctor;
}

void apunit_dtor(apunit_t *self, apfunc_t *dtor) {
	dtor->next = self->dtors;
	self->dtors = dtor;
}

void apunit_func(apunit_t *self, apfunc_t *func) {
	func->next = self->funcs;
	self->funcs = func;
	apsymtab_func(self->symbols, func->name, func);
}

void apunit_free(apunit_t *self) {
	if (self) {
		aptype_free(self->name);
		apimport_free(self->imports);
		apdef_free(self->defs);
		apvar_free(self->vars);
		apfunc_free(self->ctors);
		apfunc_free(self->dtors);
		apfunc_free(self->funcs);
		apunit_free(self->next);
		apsymtab_free(self->symbols);
		free(self);
	}
}
