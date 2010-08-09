#ifndef STMT_H
#define STMT_H

#include <expr.h>
#include <var.h>

#define STMT_TYPE_BLOCK 1
#define STMT_TYPE_EXPR 2
#define STMT_TYPE_UNTIL 3
#define STMT_TYPE_WHILE 4
#define STMT_TYPE_DOWHILE 5
#define STMT_TYPE_DOUNTIL 6
#define STMT_TYPE_FOR 7
#define STMT_TYPE_FOREACH 8
#define STMT_TYPE_CONDITIONAL 9
#define STMT_TYPE_DECL 10

/* Structure for holding stmt lists */
typedef struct stmt stmt_t;
struct stmt {
	int type;				/* Statement type */
	expr_t *expr;			/* Possible expression */
	var_t *var;				/* Possible variable object */
	size_t nchild;			/* Number of child stmts */
	stmt_t *child[4];		/* Child stmts */
	stmt_t *next; 
};

stmt_t *stmt_expr(expr_t *expr);
stmt_t *stmt_block();
stmt_t *stmt_append(stmt_t *self, stmt_t *stmt);
stmt_t *stmt_for(stmt_t *c1, stmt_t *c2, stmt_t *c3, stmt_t *block);
stmt_t *stmt_foreach(var_t *var, stmt_t *block);
stmt_t *stmt_until(stmt_t *guard, stmt_t *block);
stmt_t *stmt_while(stmt_t *guard, stmt_t *block);
stmt_t *stmt_dountil(stmt_t *block, stmt_t *guard);
stmt_t *stmt_dowhile(stmt_t *block, stmt_t *guard);
stmt_t *stmt_decl(var_t *var);
stmt_t *stmt_conditional(stmt_t *guard, stmt_t *br1, stmt_t *br2);
void stmt_free(stmt_t *self);

#endif
