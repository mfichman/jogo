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

#ifndef APPARSER_H
#define APPARSER_H

#include <apollo.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Primary parser structure; contains symbol table and compilation units */
struct apparser {
	apunit_t *unit; /* Current unit being parsed */
    apenv_t *env; /* Current environment for the unit */
	FILE *fd; /* Current file descriptor */
	int column; /* Current column number */
	int error; /* Error flag */
	char *filename; /* Current filename */
	void *scanner;
};

/* Union of the different types possible for a Bison production */
typedef union apnode apnode_t;
union apnode {
	apexpr_t *expr; 
	apstmt_t *stmt;
	aptype_t *type;
	apunit_t *unit;
	apvar_t *var;
	apfunc_t *func;
	apimport_t *import;
	apdef_t *def;
	char *string;	
	int null;
	int flag;
};

/* Primary parser structure; contains symbol table and compilation units */
apparser_t *apparser_alloc();
int apparser_parse(apparser_t *self, apenv_t *env);
void apparser_unit(apparser_t *self, apunit_t *unit);
int apparser_read(apparser_t *self, char *buffer, int length);
void apparser_newline(apparser_t *self);
void apparser_error(apparser_t *self, const char *fmt, ...);
void apparser_free(apparser_t *self);

#endif
