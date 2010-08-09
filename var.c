#include <var.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

var_t *var_alloc(const char *name, int flags, type_t *type, expr_t *expr) {
	var_t *self = malloc(sizeof(var_t));

	self->name = malloc(strlen(name) + 1);
	self->flags = flags;
	self->type = type;
	self->expr = expr;
	self->next = 0;
	
	strcpy(self->name, name);
	
	return self;
}

void var_free(var_t *self) {

	if (self) {

		free(self->name);
		type_free(self->type);
		expr_free(self->expr);
		var_free(self->next);
		free(self);

	}
}
