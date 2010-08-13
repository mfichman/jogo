#ifndef FUNC_H
#define FUNC_H

#include <stmt.h>
#include <var.h>
#include <type.h>

/* Descriptor struct for functions and methods */
typedef struct function func_t;
struct function {
	char *name;       		/* Qualified function name */
	int flags;				/* Function flags */
	stmt_t *block;       	/* Function body */
	type_t *rets;	     	/* List of return types (in reverse order) */
	var_t *args;			/* List of argument types (in reverse_order) */
	func_t *next;         
};

func_t *func_alloc(const char *name, var_t *arg, type_t *ret, stmt_t *body);
void func_free(func_t *self);

#endif
