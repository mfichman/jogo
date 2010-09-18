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

#include <apchecker.h>
#include <apfunc.h>
#include <apunit.h>
#include <aphash.h>
#include <apstmt.h>
#include <aptype.h>
#include <apimport.h>
#include <apvar.h>
#include <apexpr.h>
#include <apsymtab.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

apchecker_t *apchecker_alloc() {
	apchecker_t *self = malloc(sizeof(apchecker_t));
	
	self->unit = 0;
	self->types = 0;
	self->symbols = 0;
	self->error = 0;

	return self;
}

int apchecker_check(apchecker_t *self, apunit_t *unit, aphash_t *types) {
	self->unit = unit;
	self->types = types;
	self->error = 0;

	apchecker_unit(self, unit);

	return self->error;
} 

void apchecker_unit(apchecker_t *self, apunit_t *unit) {
	self->unit = unit;
	self->symbols = unit->symbols;

	/* Build the variable/function symbol table for the compilation unit */
	for (apvar_t *var = unit->vars; var; var = var->next) {
		apsymtab_put(unit->symbols, var->name, var);
	}
	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apsymtab_put(unit->symbols, func->name, func);
	}

	/* Add imported non-primitive types to the symbol table */
	for (apimport_t *import = unit->imports; import; import = import->next) {
		apunit_t *other = aphash_get(self->types, import->name);
        apsymtab_put(unit->symbols, import->name, other);
		if (APUNIT_TYPE_MODULE == other->type) {
			for (apfunc_t *func = other->funcs; func; func = func->next) {
            	apsymtab_put(unit->symbols, func->name, func);
		    }
        }
	}

	/* Type check the functions */
	for (apfunc_t *func = unit->funcs; func; func = func->next) {
		apchecker_func(self, func);
	}
}

void apchecker_func(apchecker_t *self, apfunc_t *func) {
	if (func->block) {

		/* Create a new symbol table for this function */
		func->symbols = apsymtab_alloc(self->symbols);
		self->symbols = func->symbols;
		for (apvar_t *arg = func->args; arg; arg = arg->next) {
			apsymtab_put(func->symbols, arg->name, arg);
		}

		/* Save expected return values */
		self->rets = func->rets;

		/* Check the block statement */
		apchecker_stmt(self, func->block);

		/* Check for missing return statements */
		if (func->rets && !func->block->chktype && !self->error) { 
			self->error++;
			aploc_print_end(&func->block->loc, self->unit->filename, stderr);
			fprintf(stderr, "Missing return statement\n");
		}
	}	
}

void apchecker_stmt(apchecker_t *self, apstmt_t *stmt) {
	switch (stmt->type) {
	case APSTMT_TYPE_BLOCK:
		apchecker_stmt_block(self, stmt);
		break;

	case APSTMT_TYPE_RETURN:
		apchecker_stmt_return(self, stmt);
		break;

	case APSTMT_TYPE_EXPR:
		apchecker_expr(self, stmt->expr);
		break;

	case APSTMT_TYPE_DECL:
		apchecker_stmt_decl(self, stmt);
		break;

	case APSTMT_TYPE_UNTIL:
	case APSTMT_TYPE_WHILE:
	case APSTMT_TYPE_DOWHILE:
	case APSTMT_TYPE_DOUNTIL:
	case APSTMT_TYPE_FOR:
		for (int i = 0; i < stmt->nchild; i++) {
			apchecker_stmt(self, stmt->child[i]);
			if (stmt->child[i]->chktype) {
				stmt->chktype = aptype_clone(stmt->child[i]->chktype);
			}
		}
		break;

	case APSTMT_TYPE_COND:
		apchecker_stmt(self, stmt->child[0]);
		apchecker_stmt(self, stmt->child[1]);
		if (stmt->child[2]) {
			apchecker_stmt(self, stmt->child[2]);
			if (!aptype_comp(stmt->child[1]->chktype, stmt->child[2]->chktype)) {
				stmt->chktype = aptype_clone(stmt->child[1]->chktype);
			}
		}
		break;

	case APSTMT_TYPE_FOREACH:
		apchecker_var(self, stmt->var);
		apchecker_stmt(self, stmt->child[0]);
		break;
	}
}

void apchecker_stmt_return(apchecker_t *self, apstmt_t *stmt) {
	/* Check the return expression (return x) */
	if (stmt->expr) {
		apchecker_expr(self, stmt->expr);
		stmt->chktype = aptype_clone(stmt->expr->chktype);
	}

	/* Make sure the expression type matches the function return type */
	if (self->rets && !stmt->chktype && !self->error) {
		self->error++;
		aploc_print_end(&stmt->loc, self->unit->filename, stderr);
		fprintf(stderr, "Missing return statement\n");
	} else if (aptype_comp(self->rets, stmt->chktype) && !self->error) {
		self->error++;
		aploc_print(&stmt->loc, self->unit->filename, stderr);	
		fprintf(stderr, "Return type is invalid (got ");
		aptype_print(stmt->expr->chktype, stderr);
		fprintf(stderr, ", expected ");
		aptype_print(self->rets, stderr);
		fprintf(stderr, ")\n");
	}
}

void apchecker_stmt_block(apchecker_t *self, apstmt_t *stmt) {
	/* Allocate a new symbol table for this block */
	stmt->symbols = apsymtab_alloc(self->symbols);
	self->symbols = stmt->symbols;

	/* Check all child statments in the block */
	for (apstmt_t *child = stmt->child[0]; child; child = child->next) {
		apchecker_stmt(self, child);
		if (child->chktype) {
			stmt->chktype = aptype_clone(child->chktype);	
		}
	}

	/* Restore previous symbol table */
	self->symbols = apsymtab_get_parent(self->symbols);
}

void apchecker_stmt_decl(apchecker_t *self, apstmt_t *stmt) {
	/* Check the variable initializer expression (i.e., int a = 10;) */
	apchecker_var(self, stmt->var);

	/* Add the variable to the symbol table */
	apsymtab_put(self->symbols, stmt->var->name, stmt->var);
}

void apchecker_expr(apchecker_t *self, apexpr_t *expr) {
	switch (expr->type) {
		case APEXPR_TYPE_STRING:
			break;
	
		case APEXPR_TYPE_UNARY:
			apchecker_expr(self, expr->child[0]);
			expr->chktype = aptype_clone(expr->child[0]->chktype);
			break;

		case APEXPR_TYPE_BINARY:
			apchecker_expr_binary(self, expr);
			break;	
	
		case APEXPR_TYPE_CALL:
			apchecker_expr_call(self, expr);
			break;
		
		case APEXPR_TYPE_IDENT:
			apchecker_expr_ident(self, expr);
			break;

		case APEXPR_TYPE_INDEX:
			break;

		case APEXPR_TYPE_MEMBER:
			apchecker_expr_member(self, expr);
			break;
			
	}
}

void apchecker_expr_binary(apchecker_t *self, apexpr_t *expr) {
	apchecker_expr(self, expr->child[0]);
	apchecker_expr(self, expr->child[1]);
	if (!expr->child[0]->chktype || !expr->child[1]->chktype) {
		return;
	}

	if (aptype_comp(expr->child[0]->chktype, expr->child[1]->chktype)) {
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr); 
		fprintf(stderr, "Cannot apply '%s' to ", expr->string);
		aptype_print(expr->child[0]->chktype, stderr);
		fprintf(stderr, "and ");
		aptype_print(expr->child[1]->chktype, stderr);
		fprintf(stderr, "\n");
		return;
	}

	if ('=' == *expr->string) {
		int ltype = expr->child[0]->type;
		if (APEXPR_TYPE_IDENT != ltype && APEXPR_TYPE_MEMBER != ltype) {
			self->error++;
			aploc_print(&expr->loc, self->unit->filename, stderr);
			fprintf(stderr, "Expression is not assignable\n");	
			return;
		}
	}

	expr->chktype = aptype_clone(expr->child[0]->chktype);
}

void apchecker_expr_ident(apchecker_t *self, apexpr_t *expr) {
	apsymbol_t *sym = apsymtab_get(self->symbols, expr->string);
	if (!sym) {
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr);
		fprintf(stderr, "Undeclared identifier '%s'\n", expr->string);
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

void apchecker_expr_member(apchecker_t *self, apexpr_t *expr) {
	apchecker_expr(self, expr->child[0]);
	aptype_t *type = expr->child[0]->chktype;
	if (!type) {
		return;
	}

	apunit_t *unit = aphash_get(self->types, type->name);
	if (!unit) {
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr);
		fprintf(stderr, "Unknown type '%s'\n", type->name);
		return;
	}

	apsymbol_t *sym = apsymtab_get(unit->symbols, expr->string);
	if (!sym) {
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr);
		fprintf(stderr, "Undeclared identifier '%s'\n", expr->string);
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

void apchecker_expr_call(apchecker_t *self, apexpr_t *expr) {
	apchecker_expr(self, expr->child[0]);
	if (!expr->child[0]->chktype->type) {
		return; 
	}

	switch (expr->child[0]->chktype->type) {
	case APTYPE_TYPE_FUNC:
		apchecker_expr_call_func(self, expr);
		break;
	case APTYPE_TYPE_OBJECT:
		apchecker_expr_call_ctor(self, expr);
		break;
	default:
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr);		
		fprintf(stderr, "Not a function type\n");
		return;
	}

}

void apchecker_expr_call_func(apchecker_t *self, apexpr_t *expr) {
	aptype_t *type = expr->child[0]->chktype;
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

	if (apchecker_args(self, type->func, expr->child[1])) {
		self->error++;
		aploc_print(&expr->loc, self->unit->filename, stderr);
		fprintf(stderr, "Cannot apply '%s.", type->func->unit->name);
		fprintf(stderr, "%s", type->func->name);
		apchecker_print_params(self, type->func->args);
		fprintf(stderr, "' to '");
		apchecker_print_args(self, expr->child[1]);
		fprintf(stderr, "'\n");
		return;
	}

	expr->chktype = aptype_clone(type->func->rets);
}

void apchecker_expr_call_ctor(apchecker_t *self, apexpr_t *expr) {

}

int apchecker_args(apchecker_t *self, apfunc_t *func, apexpr_t *expr) {
	apvar_t *var = func->args;
	for (apexpr_t *child = expr; child; child = child->next) {
		if (!child->chktype) {
			apchecker_expr(self, child);	
			if (!child->chktype) {
				return 0;
			}
		}
		if (!var || aptype_comp(var->type, child->chktype)) {
			return 1;
		}
		var = var->next;
	}
	return var ? 1 : 0;
}

void apchecker_var(apchecker_t *self, apvar_t *var) {

	/* All non-nullable references should be initialized */
	if (APTYPE_FLAG_NULLABLE & ~var->type->flags && !var->expr) {
		self->error++;	
		aploc_print(&var->loc, self->unit->filename, stderr);
		fprintf(stderr, "Uninitialized reference\n");
	}
 	if (!var->expr) {
		return;
	}

	apchecker_expr(self, var->expr);
	if (self->error) {
		return;
	}

	if (var->type && aptype_comp(var->type, var->expr->chktype)) {
		self->error++;
		aploc_print(&var->expr->loc, self->unit->filename, stderr);
		fprintf(stderr, "Incompatible type in assignment (got ");
		aptype_print(var->expr->chktype, stderr);
		fprintf(stderr, " expected ");
		aptype_print(var->type, stderr);
		fprintf(stderr, ")\n");
	}
}

void apchecker_print_args(apchecker_t *self, apexpr_t *expr) {
	fprintf(stderr, "(");
	while (expr) {
		if (expr->next) {
			fprintf(stderr, "%s,", expr->chktype->name);
		} else {
			fprintf(stderr, "%s", expr->chktype->name);
		}
		expr = expr->next;
	}
	fprintf(stderr, ")");
}

void apchecker_print_params(apchecker_t *self, apvar_t *var) {
	fprintf(stderr, "(");
	while (var) {
		if (var->next) {
			fprintf(stderr, "%s,", var->type->name);
		} else {
			fprintf(stderr, "%s", var->type->name);
		}
		var = var->next;
	}
	fprintf(stderr, ")");


}

void apchecker_free(apchecker_t *self) {
	free(self);
}
