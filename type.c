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

#include <type.h>
#include <string.h>
#include <stdlib.h>


type_t *type_object(const char *name) {
	type_t *self = malloc(sizeof(type_t));

	self->name = malloc(strlen(name) + 1);
	self->type = TYPE_TYPE_OBJECT;
	self->next = 0;
	self->pointer = 0;

	strcpy(self->name, name);
	
	return self;
}

type_t *type_primitive(const char *name) {
	type_t *self = malloc(sizeof(type_t));

	self->name = malloc(strlen(name) + 1);
	self->type = TYPE_TYPE_PRIMITIVE;
	self->next = 0;
	self->pointer = 0;

	strcpy(self->name, name);
	
	return self;
}

type_t *type_concat(type_t *self, type_t *type) {

	size_t length = strlen(self->name) + strlen(type->name) + 3;
	self->name = realloc(self->name, length); 
	strcat(self->name, "::");
	strcat(self->name, type->name);
	
	type_free(type);
	
	return self;
}

int type_comp(type_t *self, type_t *other) {

	while (self && other) {
		if (strcmp(self->name, other->name)) {
			return 0;
		}
		self = self->next;
		other = other->next;
	}

	if (self && other) {
		return 1;
	} else {
		return 0;
	}
}

void type_free(type_t *self) {
	if (self) {
		free(self->name);
		type_free(self->next);
		free(self);
	}
}

