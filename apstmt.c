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

#include <apstmt.h>
#include <apvar.h>
#include <apexpr.h>
#include <apsymtab.h>
#include <apparser.h>
#include <stdlib.h>

apstmt_t *apstmt_expr(apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_EXPR;
	self->symbols = 0;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;

	return self;
}

apstmt_t *apstmt_block(apsymtab_t *symbols) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_BLOCK;
	self->symbols = apsymtab_alloc(symbols);
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = 0; /* Head of the list */
	self->child[1] = 0; /* Tail fo the list */
	self->next = 0;
	
	return self;
}

apstmt_t *apstmt_append(apstmt_t *self, apstmt_t *stmt) {

	if (!self->child[0]) {
		self->child[0] = stmt;
		self->child[1] = stmt;
	} else {
		self->child[1]->next = stmt;
		self->child[1] = stmt;
	}

	return self;
}

apstmt_t *apstmt_for(apstmt_t *c1, apstmt_t *c2, apstmt_t *c3, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_FOR;
	self->symbols = 0;
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

apstmt_t *apstmt_foreach(apvar_t *var, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));
	
	self->type = APSTMT_TYPE_FOREACH;	
	self->symbols = 0;
	self->expr = 0;
	self->var = var;
	self->nchild = 1;
	self->child[0] = block;
	self->next = 0;
	
	return self;
}

apstmt_t *apstmt_until(apstmt_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_UNTIL;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

apstmt_t *apstmt_while(apstmt_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_WHILE;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

apstmt_t *apstmt_dountil(apstmt_t *block, apstmt_t *guard) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DOUNTIL;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

apstmt_t *apstmt_dowhile(apstmt_t *block, apstmt_t *guard) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DOWHILE;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;

	return self;
}

apstmt_t *apstmt_decl(apparser_t *parser, apvar_t *var) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DECL;
	self->symbols = 0;
	self->expr = 0;
	self->var = var;
	self->nchild = 0;
	self->next = 0;

	apsymtab_var(parser->symbols, var->name, var);

	return self;
}

apstmt_t *apstmt_conditional(apstmt_t *guard, apstmt_t *br1, apstmt_t *br2) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_CONDITIONAL;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	if (br2) {
		self->nchild = 3;
		self->child[0] = guard;
		self->child[1] = br1;
		self->child[2] = br2;
	} else {
		self->nchild = 2;
		self->child[0] = guard;
		self->child[1] = br1;
	}
	self->next = 0;
	
	return self;
}

apstmt_t *apstmt_return(apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_RETURN;
	self->symbols = 0;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;

	return self;
}


void apstmt_free(apstmt_t *self) {
 	if (self) {
		apexpr_free(self->expr);
		apvar_free(self->var);
		if (APSTMT_TYPE_BLOCK == self->type) {
			apstmt_free(self->child[0]);
		} else {
			for (size_t i = 0; i < self->nchild; i++) {
				apstmt_free(self->child[i]);
			}
		}
		apsymtab_free(self->symbols);
		apstmt_free(self->next);
		free(self);
	}
}






