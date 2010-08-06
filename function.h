#ifndef FUNCTION_H
#define FUNCTION_H

/* Descriptor struct for functions and methods */
typedef struct function function_t;
struct function {
	const char *name;       /* Qualified function name */
	compound_t *body;       /* Function body */
	type_t *rets;	     	/* List of return types (in reverse order) */
	type_t *args;			/* List of argument types (in reverse_order) */
	function *next;         
};

function_t *function_alloc(const char *name);
void function_free(function_t *self);
void function_ret(function_t *self, type_t *ret);
void function_arg(function_t *self, type_t *arg); 

#endif
