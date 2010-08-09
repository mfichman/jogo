#ifndef PARSER_H
#define PARSER_H

#include <type.h>
#include <func.h>
#include <unit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Parser structure */
typedef struct parser parser_t;
struct parser {
	unit_t *units;
	int fd;
	void *scanner;
};

parser_t *parser_alloc();
void parser_parse(parser_t *self, int fd);
void parser_class(parser_t *self, unit_t *unit);
void parser_interface(parser_t *self, unit_t *unit);
void parser_struct(parser_t *self, unit_t *unit);
void parser_module(parser_t *self, unit_t *unit);
int parser_read(parser_t *self, char *buffer, size_t length);
void parser_free(parser_t *self);


typedef union node node_t;
union node {
	expr_t *expr; 
	stmt_t *stmt;
	type_t *type;
	unit_t *unit;
	var_t *var;
	func_t *func;
	char *string;	
	int null;
	int flag;
};

#define YYSTYPE node_t
#define YY_EXTRA_TYPE parser_t *
#define YY_NO_INPUT
#define YYERROR_VERBOSE

int yylex(node_t *node, void *scanner);
int yyparse(parser_t *parser, void *scanner);
void yyerror(parser_t *parser, void *scanner, const char *message);
int yylex_init(void **);
int yylex_destroy(void *);
void yyset_extra(parser_t *, void *);
/*

int yylex(YYSTYPE *, void *);
int yyparse(parser_t *, void *);
int yyerror(parser_t *, void *, const char *error);
*/

#endif
