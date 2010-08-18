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

#include <expr.h>
#include <parser.h>
#include <type.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <symtab.h>
#include <var.h>
#include <assert.h>

expr_t *expr_literal(parser_t *parser, type_t *type, char *string) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_STRING;
	self->line = parser_line_number(parser);
	self->string = string; 
	self->nchild = 0;
	self->chktype = type;
	self->clstype = 0;
	self->next = 0;

	return self;
}

expr_t *expr_binary(parser_t *parser, char *op, expr_t *lhs, expr_t *rhs) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_BINARY;
	self->line = parser_line_number(parser);
	self->string = op;
	self->nchild = 2;
	self->child[0] = lhs;
	self->child[1] = rhs;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_unary(parser_t *parser, char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_UNARY;
	self->line = parser_line_number(parser);
	self->string = op; 
	self->nchild = 1;
	self->child[0] = expr;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;
	
	return self;
}

expr_t *expr_call(parser_t *parser, char *fn, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_CALL;
	self->line = parser_line_number(parser);
	self->string = fn; 
	self->nchild = 1;
	self->child[0] = args;
	self->chktype = 0; 
	self->clstype = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_mcall(parser_t *parser, expr_t *obj, char *fn, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_MCALL;
	self->line = parser_line_number(parser);
	self->string = fn;
	self->nchild = 2;
	self->child[0] = obj;
	self->child[1] = args;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;
	
	return self;
}

expr_t *expr_scall(parser_t *parser, type_t *obj, char *fn, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_MCALL;
	self->line = parser_line_number(parser);
	self->string = fn;
	self->nchild = 1;
	self->child[0] = args;
	self->chktype = 0;
	self->clstype = obj;
	self->next = 0;

	return self;
}

expr_t *expr_ctor(parser_t *parser, type_t *type, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_CTOR;
	self->line = parser_line_number(parser);
	self->string = 0;
	self->nchild = 1;
	self->child[0] = args;
	self->chktype = 0;
	self->clstype = type;
	self->next = 0;

	return self;
}
	
expr_t *expr_index(parser_t *parser, expr_t *expr, expr_t *index) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_INDEX;
	self->line = parser_line_number(parser);
	self->string = 0; 
	self->nchild = 2;
	self->child[0] = expr;
	self->child[1] = index;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_member(parser_t *parser, expr_t *expr, char *ident) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_MEMBER;
	self->line = parser_line_number(parser);
	self->string = ident; 
	self->nchild = 1;
	self->child[0] = expr;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;

	return self;
}

expr_t *expr_static(parser_t *parser, type_t *type, char *ident) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_STATIC;
	self->line = parser_line_number(parser);
	self->string = ident;	
	self->nchild = 0;
	self->clstype = type;
	self->chktype = 0;
	self->next = 0;
	
	return self;
}

expr_t *expr_var(parser_t *parser, char *name) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_VAR;
	self->line = parser_line_number(parser);
	self->string = name; 
	self->nchild = 0;
	self->chktype = 0;
	self->clstype = 0;
	self->next = 0;

	return self;
}
	
void expr_free(expr_t *self) {
	if (self) {
		for (size_t i = 0; i < self->nchild; i++) {
			expr_free(self->child[i]);	
		}
		expr_free(self->next);
		type_free(self->clstype);
		type_free(self->chktype);
		free(self->string);
		free(self);
	}
}
