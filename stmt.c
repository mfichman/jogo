#include <stmt.h>
#include <stdlib.h>

stmt_t *stmt_expr(expr_t *expr) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_EXPR;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;

	return self;
}

stmt_t *stmt_block() {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_BLOCK;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = 0; /* Head of the list */
	self->child[1] = 0; /* Tail fo the list */
	self->next = 0;
	
	return self;
}

stmt_t *stmt_append(stmt_t *self, stmt_t *stmt) {

	if (!self->child[0]) {
		self->child[0] = stmt;
		self->child[1] = stmt;
	} else {
		self->child[1]->next = stmt;
		self->child[1] = stmt;
	}

	return self;
}

stmt_t *stmt_for(stmt_t *c1, stmt_t *c2, stmt_t *c3, stmt_t *block) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_FOR;
	self->expr = 0;
	self->var = 0;
	self->nchild = 4;
	self->child[0] = c1; /* Initializer */
	self->child[1] = c2; /* Guard */
	self->child[2] = c3; /* Increment */
	self->child[3] = block;	/* Block */
	self->next = 0;

	return self;
}

stmt_t *stmt_foreach(var_t *var, stmt_t *block) {
	stmt_t *self = malloc(sizeof(stmt_t));
	
	self->type = STMT_TYPE_FOREACH;	
	self->expr = 0;
	self->var = var;
	self->nchild = 1;
	self->child[0] = block;
	self->next = 0;
	
	return self;
}

stmt_t *stmt_until(stmt_t *guard, stmt_t *block) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_UNTIL;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

stmt_t *stmt_while(stmt_t *guard, stmt_t *block) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_WHILE;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

stmt_t *stmt_dountil(stmt_t *block, stmt_t *guard) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_DOUNTIL;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

stmt_t *stmt_dowhile(stmt_t *block, stmt_t *guard) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_DOWHILE;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

stmt_t *stmt_decl(var_t *var) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_DECL;
	self->expr = 0;
	self->var = var;
	self->nchild = 0;
	self->next = 0;

	return self;
}

stmt_t *stmt_conditional(stmt_t *guard, stmt_t *br1, stmt_t *br2) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_CONDITIONAL;
	self->expr = 0;
	self->var = 0;
	self->nchild = 3;
	self->child[0] = guard;
	self->child[1] = br1;
	self->child[2] = br2;
	self->next = 0;
	
	return self;
}

stmt_t *stmt_return(expr_t *expr) {
	stmt_t *self = malloc(sizeof(stmt_t));

	self->type = STMT_TYPE_RETURN;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;

	return self;
}


void stmt_free(stmt_t *self) {
 	if (self) {
		expr_free(self->expr);
		var_free(self->var);
		if (STMT_TYPE_BLOCK == self->type) {
			stmt_free(self->child[0]);
		} else {
			for (size_t i = 0; i < self->nchild; i++) {
				stmt_free(self->child[i]);
			}
		}
		stmt_free(self->next);
	}
}






