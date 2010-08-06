#ifndef EXPR_H
#define EXPR_H



typedef struct type type_t;
struct type {
	char *name;             /* Fully qualified type name */
};
type_t *type_alloc(const char *name);
void type_free(type_t *self);
int type_comp(type_t *self, type_t *other);


#define EXPR_TYPE_STRING 1
#define EXPR_TYPE_PAIR 2
#define EXPR_TYPE_LIST 3
#define EXPR_TYPE_UNARY 4
#define EXPR_TYPE_BINARY 5
#define EXPR_TYPE_CALL 6
#define EXPR_TYPE_INDEX 7
#define EXPR_TYPE_POSTFIX 8
#define EXPR_TYPE_MEMBER 9

typedef struct expr expr_t;
struct expr {
	int type				/* Node type */
	const char *string;		/* Possible string value of the expression */
	expr_t *child;          /* Child expressions */
	expr_t *next;
};


expr_t *expr_string(const char *string);
expr_t *expr_strcat2(expr_t *self, const char *string, expr_t *expr);
expr_t *expr_prepend(expr_t *self, expr_t *expr);
expr_t *expr_list();
expr_t *expr_pair(expr_t *left, expr_t *right);
expr_t *expr_binary(const char *op, expr_t *lhs, expr_t *rhs);
expr_t *expr_unary(const char *op, expr_t *expr);
expr_t *expr_call(expr_t *func, expr_t *args);
expr_t *expr_index(expr_t *tuple, expr_t *expr);
expr_t *expr_member(expr_t *expr, expr_t *ident);
expr_t *expr_postfix(const char *op, expr_t *expr);
void expr_free(expr_t *self);



#endif
