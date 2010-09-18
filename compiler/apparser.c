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

#include <apparser.h>
#include <apchecker.h>
#include <apsymtab.h>
#include <apunit.h>
#include <aphash.h>
#include <aptype.h>
#include <apfunc.h>
#include <apvar.h>
#include <apimport.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int yyparse(apparser_t *parser, void *scanner);
int yylex_init(void **);
int yylex_destroy(void *);
void yyset_extra(apparser_t *, void *);

apparser_t *apparser_alloc() {
	apparser_t *self = malloc(sizeof(apparser_t));

	self->unit = 0;
	self->fd = 0;
	self->column = 1;
	self->error = 0;
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

int apparser_parse(apparser_t *self, apunit_t *unit) {
	fprintf(stderr, "Parsing %s\n", unit->filename);

	/* Open the import class/module/interface file and parse it */
	self->unit = unit;
	self->column = 1;
	self->fd = fopen(unit->filename, "r");
	if (!self->fd) {
		self->error++;
		fprintf(stderr, "Could not find '%s'\n", self->unit->filename);	
		return self->error;
	}

	/* Begin parsing */
	yyparse(self, self->scanner);
	apparser_newline(self);
	fclose(self->fd);

	/* Add implicit 'self' arguments */
	if (APUNIT_TYPE_MODULE != self->unit->type) {	
		for (apfunc_t *func = self->unit->funcs; func; func = func->next) {
			if (APUNIT_FLAG_STATIC & ~func->flags) {
				aptype_t *type = aptype_object(strdup(self->unit->name));
				apvar_t *arg = apvar_alloc(&func->loc, strdup("self"), type);	
				arg->next = func->args;
				func->args = arg;
			}
		}
	}

	return self->error;
}

int apparser_read(apparser_t *self, char *buffer, int length) {
	return fread(buffer, sizeof(char), length, self->fd);
}

void apparser_free(apparser_t *self) {
	if (self) {
		yylex_destroy(self->scanner);
		free(self);
	}
}

void yyerror(aploc_t *loc, apparser_t *self, void* scanner, const char *msg) {
	self->error++;
	aploc_print(loc, self->unit->filename, stderr);
	fprintf(stderr, "%c%s\n", toupper(msg[0]), msg + 1);
}
