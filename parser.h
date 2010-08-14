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

#ifndef PARSER_H
#define PARSER_H

#include <apollo.h>

/* Parser structure */
struct parser {
	unit_t *units;
	int fd;
	char *filename;
	void *scanner;
};

parser_t *parser_alloc();
void parser_parse(parser_t *self, const char* filename, int fd);
void parser_class(parser_t *self, unit_t *unit);
void parser_interface(parser_t *self, unit_t *unit);
void parser_struct(parser_t *self, unit_t *unit);
void parser_module(parser_t *self, unit_t *unit);
int parser_read(parser_t *self, char *buffer, int length);
void parser_free(parser_t *self);


typedef union node node_t;
union node {
	expr_t *expr; 
	stmt_t *stmt;
	type_t *type;
	unit_t *unit;
	var_t *var;
	func_t *func;
	import_t *import;
	def_t *def;
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
