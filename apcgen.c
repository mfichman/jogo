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

#include <apcgen.h>
#include <apunit.h>
#include <aptype.h>
#include <apvar.h>
#include <apexpr.h>
#include <apstmt.h>
#include <aptype.h>
#include <apfunc.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Structure for generating C code using the data from the given parser */
struct apcgen {
	apunit_t *unit;
	int indent;
	FILE *fd;
};

apcgen_t *apcgen_alloc() {
	apcgen_t *self = malloc(sizeof(apcgen_t));
	self->fd = 0;
	self->unit = 0;
	self->indent = 0;

	return self;
}

void apcgen_gen_unit(apcgen_t *self, apunit_t *unit, FILE *fd) {
	self->fd = fd;
	self->unit = unit;
	apcgen_print(self, "typedef struct ");
	apcgen_print_type(self, unit->name);
	apcgen_print(self, " ");
	apcgen_print_type(self, unit->name);
	apcgen_print(self, " {\n");
	for (apvar_t *var = unit->vars; var; var = var->next) {
		apcgen_print(self, "    ");
		apcgen_print_type(self, var->type);
	 	apcgen_print(self, " %s;\n", var->name);
	}
	apcgen_print(self, "};\n\n");

	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apcgen_gen_func(self, func);
	}
}

void apcgen_gen_func(apcgen_t *self, apfunc_t *func) {
	apcgen_print_type(self, func->rets);
	apcgen_print(self, " ");
	apcgen_print_type(self, self->unit->name);	
	apcgen_print(self, "_%s(", func->name);
	for (apvar_t *var = func->args; var; var = var->next) {
		apcgen_print_type(self, var->type);
		apcgen_print(self, " %s", var->name);
		if (var->next) {
			apcgen_print(self, ", ");
		}
	}
	apcgen_print(self, ") ");
	if (func->block) {
		apcgen_gen_stmt(self, func->block);
		apcgen_print(self, "\n\n");
	} else {
		apcgen_print(self, ";\n");
	}
}

void apcgen_indent(apcgen_t *self) {
	for (int i = 0; i < self->indent; i++) {
		fprintf(self->fd, "    ");
	}
}

void apcgen_gen_stmt(apcgen_t *self, apstmt_t *stmt) {

	switch (stmt->type) {
	case APSTMT_TYPE_BLOCK:
		apcgen_print(self, "{\n");
		self->indent++;
		for (apstmt_t *child = stmt->child[0]; child; child = child->next) {
			apcgen_indent(self);
			apcgen_gen_stmt(self, child);
			apcgen_print(self, "\n");
		}
		self->indent--;
		apcgen_indent(self);
		apcgen_print(self, "}");
		break;

	case APSTMT_TYPE_RETURN:
		apcgen_print(self, "return");
		if (stmt->expr) {
			apcgen_print(self, " ");
			apcgen_gen_expr(self, stmt->expr);
		}
		apcgen_print(self, ";");
		break;
	
	case APSTMT_TYPE_EXPR:
		apcgen_gen_expr(self, stmt->expr);
		apcgen_print(self, ";");
		break;

	case APSTMT_TYPE_DECL:
		apcgen_print_type(self, stmt->var->type);
		apcgen_print(self, " %s", stmt->var->name);
		if (stmt->var->expr) {
			apcgen_print(self, " = ");
			apcgen_gen_expr(self, stmt->expr);
		}
		apcgen_print(self, ";");
		break;


	case APSTMT_TYPE_UNTIL:
		apcgen_print(self, "while (!(");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, ")) ");
		apcgen_gen_stmt(self, stmt->child[1]);
		break;

	case APSTMT_TYPE_WHILE:
		apcgen_print(self, "while (");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, ") ");
		apcgen_gen_stmt(self, stmt->child[1]);
		break;

	case APSTMT_TYPE_DOUNTIL:
		apcgen_print(self, "do ");
		apcgen_gen_stmt(self, stmt->child[1]);
		apcgen_print(self, " while(!(");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, "));\n");
		break;

	case APSTMT_TYPE_DOWHILE:
		apcgen_print(self, "do ");
		apcgen_gen_stmt(self, stmt->child[1]);
		apcgen_print(self, " while(");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, ");\n");
		break;

	case APSTMT_TYPE_FOR:
		apcgen_print(self, "for (");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, "; ");
		apcgen_gen_expr(self, stmt->child[1]->expr);
		apcgen_print(self, "; ");
		apcgen_gen_expr(self, stmt->child[2]->expr);
		apcgen_print(self, ") ");
		apcgen_gen_stmt(self, stmt->child[3]);
		break;

	case APSTMT_TYPE_COND:
		apcgen_print(self, "if (");
		apcgen_gen_expr(self, stmt->child[0]->expr);
		apcgen_print(self, ") ");
		apcgen_gen_stmt(self, stmt->child[1]);
		if (stmt->child[2]) {
			apcgen_print(self, " else ");
			apcgen_gen_stmt(self, stmt->child[2]);
		}
		break;

	case APSTMT_TYPE_FOREACH:

		break;
	}

}

void apcgen_gen_expr(apcgen_t *self, apexpr_t *expr) {
	switch (expr->type) {
		case APEXPR_TYPE_STRING:
			apcgen_print(self, "\"%s\"", expr->string);
			break;
		
		case APEXPR_TYPE_UNARY:
			apcgen_print(self, "(");
			apcgen_print(self, expr->string);
			apcgen_gen_expr(self, expr->child[0]);
			apcgen_print(self, ")");
			break;

		case APEXPR_TYPE_BINARY:
			apcgen_print(self, "(");
			apcgen_gen_expr(self, expr->child[0]); 
			apcgen_print(self, expr->string);
			apcgen_gen_expr(self, expr->child[1]);
			apcgen_print(self, ")");
			break;
		
		case APEXPR_TYPE_VAR:
			apcgen_print(self, "%s", expr->string);
			break;

	}	
}

void apcgen_gen_var(apcgen_t *self, apvar_t *var) {
}

void apcgen_print_type(apcgen_t *self, aptype_t *type) {
	if (APTYPE_TYPE_PRIMITIVE == type->type) {
		fputs("ap", self->fd);
	}

	for (char *c = type->name; *c; c++) {
		if (*c == ':') {
			fputc('_', self->fd);
			c += 2;
		}
		fputc(*c, self->fd);
	}
}

void apcgen_print(apcgen_t *self, const char* fmt, ...) {
	va_list vargs;
	va_start(vargs, fmt);
	vfprintf(self->fd, fmt, vargs);
	va_end(vargs);
}

void apcgen_free(apcgen_t *self) {
}
