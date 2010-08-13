#include <func.h>
#include <string.h>


func_t *func_alloc(const char *name, var_t *arg, type_t *ret, stmt_t *body) {
	func_t *self = malloc(sizeof(func_t));

	self->name = malloc(strlen(name) + 1);
	self->flags = 0;
	self->block = body;
	self->rets = ret;
	self->args = arg;
	self->next = 0;

	strcpy(self->name, name);

	return self;
}

void func_free(func_t *self) {
	if (self) {
		free(self->name);
		stmt_free(self->block);
		type_free(self->rets);
		var_free(self->args);
		func_free(self->next);
		free(self);
	}
}

