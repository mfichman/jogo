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

#ifndef APUNIT_H
#define APUNIT_H

#include <apollo.h>

#define APUNIT_TYPE_CLASS 1 /* Class compilation apunit */
#define APUNIT_TYPE_STRUCT 2 /* Structure compilation apunit */
#define APUNIT_TYPE_MODULE 3 /* Module compilation apunit */
#define APUNIT_TYPE_INTERFACE 4 /* Interface compilation apunit */

#define APUNIT_FLAG_PUBLIC 0x1
#define APUNIT_FLAG_PRIVATE 0x2
#define APUNIT_FLAG_NATIVE 0x4
#define APUNIT_FLAG_STATIC 0x8
#define APUNIT_FLAG_CONST 0x10


/* Compilation apunit struct for classes, structs, and enums */
struct apunit {
	apsymbol_t symbol; /* Symbol type tag */
	char *name; /* Name of the compilation unit */
	int type; /* Type of apunit: struct, class, or enum */
	char *filename; /* File name of the compilation unit */
	apimport_t *imports; /* List of imports */
	apdef_t *defs; /* List of defines */
	apvar_t *vars; /* List of variables */
	apvar_t *consts; /* List of constants */
	apfunc_t *ctors; /* List of constructors */
	apfunc_t *dtors; /* List of destructors */
	apfunc_t *funcs; /* List of functions */
	apsymtab_t *symbols; /* Hash table of functions and variables */
	apunit_t *next;
};

apunit_t *apunit_alloc(char *filename, int type);
void apunit_name(apunit_t *self, char *name);
void apunit_import(apunit_t *self, apimport_t *import);
void apunit_def(apunit_t *self, apdef_t *def);
void apunit_var(apunit_t *self, apvar_t *var);
void apunit_const(apunit_t *self, apvar_t *cons);
void apunit_ctor(apunit_t *self, apfunc_t *ctor);
void apunit_dtor(apunit_t *self, apfunc_t *dtor);
void apunit_func(apunit_t *self, apfunc_t *func);
void apunit_free(apunit_t *self);

#endif
