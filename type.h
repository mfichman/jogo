#ifndef TYPE_H
#define TYPE_H

/* A fully-qualified a type name */ 
typedef struct type type_t;
struct type {
	char *name;             /* Fully qualified type name */
};

type_t *type_alloc(const char *name);
void type_free(type_t *self);
int type_comp(type_t *self, type_t *other);

#endif
