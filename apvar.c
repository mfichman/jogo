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

#include <apvar.h>
#include <apexpr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

apvar_t *apvar_alloc(char *name, int flags, aptype_t *type, apexpr_t *expr) {
	apvar_t *self = malloc(sizeof(apvar_t));

	self->name = name;
	self->flags = flags;
	self->type = type;
	self->expr = expr;
	self->next = 0;
	
	return self;
}

void apvar_free(apvar_t *self) {

	if (self) {

		free(self->name);
		aptype_free(self->type);
		apexpr_free(self->expr);
		apvar_free(self->next);
		free(self);

	}
}

int apvar_comp(apvar_t *self, apvar_t *var) {
	return self - var;
}
