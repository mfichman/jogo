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
#include <assert.h>


apstmt_t *apstmt_expr(aploc_t *loc, apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_EXPR;
	self->chktype = 0;
	self->expr = expr;
	self->var = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_case(aploc_t *loc, apexpr_t *expr, apstmt_t *block) {
    apstmt_t *self = malloc(sizeof(apstmt_t));

    self->type = APSTMT_TYPE_CASE;
    self->chktype = 0;
    self->expr = expr;
    self->var = 0;
    self->child1 = block;
    self->child2 = 0;
    self->next = 0;
    self->loc = *loc;

    return self;
}

apstmt_t *apstmt_when(aploc_t *loc, apvar_t *var, apstmt_t *block) {
    apstmt_t *self = malloc(sizeof(apstmt_t));

    self->type = APSTMT_TYPE_WHEN;
    self->chktype = 0;
    self->expr = 0;
    self->var = var;
    self->child1 = block;
    self->child2 = 0;
    self->next = 0;
    self->loc = *loc;

    return self;
}

apstmt_t *apstmt_block(aploc_t *loc) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_BLOCK;
	self->chktype = 0;
	self->expr = 0;
	self->var = 0;
	self->child1 = 0; /* Head of the list */
	self->child2 = 0; /* Tail fo the list */
	self->next = 0;
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_for(aploc_t *loc, apvar_t *var, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));
	
	self->type = APSTMT_TYPE_FOR;	
	self->chktype = 0;
	self->expr = 0;
	self->var = var;
	self->child1 = block;
    self->child2 = 0;
	self->next = 0;
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_until(aploc_t *loc, apexpr_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_UNTIL;
	self->chktype = 0;
	self->expr = guard;
	self->var = 0;
	self->child1 = block;
    self->child2 = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_while(aploc_t *loc, apexpr_t *guard, apstmt_t *block) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_WHILE;
	self->chktype = 0;
	self->expr = guard;
	self->var = 0;
	self->child1 = block;
    self->child2 = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_decl(aploc_t *loc, apvar_t *var) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_DECL;
	self->chktype = 0;
	self->expr = 0;
	self->var = var;
    self->child1 = 0;
    self->child2 = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_cond(aploc_t *loc, apexpr_t *guard, apstmt_t *br1, apstmt_t *br2) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_COND;
	self->chktype = 0;
	self->expr = guard;
	self->var = 0;
    self->child1 = br1;
	if (br2) {
		self->child2 = br2;
	} else {
        self->child2 = 0;
	}
	self->next = 0;
	self->loc = *loc;
	
	return self;
}

apstmt_t *apstmt_return(aploc_t *loc, apexpr_t *expr) {
	apstmt_t *self = malloc(sizeof(apstmt_t));

	self->type = APSTMT_TYPE_RETURN;
	self->chktype = 0;
	self->expr = expr;
	self->var = 0;
    self->child1 = 0;
    self->child2 = 0;
	self->next = 0;
	self->loc = *loc;

	return self;
}

apstmt_t *apstmt_empty(aploc_t *loc) {
    apstmt_t *self = malloc(sizeof(apstmt_t));
    
    self->type = APSTMT_TYPE_EMPTY;
    self->chktype = 0;
    self->expr = 0;
    self->var = 0;
    self->child1 = 0;
    self->child2 = 0;
    self->next = 0;
    self->loc = *loc;

    return self;
}

apstmt_t *apstmt_append(apstmt_t *self, apstmt_t *stmt) {
    // The first argument is a block, the second argument
    // is a statement to append to the block
    
    assert(stmt);

	if (!self->child1) {
		self->child1 = stmt;
		self->child2 = stmt;
	} else {
		self->child2->next = stmt;
		self->child2 = stmt;
	}

	return self;
}


void apstmt_free(apstmt_t *self) {
 	if (self) {
		apexpr_free(self->expr);
		apvar_free(self->var);
	    apstmt_free(self->child1);
        if (APSTMT_TYPE_BLOCK == self->type) {
            self->child2 = 0;
        } else {
            apstmt_free(self->child2);
        }
		apstmt_free(self->next);
		aptype_free(self->chktype);
		free(self);
	}
}
