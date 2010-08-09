#ifndef TYPE_H
#define TYPE_H

#define TYPE_TYPE_PRIMITIVE 1
#define TYPE_TYPE_NAME 2

/* A fully-qualified a type name */ 
typedef struct type type_t;
struct type {
	char *name;             /* Fully qualified type name */
	int type;				/* Primitive or not? */
	type_t *next;
};

type_t *type_name(const char *name);
type_t *type_primitive(const char *name);
type_t *type_concat(type_t *self, type_t *type);
int type_comp(type_t *self, type_t *type);
void type_free(type_t *self);

#endif
