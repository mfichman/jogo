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

apstmt_t *apstmt_expr(aploc_t *loc, apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_EXPR;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_block(aploc_t *loc) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_BLOCK;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = 0; /* Head of the list */
	self->child[1] = 0; /* Tail fo the list */
	self->next = 0;
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_for(aploc_t *loc, apstmt_t *gd[3], apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_FOR;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 4;
	self->child[0] = gd[0]; /* Initializer */
	self->child[1] = gd[1]; /* Guard */
	self->child[2] = gd[2]; /* Increment */
	self->child[3] = block;	/* Block */
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_foreach(aploc_t *loc, apvar_t *var, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));
	
	self->type = APSTMT_TYPE_FOREACH;	
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = var;
	self->nchild = 1;
	self->child[0] = block;
	self->next = 0;
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_until(aploc_t *loc, apstmt_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_UNTIL;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_while(aploc_t *loc, apstmt_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_WHILE;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_dountil(aploc_t *loc, apstmt_t *block, apstmt_t *guard) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DOUNTIL;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_dowhile(aploc_t *loc, apstmt_t *block, apstmt_t *guard) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DOWHILE;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = 0;
	self->nchild = 2;
	self->child[0] = guard;
	self->child[1] = block;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_decl(aploc_t *loc, apvar_t *var) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DECL;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = 0;
	self->var = var;
	self->nchild = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_cond(aploc_t *loc, apstmt_t *guard, apstmt_t *br1, apstmt_t *br2) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_COND;
	self->chktype = 0;
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
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_return(aploc_t *loc, apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_RETURN;
	self->chktype = 0;
	self->symbols = 0;
	self->expr = expr;
	self->var = 0;
	self->nchild = 0;
	self->next = 0;
	self->loc = *loc;

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
		aptype_free(self->chktype);
		free(self);
	}
}
