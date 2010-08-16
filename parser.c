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

#include <parser.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <unit.h>
#include <stmt.h>
#include <type.h>
#include <var.h>
#include <expr.h>
#include <assert.h>

parser_t *parser_alloc() {
	parser_t *self = malloc(sizeof(parser_t));

	self->units = 0;
	self->filename = 0;
	self->fd = 0;
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

void parser_parse(parser_t *self, const char *filename, int fd) {
	self->fd = fd;
	self->filename = realloc(self->filename, strlen(filename) + 1);
	strcpy(self->filename, filename);

	yyparse(self, self->scanner);
}

void parser_error(parser_t *self) {
	yyerror(self, self->scanner, 0);

}

int parser_read(parser_t *self, char *buffer, int length) {
	return read(self->fd, buffer, length);
}

void parser_class(parser_t *self, unit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void parser_interface(parser_t *self, unit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void parser_struct(parser_t *self, unit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

void parser_module(parser_t *self, unit_t *unit) {
	unit->next = self->units;
	self->units = unit;
}

int parser_check_stmt(parser_t *self, stmt_t *stmt) {
	return 1;
}

int parser_check_expr(parser_t *self, expr_t *expr) {
	return 1;
}

int parser_check_var(parser_t *self, var_t *var) {
	return 1;
} 

int parser_resolve_type(type_t *type) {

	return 1;
}


void parser_free(parser_t *self) {
	if (self) {
		yylex_destroy(self->scanner);
		unit_free(self->units);
		free(self->filename);
		free(self);
	}
}

