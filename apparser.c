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
#include <apimport.h>
#include <apvar.h>
#include <apexpr.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
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
	self->types = aphash_alloc(comp, hash);
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

int apparser_parse(apparser_t *self, const char *filename) {
	apparser_parse_unit(self, filename);

	while (self->queue) {
		apimport_t *import = self->queue;
		self->queue = self->queue->next;

		char *filename = malloc(strlen(import->type->name) + strlen(".ap"));
		char *in = import->type->name;
		char *out = filename;
		while (*in) {
			if (*in == ':') {
				*out++ = '/';
				in += 2;
			} else {
				*out++ = *in++;
			}
		}
		*out = 0;
		strcat(out, ".ap");

		apparser_parse_unit(self, filename);
		apimport_free(import);
		free(filename);
	}

	for (apunit_t *unit = self->units; unit; unit = unit->next) {
		apparser_check_unit(self, unit);
	}

	return self->error;
}

int apparser_parse_unit(apparser_t *self, const char *filename) {
	fprintf(stderr, "Parsing %s\n", filename);
	self->filename = realloc(self->filename, strlen(filename) + 1);
	strcpy(self->filename, filename);
	self->fd = fopen(self->filename, "r");
	if (!self->fd) {
		apparser_error(self, "Could not find '%s'\n", self->filename);	
		return self->error;
	}
	self->column = 1;
	self->symbols = 0;
	yyparse(self, self->scanner);
	apparser_reset_line(self);
	fclose(self->fd);

	apunit_t *unit = self->units;
	for (apimport_t *import = unit->imports; import; import = import->next) {
		if (!aphash_get(self->types, import->type)) {
			apimport_t *next = apimport_clone(import);
			next->next = self->queue;
			self->queue = next;
		}
	}

	return self->error;
}

int apparser_read(apparser_t *self, char *buffer, int length) {
	return fread(buffer, sizeof(char), length, self->fd);
}

void apparser_class(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
	aphash_put(self->types, unit->name, unit);

	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		if (APUNIT_FLAG_MEMBER & func->flags) {
			/* Add the implicit 'self' argument */
			aptype_t *type = aptype_clone(unit->name);
			apvar_t *arg = apvar_alloc(strdup("self"), 0, type, 0);	
			arg->next = func->args;
			func->args = arg;
		}
	}
}

void apparser_interface(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
	aphash_put(self->types, unit->name, unit);
}

void apparser_struct(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
	aphash_put(self->types, unit->name, unit);

	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		if (APUNIT_FLAG_MEMBER & func->flags) {
			/* Add the implicit 'self' argument */
			aptype_t *type = aptype_clone(unit->name);
			apvar_t *arg = apvar_alloc(strdup("self"), 0, type, 0);	
			arg->next = func->args;
			func->args = arg;
		}
	}
}

void apparser_module(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
	aphash_put(self->types, unit->name, unit);
}

void apparser_check_unit(apparser_t *self, apunit_t *unit) {
	self->unit = unit;
	self->symbols = unit->symbols;

	/* Build the symbol table for the compilation unit */
	for (apvar_t *var = unit->vars; var; var = var->next) {
		apsymtab_var(unit->symbols, var->name, var);
	}
	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apsymtab_func(unit->symbols, func->name, func);
	}

	/* Type check the functions */
	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apparser_check_func(self, func);
	}
}

void apparser_check_func(apparser_t *self, apfunc_t *func) {
	if (func->block) {
		func->block->symbols = apsymtab_alloc(self->symbols);
		for (apvar_t *arg = func->args; arg; arg = arg->next) {
			apsymtab_var(func->block->symbols, arg->name, arg);
		}

		self->rets = func->rets;
		apparser_check_stmt(self, func->block);
		if (func->rets && !func->block->chktype) {
			apparser_print_loc_end(self, &func->block->loc);
			apparser_error(self, "Missing return statement\n");
		}	
	}	
}

void apparser_check_stmt(apparser_t *self, apstmt_t *stmt) {
	switch (stmt->type) {
	case APSTMT_TYPE_BLOCK:
		/* Allocate a new symbol table for this block */
		if (!stmt->symbols) { 
			stmt->symbols = apsymtab_alloc(self->symbols);
		}
		self->symbols = stmt->symbols;
		for (apstmt_t *child = stmt->child[0]; child; child = child->next) {
			apparser_check_stmt(self, child);
			if (child->chktype) {
				stmt->chktype = child->chktype;	
			}
		}
		self->symbols = apsymtab_get_parent(self->symbols);
		break;

	case APSTMT_TYPE_RETURN:
		if (stmt->expr) {
			apparser_check_expr(self, stmt->expr);
			stmt->chktype = aptype_clone(stmt->expr->chktype);
		}
		if (self->rets && !stmt->chktype) {
			apparser_print_loc_end(self, &stmt->loc);
			apparser_error(self, "Missing return statement\n");
		} if (aptype_comp(self->rets, stmt->chktype)) {
			apparser_print_loc(self, &stmt->loc);	
			apparser_error(self, "Return type is invalid (got ");
			apparser_print_type(self, stmt->expr->chktype);
			apparser_error(self, ", expected ");
			apparser_print_type(self, self->rets);
			apparser_error(self, ")\n");
		}
		break;

	case APSTMT_TYPE_EXPR:
		apparser_check_expr(self, stmt->expr);
		break;

	case APSTMT_TYPE_DECL:
		apparser_check_var(self, stmt->var);
		apsymtab_var(self->symbols, stmt->var->name, stmt->var);
		if (APTYPE_TYPE_OBJECT == stmt->var->type->type && !stmt->var->expr) {
			apparser_print_loc(self, &stmt->loc);
			apparser_error(self, "Uninitialized reference\n");
		}
		break;

	case APSTMT_TYPE_UNTIL:
	case APSTMT_TYPE_WHILE:
	case APSTMT_TYPE_DOWHILE:
	case APSTMT_TYPE_DOUNTIL:
	case APSTMT_TYPE_FOR:
		for (int i = 0; i < stmt->nchild; i++) {
			apparser_check_stmt(self, stmt->child[i]);
			if (stmt->child[i]->chktype) {
				stmt->chktype = stmt->child[i]->chktype;
			}
		}
		break;

	case APSTMT_TYPE_COND:
		apparser_check_stmt(self, stmt->child[0]);
		apparser_check_stmt(self, stmt->child[1]);
		if (stmt->child[2]) {
			apparser_check_stmt(self, stmt->child[2]);
			if (!aptype_comp(stmt->child[1]->chktype, stmt->child[2]->chktype)) {
				stmt->chktype = stmt->child[1]->chktype;
			}
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

		case APEXPR_TYPE_CALL:
			apparser_check_expr_call(self, expr);
			break;

		case APEXPR_TYPE_SCALL:
			apparser_check_expr_scall(self, expr);
			break;

		case APEXPR_TYPE_CTOR:
			apparser_check_expr_ctor(self, expr);
			break;

		case APEXPR_TYPE_MCALL:
			apparser_check_expr_mcall(self, expr);	
			break;
			
		case APEXPR_TYPE_MVAR:
			apparser_check_expr_mvar(self, expr);
			break;

		case APEXPR_TYPE_INDEX:
			break;

		case APEXPR_TYPE_SVAR:
			apparser_check_expr_svar(self, expr);
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
		apparser_error(self, "Cannot apply '%s' to ", expr->string);
		apparser_print_type(self, expr->child[0]->chktype);
		apparser_error(self, "and ");
		apparser_print_type(self, expr->child[1]->chktype);
		apparser_error(self, "\n");
		return;
	}
	expr->chktype = aptype_clone(expr->child[0]->chktype);
}

void apparser_check_expr_mvar(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[0]);
	aptype_t *type = expr->child[0]->chktype;
	apunit_t *unit = (apunit_t *)aphash_get(self->types, type);
	if (!unit) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared module, class, or interface ");
		apparser_error(self, "'%s'\n", type->name);
		return;
	}

	apvar_t *var = apsymtab_get_var(unit->symbols, expr->string);

	if (!var) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared variable '%s", unit->name->name);
		apparser_error(self, ".%s'\n", expr->string);
		return;
	}
	if (APUNIT_FLAG_STATIC & var->flags) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Not an instance variable: ");
		apparser_error(self, "'%s", unit->name->name);
		apparser_error(self, ".%s\n'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(var->type);
}

void apparser_check_expr_svar(apparser_t *self, apexpr_t *expr) {
	aptype_t *type = expr->clstype;
	apunit_t *unit = (apunit_t *)aphash_get(self->types, type);
	if (!unit) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared module, class, or interface ");
		apparser_error(self, "'%s'\n", type->name);
		return;
	}

	apvar_t *var = apsymtab_get_var(unit->symbols, expr->string);
	
	if (!var) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared variable '%s", unit->name->name);
		apparser_error(self, ".%s'\n", expr->string);
		return;
	}
	if (!(APUNIT_FLAG_STATIC & var->flags)) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Not a static variable: ");
		apparser_error(self, "'%s", unit->name->name);
		apparser_error(self, ".%s'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(var->type);
}

void apparser_check_expr_var(apparser_t *self, apexpr_t *expr) {
	apvar_t *var = apsymtab_get_var(self->symbols, expr->string);	
	if (!var) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared variable '%s'\n", expr->string);
		return;
	}
	if (APUNIT_FLAG_MEMBER & var->flags) {
		if (APUNIT_TYPE_MODULE == self->unit->type) {
			expr->type = APEXPR_TYPE_SVAR;
			expr->clstype = aptype_clone(self->unit->name);
		} else {
			/* Member variable access with an implied 'self' keyword */
			expr->type = APEXPR_TYPE_MVAR;
			expr->nchild = 1;
			expr->child[0] = apexpr_var(&expr->loc, strdup("self"));
			expr->child[0]->chktype = aptype_clone(self->unit->name);
		}
	}
	expr->chktype = aptype_clone(var->type);
}

void apparser_check_expr_ctor(apparser_t *self, apexpr_t *expr) {
	apunit_t *unit = (apunit_t *)aphash_get(self->types, expr->clstype);
	if (!unit) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Unknown type '%s'\n", expr->clstype->name);
		return;
	}
	expr->chktype = aptype_clone(expr->clstype);

	for (apfunc_t *ctor = unit->ctors; ctor; ctor = ctor->next) {
		if (!apparser_check_args(self, ctor, expr->child[0])) {
			return;
		}
	}

	apparser_print_loc(self, &expr->loc);
	apparser_error(self, "Constructor '%s", expr->clstype->name);
	apparser_print_args(self, expr->child[0]);
	apparser_error(self, "' not found\n");
}

void apparser_check_expr_call(apparser_t *self, apexpr_t *expr) {
	apfunc_t *func = apsymtab_get_func(self->symbols, expr->string);
	if (!func) {
		apparser_print_loc(self, &expr->loc);	
		apparser_error(self, "Undeclared function '%s()'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(func->rets);

	if (APUNIT_FLAG_MEMBER & func->flags) {
		if (APUNIT_TYPE_MODULE == self->unit->type) {
			expr->type = APEXPR_TYPE_SCALL;
			expr->clstype = aptype_clone(self->unit->name);
		} else {
			/* Member function call with implied 'self' keyword */
			expr->type = APEXPR_TYPE_MCALL;
			apexpr_t *arg = apexpr_var(&expr->loc, strdup("self"));
			arg->next = expr->child[0];
			arg->chktype = aptype_clone(self->unit->name);
			expr->child[0] = arg;
		}
	}	

	if (apparser_check_args(self, func, expr)) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Cannot apply '%s", func->name);
		apparser_print_params(self, func->args);
		apparser_error(self, "' to '");
		apparser_print_args(self, expr->child[0]);
		apparser_error(self, "'\n");
		return;	
	}
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
		apparser_error(self, "Undeclared function '%s", unit->name->name);
		apparser_error(self, ".%s()'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(func->rets);

	if (apparser_check_args(self, func, expr)) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Cannot apply '%s", func->name);
		apparser_print_params(self, func->args);
		apparser_error(self, "' to '");
		apparser_print_args(self, expr->child[0]);
		apparser_error(self, "'\n");
		return;
	}
}

void apparser_check_expr_mcall(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[0]);
	aptype_t *type = expr->child[0]->chktype;
	if (!type) {
		return;
	}
	apunit_t *unit = (apunit_t *)aphash_get(self->types, type);
	if (!unit) {
		return;
	}

	apfunc_t *func = apsymtab_get_func(unit->symbols, expr->string);
	if (!func) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared function '%s", unit->name->name);
		apparser_error(self, ".%s()'\n", expr->string);
		return;
	}
	expr->chktype = aptype_clone(func->rets);
	
	if (apparser_check_args(self, func, expr)) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Cannot apply '%s", func->name);
		apparser_print_params(self, func->args);
		apparser_error(self, "' to '");
		apparser_print_args(self, expr->child[0]);
		apparser_error(self, "'\n");
		return;
	}
}

void apparser_check_var(apparser_t *self, apvar_t *var) {

	if (var->expr) {
		apparser_check_expr(self, var->expr);
		if (var->expr->chktype && aptype_comp(var->type, var->expr->chktype)) {
			apparser_print_loc(self, &var->expr->loc);
			apparser_error(self, "Incompatible type in assignment (got ");
			apparser_print_type(self, var->expr->chktype);
			apparser_error(self, "expected ");
			apparser_print_type(self, var->type);
			apparser_error(self, ")\n");
			return;
		}
	}
} 

int apparser_check_args(apparser_t *self, apfunc_t *func, apexpr_t *expr) {
	apvar_t *var = func->args;
	if (expr) {
		for (apexpr_t *child = expr->child[0]; child; child = child->next) {
			apparser_check_expr(self, child);	
			if (!var || aptype_comp(var->type, child->chktype)) {
				return 1;
			}
			var = var->next;
		}
	}
	if (var) {
		return 1;
	}
	return 0;
}

int apparser_resolve_type(apparser_t *self, aptype_t *type) {

	return 0;
}

void apparser_print_type(apparser_t *self, aptype_t *type) {
	if (type) {
		apparser_error(self, "'%s'", type->name);	
	} else {
		apparser_error(self, "'void'");
	}
}

void apparser_print_args(apparser_t *self, apexpr_t *expr) {
	apparser_error(self, "(");
	while (expr) {
		if (expr->next) {
			apparser_error(self, "%s,", expr->chktype->name);
		} else {
			apparser_error(self, "%s", expr->chktype->name);
		}
		expr = expr->next;
	}
	apparser_error(self, ")");
}

void apparser_print_params(apparser_t *self, apvar_t *var) {
	apparser_error(self, "(");
	while (var) {
		if (var->next) {
			apparser_error(self, "%s,", var->type->name);
		} else {
			apparser_error(self, "%s", var->type->name);
		}
		var = var->next;
	}
	apparser_error(self, ")");


}

void apparser_print_loc(apparser_t *self, aploc_t *loc) {
	int line = loc->first_line;
	int column = loc->first_column;

	fprintf(stderr, "%s:%d:%d: ", self->unit->filename, line, column);
}

void apparser_print_loc_end(apparser_t *self, aploc_t *loc) {
	int line = loc->last_line;
	int column = loc->last_column;

	fprintf(stderr, "%s:%d:%d: ", self->unit->filename, line, column);
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
		free(self->filename);
		free(self);
	}
}

void yyerror(aploc_t *loc, apparser_t *self, void* scanner, const char *msg) {
	fprintf(stderr, "%s:%d:", self->filename, loc->first_line);
	fprintf(stderr, "%d: %c%s\n", loc->first_column, toupper(msg[0]), msg + 1);
	self->error = 1;
}
