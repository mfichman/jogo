#include <expr.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

expr_t *expr_string(const char *string) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_STRING;
	self->string = malloc(strlen(string) + 1);
	self->nchild = 0;
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
	self->next = 0;
		
	return self;
}

expr_t *expr_member(expr_t *expr, const char *ident) {
	expr_t *self = malloc(sizeof(expr_t));

	self->type = EXPR_TYPE_MEMBER;
	self->string = malloc(strlen(ident) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->next = 0;

	strcpy(self->string, ident);
	
	return self;
}

expr_t *expr_postfix(const char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof(expr_t));
	
	self->type = EXPR_TYPE_POSTFIX;
	self->string = malloc(strlen(op) + 1);
	self->nchild = 1;
	self->child[0] = expr;
	self->next = 0;

	strcpy(self->string, op);

	return self;
}


void expr_free(expr_t *self) {
	if (self) {
		for (size_t i = 0; i < self->nchild; i++) {
			expr_free(self->child[i]);	
		}
		expr_free(self->next);
		free(self->string);
		free(self);
	}
}
