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
	apunit_t *units; /* List of compilation units (classes) */
	apsymtab_t *symbols; /* Temporary symbol table variable */
	aphash_t *types; /* Hash for looking up units by name */
	aptype_t *rets; /* Return type of current function being parsed */
	apimport_t *queue; /* Queue of files to parse */
	FILE *fd; /* Current file descriptor */
	int column; /* Current column number */
	int error; /* Error number */
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

/* Source code location range */
struct aploc {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
};

apparser_t *apparser_alloc();
int apparser_parse(apparser_t *self, const char *filename);
int apparser_parse_unit(apparser_t *self, const char *filename);
void apparser_class(apparser_t *self, apunit_t *unit);
void apparser_interface(apparser_t *self, apunit_t *unit);
void apparser_struct(apparser_t *self, apunit_t *unit);
void apparser_module(apparser_t *self, apunit_t *unit);
int apparser_read(apparser_t *self, char *buffer, int length);
void apparser_check_unit(apparser_t *self, apunit_t *unit);
void apparser_check_func(apparser_t *self, apfunc_t *func);
void apparser_check_stmt(apparser_t *self, apstmt_t *stmt);
void apparser_check_expr(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_binary(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_var(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_mvar(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_svar(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_ctor(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_call(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_scall(apparser_t *self, apexpr_t *expr);
void apparser_check_expr_mcall(apparser_t *self, apexpr_t *expr);
void apparser_check_var(apparser_t *self, apvar_t *var);
int apparser_check_args(apparser_t *self, apfunc_t *func, apexpr_t *expr);
int apparser_resolve_type(apparser_t *self, aptype_t *type);
void apparser_print_type(apparser_t *self, aptype_t *type);
void apparser_print_loc(apparser_t *self, aploc_t *loc);
void apparser_print_loc_end(apparser_t *self, aploc_t *loc);
void apparser_print_params(apparser_t *self, apvar_t *var);
void apparser_print_args(apparser_t *self, apexpr_t *expr);
void apparser_error(apparser_t *self, const char* fmt, ...);
void apparser_free(apparser_t *self);

static inline char *strdup(const char* str) {
	char *copy = malloc(strlen(str) + 1);
	strcpy(copy, str);
	return copy;
}


#endif
