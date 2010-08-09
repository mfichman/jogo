#include <unit.h>
#include <assert.h>

unit_t *unit_alloc(int type) {
	unit_t *self = malloc(sizeof(unit_t));

	self->name = 0;
	self->type = type;
	self->vars = 0;
	self->ctors = 0;
	self->funcs = 0;
	self->next = 0;

	return self;
}

void unit_name(unit_t *self, type_t *name) {
	assert(!self->name);
	self->name = name;
}

void unit_var(unit_t *self, var_t *var) {
	var->next = self->vars;
	self->vars = var;
}

void unit_ctor(unit_t *self, func_t *ctor) {
	ctor->next = self->ctors;
	self->ctors = ctor;
}

void unit_dtor(unit_t *self, func_t *dtor) {
	dtor->next = self->dtors;
	self->dtors = dtor;
}

void unit_func(unit_t *self, func_t *func) {
	func->next = self->funcs;
	self->funcs = func;
}

void unit_free(unit_t *self) {
	if (self) {
		type_free(self->name);
		var_free(self->vars);
		func_free(self->dtors);
		func_free(self->funcs);
		unit_free(self->next);
		free(self);
	}
}
