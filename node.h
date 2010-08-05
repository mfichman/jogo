#ifndef NODE_H
#define NODE_H



#define NODE_TYPE_FUNCTION
#define NODE_TYPE_VARIABLE
#define NODE_TYPE_LITERAL
#define NODE_TYPE_CLASS
#define NODE_TYPE_STRUCT
#define NODE_TYPE_EXPRESSION
#define NODE_TYPE_STATEMENT


typedef struct literal literal_t;
struct literal {
	char *literal;
};


#define UNIT_TYPE_CLASS 1
#define UNIT_TYPE_STRUCT 2
#define UNIT_TYPE_ENUM 3

/* Compilation unit struct for classes, structs, and enums */
typedef struct unit unit_t;
struct unit {
	char *name;             /* Name of the compilation unit */
	int type;               /* Type of unit: struct, class, or enum */
	import_t *imports;      /* List of imports */
	def_t *defs;            /* List of defines */
	variable_t *vars;       /* List of variables */
	function_t *ctors;      /* List of constructors */
	function_t *dtors;      /* List of destructors */
	function_t *funcs;      /* List of functions */
	unit *next;
};

typedef struct function function_t;
struct function {
	char *name;             /* Qualified function name */
	compound_t *body;       /* Function body */
	type_t *rets;	     	/* List of return types (in reverse order) */
	type_t *args;			/* List of argument types (in reverse_order) */
	function *next;         
};

typedef struct type type_t;
struct type {
	char *name;             /* Fully qualified type name */
};

#define NODE_TYPE_STRING
#define NODE_TYPE_PAIR
#define NODE_TYPE_LIST
#define NODE_TYPE_UNARY
#define NODE_TYPE_BINARY
#define NODE_TYPE_CALL
#define NODE_TYPE_INDEX
#define NODE_TYPE_POSTFIX
#define NODE_TYPE_MEMBER

typedef struct node_t;
struct node {
	int type				/* Node type */
	char *string;			/* Possible string value of the expression */
	node_t *child;          /* Child expressions */
	node_t *next;
};

unit_t *unit_alloc(const char *name, int type);
void unit_free(unit_t *self);
void unit_import(unit_t *self, import_t *import);
void unit_def(unit_t *self, def_t *def);
void unit_var(unit_t *self, variable_t *var);
void unit_ctor(unit_t *self, function_t *ctor);
void unit_dtor(unit_t *self, function_t *dtor);
void unit_func(unit_t *self, function_t *func);

function_t *function_alloc(const char *name);
void function_free(function_t *self);
void function_ret(function_t *self, type_t *ret);
void function_arg(function_t *self, type_t *arg); 

type_t *type_alloc(const char *name);
void type_free(type_t *self);

expr_t *expr_alloc(const char *op);
void expr_free(expr_t *self);


#endif
