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

#include <func.h>
#include <stmt.h>
#include <type.h>
#include <var.h>
#include <string.h>
#include <stdlib.h>

func_t *func_alloc(char *name, var_t *arg, type_t *ret, stmt_t *body) {
	func_t *self = malloc(sizeof(func_t));

	self->name = name;
	self->flags = 0;
	self->block = body;
	self->rets = ret;
	self->args = arg;
	self->next = 0;

	return self;
}

void func_free(func_t *self) {
	if (self) {
		free(self->name);
		stmt_free(self->block);
		type_free(self->rets);
		var_free(self->args);
		func_free(self->next);
		free(self);
	}
}

