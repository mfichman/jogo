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

expr_t *expr_literal(parser_t *parser, type_t *type, const char *string) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_STRING;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(string) + 1);
	self->nchild = 0;
	self->chktype = type;
	self->object = 0;
	self->next = 0;

	strcpy(self->string, string);

	return self;
}

expr_t *expr_binary(parser_t *parser, const char *op, expr_t *lhs, expr_t *rhs) {
	if (type_comp(lhs->chktype, rhs->chktype)) {
		parser_error(parser);
		fprintf(stderr, "Invalid operands for '%s': %s, %s\n", op, 
			lhs->chktype->name, rhs->chktype->name);
		expr_free(lhs);
		expr_free(rhs);
		return 0;
	}

	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_BINARY;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(op) + 1);
	self->nchild = 2;
	self->child[0] = lhs;
	self->child[1] = rhs;
	self->chktype = type_clone(lhs->chktype);
	self->object = 0;
	self->next = 0;
		
	strcpy(self->string, op);

	return self;
}

expr_t *expr_unary(parser_t *parser, const char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_UNARY;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(op) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->chktype = type_clone(expr->chktype);
	self->object = 0;
	self->next = 0;
	
	strcpy(self->string, op);

	return self;
}

expr_t *expr_call(parser_t *parser, expr_t *func, expr_t *args) {
	/* TODO: Look up function in symbol table */


	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_CALL;
	self->line = parser_line_number(parser);
	self->string = 0; 
	self->nchild = 2;
	self->child[0] = func;
	self->child[1] = args;
	self->chktype = 0; /* Assign the return value of the lookup function */
	self->object = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_ctor(parser_t *parser, type_t *type, expr_t *args) {
	/* TODO: Ditto expr_call */
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_CTOR;
	self->line = parser_line_number(parser);
	self->string = 0;
	self->nchild = 1;
	self->child[0] = args;
	self->chktype = type_clone(type);
	self->object = type;
	self->next = 0;

	return self;
}
	
expr_t *expr_index(parser_t *parser, expr_t *expr, expr_t *index) {
	/* TODO: expr must be an array type, index must be an int */
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_INDEX;
	self->line = parser_line_number(parser);
	self->string = 0; 
	self->nchild = 2;
	self->child[0] = expr;
	self->child[1] = index;
	self->chktype = 0;
	self->object = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_member(parser_t *parser, expr_t *expr, const char *ident) {
	/* TODO: look up the type of ident based on the type of expr */
	/* Members must be listed by the fully-qualifed name */
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_MEMBER;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(ident) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->chktype = 0;
	self->object = 0;
	self->next = 0;

	strcpy(self->string, ident);
	
	return self;
}

expr_t *expr_static(parser_t *parser, type_t *type, const char *ident) {
	/* Ditto 136 */
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_STATIC;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(ident) + 1);	
	self->nchild = 0;
	self->object = type;
	self->chktype = 0;
	self->next = 0;
	
	strcpy(self->string, ident);
	
	return self;
}

expr_t *expr_var(parser_t *parser, const char *name) {
	var_t *var = symtab_get_var(parser->symbols, name);
	if (!var) {
		parser_error(parser);
		fprintf(stderr, "Undeclared identifier: %s\n", name);
		free((void*)name);
		return 0;
	}

	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_VAR;
	self->line = parser_line_number(parser);
	self->string = malloc(strlen(name) + 1);
	self->nchild = 0;
	self->chktype = type_clone(var->type);
	self->object = 0;
	self->next = 0;

	strcpy(self->string, name);
	
	return self;
}
	
void expr_free(expr_t *self) {
	if (self) {
		for (size_t i = 0; i < self->nchild; i++) {
			expr_free(self->child[i]);	
		}
		expr_free(self->next);
		type_free(self->object);
		type_free(self->chktype);
		free(self->string);
		free(self);
	}
}
