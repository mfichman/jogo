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

#include <var.h>
#include <expr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

var_t *var_alloc(char *name, int flags, type_t *type, expr_t *expr) {
	var_t *self = malloc(sizeof(var_t));

	self->name = name;
	self->flags = flags;
	self->type = type;
	self->expr = expr;
	self->next = 0;
	
	return self;
}

void var_free(var_t *self) {

	if (self) {

		free(self->name);
		type_free(self->type);
		expr_free(self->expr);
		var_free(self->next);
		free(self);

	}
}

int var_comp(var_t *self, var_t *var) {
	return self - var;
}
