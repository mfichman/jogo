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
#define EXPR_TYPE_MCALL 11
#define EXPR_TYPE_SCALL 12

/* Structure for holding expression syntax tree */
struct expr {
	int type;				/* Expression type type */
	int line;				/* Line number for this expression */
	char *string;			/* Possible string value of the expression */
	int nchild;				/* Number of children */
	type_t *chktype;		/* Type evaluation for expression */
	type_t *clstype;		/* Optional class type name */
	expr_t *child[2];      	/* Child expressions */
	expr_t *next;
};


expr_t *expr_literal(parser_t *parser, type_t *type, char *string);
expr_t *expr_binary(parser_t *parser, char *op, expr_t *lh, expr_t *rh);
expr_t *expr_unary(parser_t *parser, char *op, expr_t *expr);
expr_t *expr_call(parser_t *parser, char *fn, expr_t *args);
expr_t *expr_mcall(parser_t *parser, expr_t *obj, char *fn, expr_t *args);
expr_t *expr_scall(parser_t *parser, type_t *obj, char *fn, expr_t *args);
expr_t *expr_ctor(parser_t *parser, type_t *obj, expr_t *args);
expr_t *expr_index(parser_t *parser, expr_t *obj, expr_t *index);
expr_t *expr_member(parser_t *parser, expr_t *expr, char *ident);
expr_t *expr_static(parser_t *parser, type_t *type, char *ident);
expr_t *expr_var(parser_t *parser, char *name);
void expr_free(expr_t *self);



#endif
