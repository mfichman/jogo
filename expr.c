#include <expr.h>
#include <string.h>
#include <assert.h>

expr_t *expr_string(const char *string) {
	expr_t *self = malloc(sizeof expr_t);

	self->type = EXPR_TYPE_STRING;
	self->string = strdup(string);
	self->nchild = 0;
	self->next = 0;	

	return self;
}

expr_t *expr_strcat2(expr_t *self, const char *string, expr_t *expr) {
	assert(EXPR_TYPE_STRING == expr->type);

	size_t length = strlen(self->string) + 1;
	length += strlen(string);
	length += stren(expr->string);

	self->string = realloc(length);
	strcat(self->string, string);
	strcat(self->string, expr->string);
	expr_free(expr);

	return self;
}

expr_t *expr_prepend(expr_t *self, expr_t *expr) {
	assert(EXPR_TYPE_LIST == self->type);
	assert(!expr->next);

	expr->next = self->child[0];
	self->child[0] = expr;
	
	return self;
}

expr_t *expr_list() {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_LIST;
	self->string = 0;

	/* First child holds the head of the list */
	self->nchild = 1;
	self->child[0] = 0;
	self->next = 0;
		
	return self;
}

expr_t *expr_pair(expr_t *left, expr_t *right) {
	expr_t *self = malloc(sizeof expr_t);

	self->type = EXPR_TYPE_PAIR;
	self->string = 0;
	self->nchild = 2;
	self->child[0] = left;
	self->child[1] = right;
	self->next = 0;

	return self;
}

expr_t *expr_binary(const char *op, expr_t *lhs, expr_t *rhs) {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_BINARY;
	self->string = strdup(op);
	self->nchild = 2;
	self->child[0] = lhs;
	self->child[1] = rhs;
	self->next = 0;
		
	return self;
}

expr_t *expr_unary(const char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_UNARY;
	self->string = strdup(op);
	self->nchild = 1;
	self->child[0] = expr;
	self->next = 0;

	return self;
}

expr_t *expr_call(expr_t *func, expr_t *args) {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_CALL;
	self->string = 0 
	self->nchild = 2;
	self->child[0] = func;
	self->child[1] = args;
	self->next = 0;
		
	return self;
}
	
expr_t *expr_index(expr_t *tuple, expr_t *expr) {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_INDEX;
	self->string = 0 
	self->nchild = 2;
	self->child[0] = tuple;
	self->child[1] = expr;
	self->next = 0;
		
	return self;
}

expr_t *expr_member(expr_t *expr, expr_t *ident) {
	expr_t *self = malloc(sizeof expr_t);

	self->type = EXPR_TYPE_MEMBER;
	self->string = 0;
	self->nchild = 2;
	self->child[0] = expr;
	self->child[1] = ident;
	self->next = 0;
	
	return self;
}

expr_t *expr_postfix(const char *op, expr_t *expr) {
	expr_t *self = malloc(sizeof expr_t);
	
	self->type = EXPR_TYPE_POSTFIX;
	self->string = strdup(op);
	self->nchild = 1;
	self->child[0] = expr;
	self->next = 0;

	return self;
}


void expr_free(expr_t *self) {
	if (self) {
		for (size_t i = 0; i < self->nchild) {
			expr_free(self->child[i]);	
		}
		expr_free(self->next);
		free(self->string);
		free(self);
	}
}
