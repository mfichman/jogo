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

#ifndef APTYPE_H
#define APTYPE_H

#define APTYPE_TYPE_PRIMITIVE 1
#define APTYPE_TYPE_OBJECT 2

#include <apollo.h>

/* A fully-qualified a aptype name */ 
struct aptype {
	char *name;             /* Fully qualified aptype name */
	int type;				/* Primitive or not? */
	int pointer;			/* True if this is a pointer */
	aptype_t *next;
};

aptype_t *aptype_object(char *name);
aptype_t *aptype_primitive(char *name);
aptype_t *aptype_concat(aptype_t *self, aptype_t *type);
aptype_t *aptype_clone(aptype_t *self);
int aptype_comp(aptype_t *self, aptype_t *type);
int aptype_bool_compat(aptype_t *self);
int aptype_integer_compat(aptype_t *self);
void aptype_free(aptype_t *self);

#endif
