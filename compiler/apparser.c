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
	aphash_compfn_t comp = (aphash_compfn_t)&strcmp;
	aphash_hashfn_t hash = (aphash_hashfn_t)&aphash_string;

	self->units = 0;
	self->filename = 0;
	self->fd = 0;
	self->column = 1;
	self->error = 0;
	self->queue = 0;
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

		char *filename = malloc(strlen(import->type->name) + strlen(".ap") + 1);
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

		if (apparser_parse_unit(self, filename)) {
			apparser_error(self, "%s:", import->filename);
			apparser_error(self, "%d:", import->loc.first_line);
			apparser_error(self, "%d: ", import->loc.first_column);
			apparser_error(self, "Could not find '%s'\n", self->filename);	
		}
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
		return 1;
	}
	self->column = 1;
	self->symbols = 0;
	yyparse(self, self->scanner);
	apparser_reset_line(self);
	fclose(self->fd);

	apunit_t *unit = self->units;
	for (apimport_t *import = unit->imports; import; import = import->next) {
		if (!aphash_get(self->types, import->type->name)) {
			apimport_t *next = apimport_clone(import);
			next->next = self->queue;
			self->queue = next;
		}
	}

	return 0;
}

int apparser_read(apparser_t *self, char *buffer, int length) {
	return fread(buffer, sizeof(char), length, self->fd);
}

void apparser_class(apparser_t *self, apunit_t *unit) {
	unit->next = self->units;
	self->units = unit;
	aphash_put(self->types, unit->name, unit);

	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		if (APUNIT_FLAG_STATIC & ~func->flags) {
			/* Add the implicit 'self' argument */
			aptype_t *type = aptype_object(strdup(unit->name));
			apvar_t *arg = apvar_alloc(strdup("self"), 0, type, 0);	
			arg->next = func->args;
			func->args = arg;
		}
	}
	for (apfunc_t *ctor = unit->ctors; ctor; ctor = ctor->next) {
		ctor->rets = aptype_object(strdup(unit->name));
		apsymtab_put(unit->symbols, unit->name, ctor);
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
		if (APUNIT_FLAG_STATIC & ~func->flags) {
			/* Add the implicit 'self' argument */
			aptype_t *type = aptype_object(strdup(unit->name));
			apvar_t *arg = apvar_alloc(strdup("self"), 0, type, 0);	
			arg->next = func->args;
			func->args = arg;
		}
	}
	for (apfunc_t *ctor = unit->ctors; ctor; ctor = ctor->next) {
		ctor->rets = aptype_object(strdup(unit->name));
		apsymtab_put(unit->symbols, unit->name, ctor);
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
		apsymtab_put(unit->symbols, var->name, var);
	}
	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apsymtab_put(unit->symbols, func->name, func);
	}
	for (apimport_t *import = unit->imports; import; import = import->next) {
		apunit_t *other = aphash_get(self->types, import->type->name);
		apsymtab_put(unit->symbols, import->type->name, other);
		if (APUNIT_TYPE_MODULE == other->type) {
			for (apfunc_t *func = other->funcs; func; func = func->next) {
				apsymtab_put(unit->symbols, func->name, func);
			}
		}	
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
			apsymtab_put(func->block->symbols, arg->name, arg);
		}

		self->rets = func->rets;
		apparser_check_stmt(self, func->block);

		if (func->rets && !func->block->chktype && !self->error) { 
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
				stmt->chktype = aptype_clone(child->chktype);	
			}
		}
		self->symbols = apsymtab_get_parent(self->symbols);
		break;

	case APSTMT_TYPE_RETURN:
		if (stmt->expr) {
			apparser_check_expr(self, stmt->expr);
			stmt->chktype = aptype_clone(stmt->expr->chktype);
		}
		if (self->rets && !stmt->chktype && !self->error) {
			apparser_print_loc_end(self, &stmt->loc);
			apparser_error(self, "Missing return statement\n");
		} else if (aptype_comp(self->rets, stmt->chktype) && !self->error) {
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
		apsymtab_put(self->symbols, stmt->var->name, stmt->var);
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
				stmt->chktype = aptype_clone(stmt->child[i]->chktype);
			}
		}
		break;

	case APSTMT_TYPE_COND:
		apparser_check_stmt(self, stmt->child[0]);
		apparser_check_stmt(self, stmt->child[1]);
		if (stmt->child[2]) {
			apparser_check_stmt(self, stmt->child[2]);
			if (!aptype_comp(stmt->child[1]->chktype, stmt->child[2]->chktype)) {
				stmt->chktype = aptype_clone(stmt->child[1]->chktype);
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
	
		case APEXPR_TYPE_CALL:
			apparser_check_expr_call(self, expr);
			break;
		
		case APEXPR_TYPE_IDENT:
			apparser_check_expr_ident(self, expr);
			break;

		case APEXPR_TYPE_INDEX:
			break;

		case APEXPR_TYPE_MEMBER:
			apparser_check_expr_member(self, expr);
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

	if ('=' == *expr->string) {
		int ltype = expr->child[0]->type;
		if (APEXPR_TYPE_IDENT != ltype && APEXPR_TYPE_MEMBER != ltype) {
			apparser_print_loc(self, &expr->loc);
			apparser_error(self, "Expression is not assignable\n");	
			return;
		}
	}

	expr->chktype = aptype_clone(expr->child[0]->chktype);
}

void apparser_check_expr_ident(apparser_t *self, apexpr_t *expr) {
	apsymbol_t *sym = apsymtab_get(self->symbols, expr->string);
	if (!sym) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared identifier '%s'\n", expr->string);
		return;
	}
	if (APSYMBOL_TYPE_FUNC == *sym) {
		apfunc_t *func = (apfunc_t *)sym;
		expr->chktype = aptype_func(func);

	} else if (APSYMBOL_TYPE_VAR == *sym) {
		apvar_t *var = (apvar_t *)sym;
		expr->chktype = aptype_clone(var->type);

	} else if (APSYMBOL_TYPE_UNIT == *sym) {
		apunit_t *unit = (apunit_t *)sym;
		expr->chktype = aptype_object(strdup(unit->name));

	}	
}

void apparser_check_expr_member(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[0]);
	aptype_t *type = expr->child[0]->chktype;
	if (!type) {
		return;
	}

	apunit_t *unit = aphash_get(self->types, type->name);
	if (!unit) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Unknown type '%s'\n", type->name);
		return;
	}

	apsymbol_t *sym = apsymtab_get(unit->symbols, expr->string);
	if (!sym) {
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Undeclared identifier '%s'\n", expr->string);
		return;
	}

	if (APSYMBOL_TYPE_FUNC == *sym) {
		apfunc_t *func = (apfunc_t *)sym;
		expr->chktype = aptype_func(func);

	} else if (APSYMBOL_TYPE_VAR == *sym) {
		apvar_t *var = (apvar_t *)sym;
		expr->chktype = aptype_clone(var->type);

	} else if (APSYMBOL_TYPE_UNIT == *sym) {
		assert("Invalid symbol" && 0);

	}	
}

void apparser_check_expr_call(apparser_t *self, apexpr_t *expr) {
	apparser_check_expr(self, expr->child[0]);
	aptype_t *type = expr->child[0]->chktype;
	if (!type) {
		return; 
	}

	if (APTYPE_TYPE_FUNC != type->type) {
		apparser_print_loc(self, &expr->loc);		
		apparser_error(self, "Not a function type\n");
		return;
	}

	apunit_t *unit = type->func->unit;
	if (APUNIT_FLAG_STATIC & ~type->func->flags) {
		if (APEXPR_TYPE_MEMBER == expr->child[0]->type) {
			/* object.func() -> func(object, func) */
			apexpr_t *self = expr->child[0]->child[0]; 
			self->next = expr->child[1];
			expr->child[1] = self;
			expr->child[0]->child[0] = 0;
		} else if (self->unit == unit) {
			/* Implicit function call */		
			apexpr_t *self = apexpr_ident(&expr->loc, strdup("self"));
			self->chktype = aptype_object(strdup(unit->name));
			self->next = expr->child[1];
			expr->child[1] = self;
		}
	}

	apvar_t *var = type->func->args;
	for (apexpr_t *child = expr->child[1]; child; child = child->next) {
		if (!child->chktype) {
			apparser_check_expr(self, child);	
			if (!child->chktype) {
				return;
			}
		}
		if (!var || aptype_comp(var->type, child->chktype)) {
			apparser_print_loc(self, &expr->loc);
			apparser_error(self, "Cannot apply '%s.", type->func->unit->name);
			apparser_error(self, "%s", type->func->name);
			apparser_print_params(self, type->func->args);
			apparser_error(self, "' to '");
			apparser_print_args(self, expr->child[1]);
			apparser_error(self, "'\n");
			return;
		}
		var = var->next;
	}
	if (var) {
		printf("here");
		apparser_print_loc(self, &expr->loc);
		apparser_error(self, "Cannot apply '%s.", type->func->unit->name);
		apparser_error(self, "%s", type->func->name);
		apparser_print_params(self, type->func->args);
		apparser_error(self, "' to '");
		apparser_print_args(self, expr->child[1]);
		apparser_error(self, "'\n");
		return;
	}
	expr->chktype = aptype_clone(type->func->rets);
}

void apparser_check_var(apparser_t *self, apvar_t *var) {
 	if (var->expr) {
		apparser_check_expr(self, var->expr);
		if (var->type && aptype_comp(var->type, var->expr->chktype)) {
			apparser_print_loc(self, &var->expr->loc);
			apparser_error(self, "Incompatible type in assignment (got ");
			apparser_print_type(self, var->expr->chktype);
			apparser_error(self, " expected ");
			apparser_print_type(self, var->type);
			apparser_error(self, ")\n");
		}
	}
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
