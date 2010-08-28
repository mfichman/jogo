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
#include <apunit.h>
#include <apstmt.h>
#include <aptype.h>
#include <apvar.h>
#include <apexpr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int yyparse(apparser_t *parser, void *scanner);
int yylex_init(void **);
int yylex_destroy(void *);
void yyset_extra(apparser_t *, void *);

apparser_t *apparser_alloc() {
	apparser_t *self = malloc(sizeof(apparser_t));

	self->units = 0;
	self->filename = 0;
	self->fd = 0;
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

void apparser_parse(apparser_t *self, const char *filename, int fd) {
	self->fd = fd;
	self->filename = realloc(self->filename, strlen(filename) + 1);
	strcpy(self->filename, filename);

	yyparse(self, self->scanner);
}

int apparser_read(apparser_t *self, char *buffer, int length) {
	return read(self->fd, buffer, length);
}

void apparser_class(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void apparser_interface(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void apparser_struct(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void apparser_module(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

int apparser_check_stmt(apparser_t *self, apstmt_t *stmt) {
	return 1;
}

int apparser_check_expr(apparser_t *self, apexpr_t *expr) {
	return 1;
}

int apparser_check_var(apparser_t *self, apvar_t *var) {
	return 1;
} 

int apparser_resolve_type(aptype_t *type) {

	return 1;
}


void apparser_free(apparser_t *self) {
	if (self) {
		yylex_destroy(self->scanner);
		apunit_free(self->units);
		free(self->filename);
		free(self);
	}
}

