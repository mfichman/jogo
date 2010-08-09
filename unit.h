#ifndef UNIT_H
#define UNIT_H

#include <type.h>
#include <func.h>
#include <var.h>

#define UNIT_TYPE_CLASS 1 /* Class compilation unit */
#define UNIT_TYPE_STRUCT 2 /* Structure compilation unit */
#define UNIT_TYPE_MODULE 3 /* Module compilation unit */
#define UNIT_TYPE_INTERFACE 4 /* Interface compilation unit */

/* Compilation unit struct for classes, structs, and enums */
typedef struct unit unit_t;
struct unit {
	type_t *name;       	/* Name of the compilation unit */
	int type;               /* Type of unit: struct, class, or enum */
	//import_t *imports;      /* List of imports */
	//def_t *defs;            /* List of defines */
	var_t *vars;       		/* List of variables */
	func_t *ctors;      	/* List of constructors */
	func_t *dtors;      	/* List of destructors */
	func_t *funcs;      	/* List of functions */
	unit_t *next;
};

unit_t *unit_alloc(int type);
void unit_name(unit_t *self, type_t *name);
//void unit_import(unit_t *self, import_t *import);
//void unit_def(unit_t *self, def_t *def);
void unit_var(unit_t *self, var_t *var);
void unit_ctor(unit_t *self, func_t *ctor);
void unit_dtor(unit_t *self, func_t *dtor);
void unit_func(unit_t *self, func_t *func);
void unit_free(unit_t *self);

#endif
