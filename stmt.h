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

#ifndef STMT_H
#define STMT_H

#include <apollo.h>

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
#define STMT_TYPE_RETURN 11

/* Structure for holding stmt lists */
struct stmt {
	int type;				/* Statement type */
	symtab_t *symbols;		/* Symbol lookup table */
	expr_t *expr;			/* Possible expression */
	var_t *var;				/* Possible variable object */
	int nchild;				/* Number of child stmts */
	stmt_t *child[4];		/* Child stmts */
	stmt_t *next; 
};

stmt_t *stmt_expr(expr_t *expr);
stmt_t *stmt_block(symtab_t *symbols);
stmt_t *stmt_append(stmt_t *self, stmt_t *stmt);
stmt_t *stmt_for(stmt_t *c1, stmt_t *c2, stmt_t *c3, stmt_t *block);
stmt_t *stmt_foreach(var_t *var, stmt_t *block);
stmt_t *stmt_until(stmt_t *guard, stmt_t *block);
stmt_t *stmt_while(stmt_t *guard, stmt_t *block);
stmt_t *stmt_dountil(stmt_t *block, stmt_t *guard);
stmt_t *stmt_dowhile(stmt_t *block, stmt_t *guard);
stmt_t *stmt_decl(parser_t *parser, var_t *var);
stmt_t *stmt_conditional(stmt_t *guard, stmt_t *br1, stmt_t *br2);
stmt_t *stmt_return(expr_t *expr);
void stmt_free(stmt_t *self);

#endif
