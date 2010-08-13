#ifndef VAR_H
#define VAR_H

#include <type.h>
#include <expr.h>

/* Local variable/symbol in the current scope */
typedef struct var var_t;
struct var {
	char *name;
	int flags;
	type_t *type;
	expr_t *expr;
	var_t *next;
};	

var_t *var_alloc(const char *name, int flags, type_t *type, expr_t *expr);
var_t *var_concat(var_t *var);
void var_free(var_t *self);



#endif
