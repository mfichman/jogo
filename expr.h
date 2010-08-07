#ifndef EXPR_H
#define EXPR_H

#define EXPR_TYPE_STRING 1
#define EXPR_TYPE_UNARY 2 
#define EXPR_TYPE_BINARY 3 
#define EXPR_TYPE_CALL 4
#define EXPR_TYPE_INDEX 5
#define EXPR_TYPE_POSTFIX 6
#define EXPR_TYPE_MEMBER 7

/* Structure for holding expression syntax tree */
typedef struct expr expr_t;
struct expr {
	int type				/* Expression type type */
	const char *string;		/* Possible string value of the expression */
	size_t nchild;			/* Number of children */
	expr_t *child[2];      	/* Child expressions */
	expr_t *next;
};


expr_t *expr_string(const char *string);
expr_t *expr_binary(const char *op, expr_t *lhs, expr_t *rhs);
expr_t *expr_unary(const char *op, expr_t *expr);
expr_t *expr_call(expr_t *func, expr_t *args);
expr_t *expr_index(expr_t *tuple, expr_t *expr);
expr_t *expr_member(expr_t *expr, expr_t *ident);
expr_t *expr_postfix(const char *op, expr_t *expr);
void expr_free(expr_t *self);



#endif
