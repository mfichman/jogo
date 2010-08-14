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

#ifndef EXPR_H
#define EXPR_H

#include <apollo.h>

#define EXPR_TYPE_STRING 1
#define EXPR_TYPE_UNARY 2 
#define EXPR_TYPE_BINARY 3 
#define EXPR_TYPE_CALL 4
#define EXPR_TYPE_INDEX 5
#define EXPR_TYPE_POSTFIX 6
#define EXPR_TYPE_MEMBER 7
#define EXPR_TYPE_STATIC 8
#define EXPR_TYPE_CTOR 9
#define EXPR_TYPE_VAR 10 

/* Structure for holding expression syntax tree */
struct expr {
	int type;				/* Expression type type */
	char *string;			/* Possible string value of the expression */
	int nchild;				/* Number of children */
	type_t *object;			/* Optional type */
	expr_t *child[2];      	/* Child expressions */
	expr_t *next;
};


expr_t *expr_literal(type_t *type, const char *string);
expr_t *expr_binary(const char *op, expr_t *lhs, expr_t *rhs);
expr_t *expr_unary(const char *op, expr_t *expr);
expr_t *expr_call(expr_t *func, expr_t *args);
expr_t *expr_ctor(type_t *type, expr_t *args);
expr_t *expr_index(expr_t *obj, expr_t *index);
expr_t *expr_member(expr_t *expr, const char *ident);
expr_t *expr_static(type_t *type, const char *ident);
expr_t *expr_var(const char *name);
void expr_free(expr_t *self);



#endif
