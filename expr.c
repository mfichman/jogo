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
#include <type.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

expr_t *expr_literal(type_t *type, const char *string) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_STRING;
	self->string = malloc(strlen(string) + 1);
	self->nchild = 0;
	self->object = type;
	self->next = 0;

	strcpy(self->string, string);

	return self;
}

expr_t *expr_binary(const char *op, expr_t *lhs, expr_t *rhs) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_BINARY;
	self->string = malloc(strlen(op) + 1);
	self->nchild = 2;
	self->child[0] = lhs;
	self->child[1] = rhs;
	self->object = 0;
	self->next = 0;
		
	strcpy(self->string, op);

	return self;
}

expr_t *expr_unary(const char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_UNARY;
	self->string = malloc(strlen(op) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->object = 0;
	self->next = 0;
	
	strcpy(self->string, op);

	return self;
}

expr_t *expr_call(expr_t *func, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_CALL;
	self->string = 0; 
	self->nchild = 2;
	self->child[0] = func;
	self->child[1] = args;
	self->object = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_ctor(type_t *type, expr_t *args) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_CTOR;
	self->string = 0;
	self->nchild = 1;
	self->child[0] = args;
	self->object = type;
	self->next = 0;

	return self;
}
	
expr_t *expr_index(expr_t *tuple, expr_t *expr) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_INDEX;
	self->string = 0; 
	self->nchild = 2;
	self->child[0] = tuple;
	self->child[1] = expr;
	self->object = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_member(expr_t *expr, const char *ident) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_MEMBER;
	self->string = malloc(strlen(ident) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->object = 0;
	self->next = 0;

	strcpy(self->string, ident);
	
	return self;
}

expr_t *expr_static(type_t *type, const char *ident) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_STATIC;
	self->string = malloc(strlen(ident) + 1);	
	self->nchild = 0;
	self->object = type;
	self->next = 0;
	
	strcpy(self->string, ident);
	
	return self;
}

expr_t *expr_var(const char *name) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_VAR;
	self->string = malloc(strlen(name) + 1);
	self->nchild = 0;
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
		free(self->string);
		free(self);
	}
}
