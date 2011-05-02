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

#ifndef APVAR_H
#define APVAR_H

#include <apollo.h>
#include <aptype.h>
#include <aploc.h>

/* Local apvariable/symbol in the current scope */
struct apvar {
	apsymbol_t symbol;
	char *name;
	int flags;
	aploc_t loc;
	aptype_t *type;
	apexpr_t *expr;
	apvar_t *next;
};	

apvar_t *apvar_alloc(aploc_t *loc, char *name, aptype_t *type, apexpr_t *expr);
apvar_t *apvar_formal(aploc_t *loc, char *name, aptype_t *type, apvar_t *next);
int apvar_comp(apvar_t *self, apvar_t *apvar);
void apvar_free(apvar_t *self);



#endif
