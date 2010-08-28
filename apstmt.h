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

#ifndef APSTMT_H
#define APSTMT_H

#include <apollo.h>

#define APSTMT_TYPE_BLOCK 1
#define APSTMT_TYPE_EXPR 2
#define APSTMT_TYPE_UNTIL 3
#define APSTMT_TYPE_WHILE 4
#define APSTMT_TYPE_DOWHILE 5
#define APSTMT_TYPE_DOUNTIL 6
#define APSTMT_TYPE_FOR 7
#define APSTMT_TYPE_FOREACH 8
#define APSTMT_TYPE_CONDITIONAL 9
#define APSTMT_TYPE_DECL 10
#define APSTMT_TYPE_RETURN 11

/* Structure for holding stmt lists */
struct apstmt {
	int type;				/* Statement type */
	apsymtab_t *symbols;		/* Symbol lookup table */
	apexpr_t *expr;			/* Possible expression */
	apvar_t *var;				/* Possible variable object */
	int nchild;				/* Number of child stmts */
	apstmt_t *child[4];		/* Child stmts */
	apstmt_t *next; 
};

apstmt_t *apstmt_expr(apexpr_t *expr);
apstmt_t *apstmt_block(apsymtab_t *symbols);
apstmt_t *apstmt_append(apstmt_t *self, apstmt_t *stmt);
apstmt_t *apstmt_for(apstmt_t *c1, apstmt_t *c2, apstmt_t *c3, apstmt_t *block);
apstmt_t *apstmt_foreach(apvar_t *var, apstmt_t *block);
apstmt_t *apstmt_until(apstmt_t *guard, apstmt_t *block);
apstmt_t *apstmt_while(apstmt_t *guard, apstmt_t *block);
apstmt_t *apstmt_dountil(apstmt_t *block, apstmt_t *guard);
apstmt_t *apstmt_dowhile(apstmt_t *block, apstmt_t *guard);
apstmt_t *apstmt_decl(apparser_t *parser, apvar_t *var);
apstmt_t *apstmt_conditional(apstmt_t *guard, apstmt_t *br1, apstmt_t *br2);
apstmt_t *apstmt_return(apexpr_t *expr);
void apstmt_free(apstmt_t *self);

#endif