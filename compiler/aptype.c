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

#include <aptype.h>
#include <aphash.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


aptype_t *aptype_object(char *name) {
	aptype_t *self = malloc(sizeof(aptype_t));

	self->name = name;
	self->type = APTYPE_TYPE_OBJECT;
    self->flags = 0;

	return self;
}

aptype_t *aptype_clone(aptype_t *other) {
	if (!other) {
		return 0;
	}

	aptype_t *self = malloc(sizeof(aptype_t));
	self->name = malloc(strlen(other->name) + 1);
	self->type = other->type;
    self->flags = 0;

	strcpy(self->name, other->name);
	
	return self;
}

int aptype_comp(aptype_t *self, aptype_t *other) {
	if (self->type != other->type) {
		return 1;
	}
	
    if (strcmp(self->name, other->name)) {
	    return 1;
    }

	if (!self && !other) {
		return 0;
	} else {
		return 1;
	}
}

void aptype_print(aptype_t *self, FILE *file) {
	if (self) {
		fprintf(file, "'%s'", self->name);
	} else {
		fprintf(file, "'void'");
	}
}

void aptype_free(aptype_t *self) {
	if (self) {
		free(self->name);
		free(self);
	}
}

