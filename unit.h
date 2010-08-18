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

#ifndef UNIT_H
#define UNIT_H

#include <apollo.h>

#define UNIT_TYPE_CLASS 1 /* Class compilation unit */
#define UNIT_TYPE_STRUCT 2 /* Structure compilation unit */
#define UNIT_TYPE_MODULE 3 /* Module compilation unit */
#define UNIT_TYPE_INTERFACE 4 /* Interface compilation unit */

#define UNIT_FLAG_PUBLIC 0x1
#define UNIT_FLAG_PRIVATE 0x2
#define UNIT_FLAG_PROTECTED 0x3
#define UNIT_FLAG_NATIVE 0x4
#define UNIT_FLAG_STATIC 0x5
#define UNIT_FLAG_CONST 0x6


/* Compilation unit struct for classes, structs, and enums */
struct unit {
	type_t *name;       	/* Name of the compilation unit */
	int type;               /* Type of unit: struct, class, or enum */
	import_t *imports;      /* List of imports */
	def_t *defs;            /* List of defines */
	var_t *vars;       		/* List of variables */
	func_t *ctors;      	/* List of constructors */
	func_t *dtors;      	/* List of destructors */
	func_t *funcs;      	/* List of functions */
	symtab_t *symtab;		/* Hash table of functions and variables */
	unit_t *next;
};

unit_t *unit_alloc(int type);
void unit_name(unit_t *self, type_t *name);
void unit_import(unit_t *self, import_t *import);
void unit_def(unit_t *self, def_t *def);
void unit_var(unit_t *self, var_t *var);
void unit_ctor(unit_t *self, func_t *ctor);
void unit_dtor(unit_t *self, func_t *dtor);
void unit_func(unit_t *self, func_t *func);
void unit_free(unit_t *self);

#endif
