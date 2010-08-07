#ifndef STATEMENT_H
#define STATEMENT_H


#define STATEMENT_TYPE_COMPOUND 1
#define STATEMENT_TYPE_SIMPLE 2
#define STATEMENT_TYPE_WHILE 3
#define STATEMENT_TYPE_DO 4
#define STATEMENT_TYPE_FOR 5
#define STATEMENT_TYPE_FOREACH 6
#define STATEMENT_TYPE_UNTIL 7
#define STATEMENT_TYPE_IF 8

/* Structure for holding statement lists */
typedef struct statement statement_t;
struct statement {
	int type;				/* Statement type */
	size_t nexpr;			/* Number of child expressions */
	size_t nchild;			/* Number of child statements */
	statement_t *child[2];	/* Child statements */
	expr_t *expr[3]			/* Possible expression */
	statement_t *next; 
}


statement_t *statement_simple();
statement_t *statement_list();
statement_t *statement_foreach(type_t *type, const char *ident, expr_t *up, 
	statement_t *block);
statement_t *statement_for(expr_t *init, expr_t *test, expr_t *up, 
	statement_t *block);
statement_t *statement_append(statement_t *self);
statement_t *statement_until(expr_t *test, statement_t *block);
statement_t *statement_while(expr_t *test, statement_t *block);
statement_t *statement_dountil(expr_t *test, statement_t *block);
statement_t *statement_dowhile(expr_t *test, statement_t *block);
statement_t *statement_decl(type_t *type, const char *ident, expr_t *init);

#endif
