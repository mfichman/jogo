#ifndef UNIT_H
#define UNIT_H

#define UNIT_TYPE_CLASS 1 /* Class compilation unit */
#define UNIT_TYPE_STRUCT 2 /* Structure compilation unit */
#define UNIT_TYPE_MODULE 3 /* Module compilation unit */

/* Compilation unit struct for classes, structs, and enums */
typedef struct unit unit_t;
struct unit {
	const char *name;       /* Name of the compilation unit */
	int type;               /* Type of unit: struct, class, or enum */
	import_t *imports;      /* List of imports */
	def_t *defs;            /* List of defines */
	variable_t *vars;       /* List of variables */
	function_t *ctors;      /* List of constructors */
	function_t *dtors;      /* List of destructors */
	function_t *funcs;      /* List of functions */
	unit *next;
};

unit_t *unit_alloc(const char *name, int type);
void unit_free(unit_t *self);
void unit_import(unit_t *self, import_t *import);
void unit_def(unit_t *self, def_t *def);
void unit_var(unit_t *self, variable_t *var);
void unit_ctor(unit_t *self, function_t *ctor);
void unit_dtor(unit_t *self, function_t *dtor);
void unit_func(unit_t *self, function_t *func);

#endif
