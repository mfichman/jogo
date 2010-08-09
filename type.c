#include <type.h>
#include <string.h>
#include <stdlib.h>


type_t *type_name(const char *name) {
	type_t *self = malloc(sizeof(type_t));

	self->name = malloc(strlen(name) + 1);
	self->type = TYPE_TYPE_NAME;
	self->next = 0;

	strcpy(self->name, name);
	
	return self;
}

type_t *type_primitive(const char *name) {
	type_t *self = malloc(sizeof(type_t));

	self->name = malloc(strlen(name) + 1);
	self->type = TYPE_TYPE_PRIMITIVE;
	self->next = 0;

	strcpy(self->name, name);
	
	return self;
}

type_t *type_concat(type_t *self, type_t *type) {

	size_t length = strlen(self->name) + strlen(type->name) + 3;
	self->name = realloc(self->name, length); 
	strcat(self->name, "::");
	strcat(self->name, type->name);
	
	type_free(type);
	
	return self;
}

int type_comp(type_t *self, type_t *other) {

	while (self && other) {
		if (strcmp(self->name, other->name)) {
			return 0;
		}
		self = self->next;
		other = other->next;
	}

	if (self && other) {
		return 1;
	} else {
		return 0;
	}
}

void type_free(type_t *self) {
	if (self) {
		free(self->name);
		type_free(self->next);
		free(self);
	}
}

