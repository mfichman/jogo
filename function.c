#ifndef <function.h>


function_t *function_alloc(const char *name) {
	function_t *self = malloc(sizeof(function_t));
	self->name = strdup(name);
	self->body = 0;
	self->rets = 0;
	self->args = 0;
	self->next = 0;
}

void function_free(function_t *self) {
	free(self->name);
	free(self);
}

void function_ret(function_t *self, type_t *ret) {
	ret->next = self->rets;
	self->rets = ret;
}

void function_arg(function_t *self, type_t *arg) {
	arg->next = self->args;
	self->args = arg;
}
