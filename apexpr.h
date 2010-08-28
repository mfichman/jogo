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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#ifndef APEXPR_H
#define APEXPR_H

#include <apollo.h>

#define APEXPR_TYPE_STRING 1
#define APEXPR_TYPE_UNARY 2 
#define APEXPR_TYPE_BINARY 3 
#define APEXPR_TYPE_CALL 4
#define APEXPR_TYPE_INDEX 5
#define APEXPR_TYPE_POSTFIX 6
#define APEXPR_TYPE_MEMBER 7
#define APEXPR_TYPE_STATIC 8
#define APEXPR_TYPE_CTOR 9
#define APEXPR_TYPE_VAR 10 
#define APEXPR_TYPE_MCALL 11
#define APEXPR_TYPE_SCALL 12

/* Structure for holding expression syntax tree */
struct apexpr {
	int type;				/* Expression type type */
	char *string;			/* Possible string value of the expression */
	int nchild;				/* Number of children */
	aptype_t *chktype;		/* Type evaluation for expression */
	aptype_t *clstype;		/* Optional class type name */
	apexpr_t *child[2];      	/* Child expressions */
	apexpr_t *next;
};


apexpr_t *apexpr_literal(aploc_t *loc, aptype_t *type, char *string);
apexpr_t *apexpr_binary(aploc_t *loc, char *op, apexpr_t *lh, apexpr_t *rh);
apexpr_t *apexpr_unary(aploc_t *loc, char *op, apexpr_t *expr);
apexpr_t *apexpr_call(aploc_t *loc, char *fn, apexpr_t *args);
apexpr_t *apexpr_mcall(aploc_t *loc, apexpr_t *obj, char *fn, apexpr_t *args);
apexpr_t *apexpr_scall(aploc_t *loc, aptype_t *obj, char *fn, apexpr_t *args);
apexpr_t *apexpr_ctor(aploc_t *loc, aptype_t *obj, apexpr_t *args);
apexpr_t *apexpr_index(aploc_t *loc, apexpr_t *obj, apexpr_t *index);
apexpr_t *apexpr_member(aploc_t *loc, apexpr_t *expr, char *ident);
apexpr_t *apexpr_static(aploc_t *loc, aptype_t *type, char *ident);
apexpr_t *apexpr_var(aploc_t *loc, char *name);
void apexpr_free(apexpr_t *self);



#endif
