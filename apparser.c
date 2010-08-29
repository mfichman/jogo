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
#include <apsymtab.h>
#include <apfunc.h>
#include <apunit.h>
#include <aphash.h>
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
	aphash_compfn_t comp = (aphash_compfn_t)&aptype_comp;
	aphash_hashfn_t hash = (aphash_hashfn_t)&aptype_hash;

	self->units = 0;
	self->filename = 0;
	self->fd = 0;
	self->column = 1;
	self->symbols = 0;
	self->types = aphash_alloc(comp, hash);
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

void apparser_parse(apparser_t *self, const char *filename, int fd) {
	self->fd = fd;
	self->column = 1;
	self->symbols = 0;
	self->filename = realloc(self->filename, strlen(filename) + 1);
	strcpy(self->filename, filename);

	yyparse(self, self->scanner);

	for (apunit_t *unit = self->units; unit; unit = unit->next) {
	}
}

int apparser_read(apparser_t *self, char *buffer, int length) {
	return read(self->fd, buffer, length);
}

void apparser_class(apparser_t *self, apunit_t *unit) {
	unit->symbols = self->symbols;
	unit->next = self->units;
	self->units = unit;
	self->symbols = unit->symbols;
	aphash_put(self->types, unit->name, unit);
	apparser_check_unit(self, unit);
}

void apparser_interface(apparser_t *self, apunit_t *unit) {
	unit->symbols = self->symbols;
	unit->next = self->units;
	self->units = unit;
	self->symbols = unit->symbols;
	aphash_put(self->types, unit->name, unit);
	apparser_check_unit(self, unit);
}

void apparser_struct(apparser_t *self, apunit_t *unit) {
	unit->symbols = self->symbols;
	unit->next = self->units;
	self->units = unit;
	self->symbols = 0;
	self->symbols = unit->symbols;
	aphash_put(self->types, unit->name, unit);
	apparser_check_unit(self, unit);
}

void apparser_module(apparser_t *self, apunit_t *unit) {
	unit->symbols = self->symbols;
	unit->next = self->units;
	self->units = unit;
	self->symbols = unit->symbols;
	aphash_put(self->types, unit->name, unit);
	apparser_check_unit(self, unit);
}

void apparser_check_unit(apparser_t *self, apunit_t *unit) {
	
	apfunc_t* func = unit->funcs;
	while (func) {
		apparser_check_func(self, func);
		func = func->next;
	}
}

void apparser_check_func(apparser_t *self, apfunc_t *func) {

	if (func->block) {
		apparser_check_stmt(self, func->block);
	}	

	/* TODO: check return types */			
	
}

void apparser_check_stmt(apparser_t *self, apstmt_t *stmt) {
	apsymtab_t *symbols;

	switch (stmt->type) {
	case APSTMT_TYPE_BLOCK:
		symbols = self->symbols;
		self->symbols = stmt->symbols;
		for (apstmt_t *child = stmt->child[0]; child; child = child->next) {
			apparser_check_stmt(self, child);
		}
		self->symbols = symbols;
		break;

	case APSTMT_TYPE_RETURN:
	case APSTMT_TYPE_EXPR:
		apparser_check_expr(self, stmt->expr);
		break;

	case APSTMT_TYPE_DECL:
		apparser_check_var(self, stmt->var);
		break;

	case APSTMT_TYPE_UNTIL:
	case APSTMT_TYPE_WHILE:
	case APSTMT_TYPE_DOWHILE:
	case APSTMT_TYPE_DOUNTIL:
	case APSTMT_TYPE_FOR:
	case APSTMT_TYPE_COND:
		for (int i = 0; i < stmt->nchild; i++) {
			apparser_check_stmt(self, stmt->child[i]);
		}
		break;

	case APSTMT_TYPE_FOREACH:
		apparser_check_var(self, stmt->var);
		apparser_check_stmt(self, stmt->child[0]);
		break;
	}
}

void apparser_check_expr(apparser_t *self, apexpr_t *expr) {
	switch (expr->type) {
		case APEXPR_TYPE_STRING:
			break;
	
		case APEXPR_TYPE_UNARY:
			apparser_check_expr(self, expr->child[0]);
			expr->chktype = aptype_clone(expr->child[0]->chktype);
			break;

		case APEXPR_TYPE_BINARY:
			apparser_check_expr_binary(self, expr);
			break;	
	
		case APEXPR_TYPE_VAR:
			apparser_check_expr_var(self, expr);
			break;

		case APEXPR_TYPE_SCALL:
		case APEXPR_TYPE_CTOR:
			apparser_check_expr_scall(self, expr);
			break;

		case APEXPR_TYPE_MCALL:
			apparser_check_expr_mcall(self, expr);	
			break;
			
		case APEXPR_TYPE_INDEX:
		case APEXPR_TYPE_MEMBER:
		case APEXPR_TYPE_STATIC:
			break;
			
	}
}

void apparser_check_expr_binary(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[0]);
	apparser_check_expr(self, expr->child[1]);
	if (!expr->child[0]->chktype || !expr->child[1]->chktype) {
		return;
	}

	if (aptype_comp(expr->child[0]->chktype, expr->child[1]->chktype)) {
		apparser_print_loc(self, &expr->loc); 
		apparser_error(self, "Cannot apply '%s' ", expr->string);
		apparser_error(self, "to '%s' ", expr->child[0]->chktype->name);
		apparser_error(self, "and '%s'\n", expr->child[1]->chktype->name);
		return;
	}
	expr->chktype = aptype_clone(expr->child[0]->chktype);
}

void apparser_check_expr_var(apparser_t *self, apexpr_t *expr) {
	apvar_t *var = apsymtab_get_var(self->symbols, expr->string);	
	if (!var) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared variable '%s'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(var->type);
}

void apparser_check_expr_scall(apparser_t *self, apexpr_t *expr) {
	apunit_t *unit = (apunit_t *)aphash_get(self->types, expr->clstype);
	if (!unit) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Unknown type '%s'\n", expr->clstype->name);
		return;
	}

	apfunc_t *func = apsymtab_get_func(unit->symbols, expr->string);
	if (!func) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared function '%s'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(func->rets);

	apvar_t *var = func->args;
	for (apexpr_t *child = expr->child[0]; child; child = child->next) {
		apparser_check_expr(self, child);	
		printf("Here");
		if (!var || aptype_comp(var->type, child->chktype)) {
		printf("er");
			goto error;
		}
		var = var->next;
	}
	if (var) {
		goto error;
	}
	return;

error:
	apparser_print_loc(self, &expr->loc);
	apparser_error(self, "Cannot apply '%s(", func->name);
	for (apvar_t *var = func->args; var; var = var->next) {
		if (var->next) {
			apparser_error(self, "%s,", var->type->name);
		} else {
			apparser_error(self, "%s", var->type->name);
		}
	}
	apparser_error(self, ")' to '(");

	for (apexpr_t *child = expr->child[0]; child; child = child->next) {
		if (child->next) {
			apparser_error(self, "%s,", child->chktype->name);
		} else {
			apparser_error(self, "%s", child->chktype->name);
		}
	}
	apparser_error(self, ")'\n");
}

void apparser_check_expr_mcall(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[1]);
	aptype_t *type = expr->child[1]->chktype;
	apunit_t *unit = (apunit_t *)aphash_get(self->types, type);
	if (!unit) {
		return;
	}

	apfunc_t *func = apsymtab_get_func(unit->symbols, expr->string);
	if (!func) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared function '%s'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(func->rets);
	
	apvar_t *var = func->args;
	int argno = 1;
	for (apexpr_t *child = expr->child[0]; child; child = child->next) {
		apparser_check_expr(self, child);	
		if (!var || aptype_comp(var->type, child->chktype)) {
			goto error;
		}
		var = var->next;
		argno++;
	}
	if (var) {
		goto error;
	}
	return;

error:
	apparser_print_loc(self, &expr->loc);
	apparser_error(self, "Cannot apply '%s(", func->name);
	for (apvar_t *var = func->args; var; var = var->next) {
		if (var->next) {
			apparser_error(self, "%s,", var->type->name);
		} else {
			apparser_error(self, "%s", var->type->name);
		}
	}
	apparser_error(self, ")' to '(");

	for (apexpr_t *child = expr->child[0]; child; child = child->next) {
		if (child->next) {
			apparser_error(self, "%s,", child->chktype->name);
		} else {
			apparser_error(self, "%s", child->chktype->name);
		}
	}
	apparser_error(self, ")'\n");
}

void apparser_check_var(apparser_t *self, apvar_t *var) {

	if (var->expr) {
		apparser_check_expr(self, var->expr);
		if (var->expr->chktype && aptype_comp(var->type, var->expr->chktype)) {
			apparser_print_loc(self, &var->expr->loc);
			apparser_error(self, "Incompatible type in assignment ");
			apparser_error(self, "(got '%s', ", var->expr->chktype->name);
			apparser_error(self, "expected '%s')\n", var->type->name);
		}
	}
} 

int apparser_resolve_type(apparser_t *self, aptype_t *type) {

	return 0;
}

void apparser_print_loc(apparser_t *self, aploc_t *loc) {
	int line = loc->first_line;
	int column = loc->first_column;

	fprintf(stderr, "%s:%d:%d: ", self->filename, line, column);
}

void apparser_error(apparser_t *self, const char *fmt, ...) { 
	va_list vargs;
	va_start(vargs, fmt);

	vfprintf(stderr, fmt, vargs);
	self->error = 1;

	va_end(vargs);
}

void apparser_free(apparser_t *self) {
	if (self) {
		yylex_destroy(self->scanner);
		apunit_free(self->units);
		aphash_free(self->types);
		//apsymtab_free(self->symbols);
		free(self->filename);
		free(self);
	}
}
