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

#ifndef APFUNC_H
#define APFUNC_H

#include <apollo.h>
#include <aploc.h>

#define APFUNC_TYPE_MEMBER 0
#define APFUNC_TYPE_FREE 1
#define APFUNC_TYPE_CTOR 2
#define APFUNC_TYPE_DTOR 3

/* Descriptor struct for functions and methods */
struct apfunc {
	apsymbol_t symbol; /* Symbol type tag */
	char *name; /* Qualified function name */
	int flags; /* Function flags */
	aploc_t loc; /* Location of the function text */
    apunit_t *unit; /* Enclosing unit */
	apstmt_t *block; /* Function body */
	aptype_t *rets; /* List of return types (in reverse order) */
	apvar_t *args; /* List of argument types (in reverse_order) */
	apfunc_t *next;         
};

apfunc_t *apfunc_alloc(aploc_t *loc, char *name, apvar_t *arg, aptype_t *ret);
void apfunc_free(apfunc_t *self);

#endif