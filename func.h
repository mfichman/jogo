#ifndef FUNC_H
#define FUNC_H

#include <stmt.h>

#define UNIT_FLAG_PUBLIC 0x1
#define UNIT_FLAG_PRIVATE 0x2
#define UNIT_FLAG_PROTECTED 0x3
#define UNIT_FLAG_NATIVE 0x4
#define UNIT_FLAG_STATIC 0x5

/* Descriptor struct for functions and methods */
typedef struct function func_t;
struct function {
	char *name;       		/* Qualified function name */
	int flags;				/* Function flags */
	stmt_t *block;       	/* Function body */
	type_t *rets;	     	/* List of return types (in reverse order) */
	type_t *args;			/* List of argument types (in reverse_order) */
	func_t *next;         
};

func_t *func_alloc(const char *name, type_t *arg, type_t *ret, stmt_t *body);
void func_free(func_t *self);

#endif
