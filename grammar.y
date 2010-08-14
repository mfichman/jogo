%{
#include <parser.h>
#include <expr.h>
#include <type.h>
#include <stmt.h>
#include <var.h>
#include <func.h>
#include <unit.h>
#include <import.h>
#include <def.h>
#include <op.h>
%}

%union { expr_t *expr; }
%union { stmt_t *stmt; }
%union { type_t *type; }
%union { unit_t *unit; }
%union { var_t *var; }
%union { func_t *func; }
%union { import_t *import; }
%union { def_t *def; }
%union { char *string; }
%union { int null; }
%union { int flag; }

%pure_parser
%parse-param { parser_t *parser }
%parse-param { void *scanner }
%lex-param { yyscan_t *scanner }

%destructor { expr_free($$); $$ = 0; } <expr>
%destructor { stmt_free($$); $$ = 0; } <stmt>
%destructor { type_free($$); $$ = 0; } <type>
%destructor { unit_free($$); $$ = 0; } <unit>
%destructor { var_free($$); $$ = 0; } <var>
%destructor { func_free($$); $$ = 0; } <func>
%destructor { free($$); } <string>


/* BISON declarations */
%token <type> TOK_TYPE
%token <type> TOK_PRIMITIVE
%token <string> TOK_IDENT
%token <string> TOK_CONST
%token <expr> TOK_STRING
%token <expr> TOK_NUMBER 
%token TOK_CLASS
%token TOK_INTERFACE
%token TOK_STRUCT
%token TOK_MODULE
%token TOK_IMPORT
%token TOK_DEF
%token TOK_INIT
%token TOK_DESTROY
%token <flag> TOK_PUBLIC
%token <flag> TOK_PRIVATE
%token <flag> TOK_PROTECTED
%token <flag> TOK_STATIC
%token <flag> TOK_NATIVE
%token TOK_WHILE
%token TOK_ELSE
%token TOK_UNTIL
%token TOK_IF
%token TOK_FOREACH
%token TOK_DO
%token TOK_FOR
%token TOK_RETURN
%token TOK_EQUAL
%token TOK_NOTEQUAL
%token TOK_SCOPE
%token TOK_OR
%token TOK_AND
%token TOK_GE
%token TOK_LE
%token TOK_LSHIFT
%token TOK_RSHIFT
%token TOK_MUL_ASSIGN
%token TOK_DIV_ASSIGN
%token TOK_SUB_ASSIGN
%token TOK_ADD_ASSIGN
%token TOK_MOD_ASSIGN
%token TOK_BITOR_ASSIGN
%token TOK_BITAND_ASSIGN
%token TOK_INC
%token TOK_DEC

%type <unit> class
%type <unit> interface
%type <unit> module
%type <unit> struct
%type <var> variable
%type <import> import
%type <def> def
%type <func> constructor
%type <func> destructor
%type <func> function
%type <func> native
%type <func> prototype
%type <flag> access
%type <flag> static
%type <var> parameter_signature
%type <var> parameter_list
%type <type> type
%type <type> qualified_name 
%type <stmt> block
%type <stmt> statement_list
%type <stmt> statement
%type <stmt> clause
%type <stmt> conditional
%type <expr> argument_list
%type <expr> expression
%type <expr> assignment
%type <expr> logical_or
%type <expr> logical_and
%type <expr> bitwise_or
%type <expr> bitwise_and
%type <expr> equality
%type <expr> relation
%type <expr> shift
%type <expr> addition
%type <expr> multiplication
%type <expr> unary
%type <expr> postfix
%type <expr> primary


/* The Standard Apollo Grammar, 2010 */
%%
translation_unit
    : TOK_CLASS qualified_name ';' class {
        unit_name($4, $2); 
		parser_class(parser, $4);
    }
    | TOK_INTERFACE qualified_name ';' interface {
        unit_name($4, $2); 
		parser_interface(parser, $4);
    }
    | TOK_STRUCT qualified_name ';' struct {
        unit_name($4, $2); 
		parser_struct(parser, $4);
    }
    | TOK_MODULE qualified_name ';' module {
        unit_name($4, $2);
		parser_module(parser, $4);
    }
    ;

class
    : import class { $$ = $2; unit_import($$, $1); }
    | def class { $$ = $2; unit_def($$, $1); }
    | variable class { $$ = $2; unit_var($$, $1); }
    | constructor class { $$ = $2; unit_ctor($$, $1); }
    | destructor class { $$ = $2; unit_dtor($$, $1); }
    | function class { $$ = $2; unit_func($$, $1); }
	| native class { $$ = $2; unit_func($$, $1); }
	| error class { $$ = $2; }
    | /* empty */ { $$ = unit_alloc(UNIT_TYPE_CLASS); }
    ;

interface
    : import interface { $$ = $2; unit_import($$, $1); }
    | def interface { $$ = $2; unit_def($$, $1); }
    | prototype interface { $$ = $2; unit_func($$, $1); }
	| error interface { $$ = $2; }
    | /* empty */ { $$ = unit_alloc(UNIT_TYPE_INTERFACE); }
    ;

struct
    : import struct { $$ = $2; unit_import($$, $1); }
    | def struct { $$ = $2; unit_def($$, $1); }
    | variable struct { $$ = $2; unit_var($$, $1); }
    | constructor struct { $$ = $2; unit_ctor($$, $1); }
    | function struct { $$ = $2; unit_func($$, $1); }
	| error struct { $$ = $2; }
    | /* empty */ { $$ = unit_alloc(UNIT_TYPE_STRUCT); }
    ;

module
    : import module { $$ = $2; unit_import($$, $1); }
    | def module { $$ = $2; unit_def($$, $1); }
    | function module { $$ = $2; unit_func($$, $1); }
	| error module { $$ = $2; }
    | /* empty */ { $$ = unit_alloc(UNIT_TYPE_MODULE); }
    ;
    
import 
    : TOK_IMPORT qualified_name ';' { $$ = import_alloc($2); }
    ;

def
    : TOK_DEF type TOK_TYPE ';' { $$ = def_alloc($2, $3); }
    ;

variable
    : access static type TOK_IDENT '=' expression ';' {
		$$ = var_alloc($4, $1|$2, $3, $6);
		free($4);
    }
    | access static type TOK_IDENT ';' {
		$$ = var_alloc($4, $1|$2, $3, 0);
		free($4);
    }
	| access static type TOK_CONST '=' expression ';' {
		$$ = var_alloc($4, $1|$2|UNIT_FLAG_CONST, $3, $6);
		free($4);
	}
    ;

constructor
    : TOK_INIT parameter_signature access block { 
		$$ = func_alloc("@init", $2, 0, $4);
		$$->flags = $3;
	}
    ;

destructor
    : TOK_DESTROY '(' ')' block { 
		$$ = func_alloc("@destroy", 0, 0, $4); 
	}
    ;

function
    : TOK_IDENT parameter_signature access static type block {
		$$ = func_alloc($1, $2, $5, $6);
		$$->flags = $3|$4;
		free($1);
    }
	| TOK_IDENT parameter_signature access static block {
		$$ = func_alloc($1, $2, 0, $5);
		$$->flags = $3|$4;
		free($1);
	}
    ;

native
	: TOK_IDENT parameter_signature access static TOK_NATIVE type ';' {
		$$ = func_alloc($1, $2, $6, 0); 
		$$->flags = $3|$4|UNIT_FLAG_NATIVE;
		free($1);
	}
	| TOK_IDENT parameter_signature access static TOK_NATIVE ';' {
		$$ = func_alloc($1, $2, 0, 0);
		$$->flags = $3|$4|UNIT_FLAG_NATIVE;
		free($1);
	}
	;

prototype
    : TOK_IDENT parameter_signature type ';' {
		$$ = func_alloc($1, $2, $3, 0);
		free($1);
    }
	| TOK_IDENT parameter_signature ';' {
		$$ = func_alloc($1, $2, 0, 0);
		free($1);
	}
    ;

parameter_signature
	: '(' parameter_list ')' { $$ = $2; }
	| '(' ')' { $$ = 0; }
	;

parameter_list
	: type TOK_IDENT ',' parameter_list { 
		$$ = $4;
		$$->next = var_alloc($2, 0, $1, 0); 
		free($2);
	}
	| type TOK_IDENT { 
		$$ = var_alloc($2, 0, $1, 0);
		free($2);
	}
    ;

access 
    : TOK_PUBLIC { $$ = UNIT_FLAG_PUBLIC; }
    | TOK_PRIVATE { $$ = UNIT_FLAG_PRIVATE; }
    | TOK_PROTECTED { $$ = UNIT_FLAG_PROTECTED; }
    | /* empty */ { $$ = 0; }
    ;

static
    : TOK_STATIC { $$ = UNIT_FLAG_STATIC; }
    | /* empty */ { $$ = 0; }
    ;
    
type 
    : TOK_PRIMITIVE { $$ = $1; } 
	| TOK_PRIMITIVE '*' { $$ = $1; $$->pointer = 1; }
    | qualified_name { $$ = $1; }
	| qualified_name '*' { $$ = $1; $$->pointer = 1; }
    ;

qualified_name
    : TOK_TYPE TOK_SCOPE qualified_name { $$ = type_concat($1, $3); }
    | TOK_TYPE { $$ = $1; } 
	| TOK_CONST TOK_SCOPE qualified_name { 
		$$ = type_concat(type_object($1), $3); 
		free($1);
	}
	| TOK_CONST { 
		$$ = type_object($1); 
		free($1);
	}
    ;
    
block
    : '{' statement_list '}' { $$ = $2; }
    ;

statement_list
    : statement_list statement { $$ = stmt_append($1, $2); }
    | /* empty */ { $$ = stmt_block(); }
    ;

statement
	: TOK_FOR '(' clause ';' clause ';' clause ')' block {
		$$ = stmt_for($3, $5, $7, $9);
	}
	| TOK_FOREACH '(' type TOK_IDENT ':' expression ')' block {
		$$ = stmt_foreach(var_alloc($4, 0, $3, $6), $8);
		free($4);
	}
	| TOK_UNTIL '(' clause ')' block {
		$$ = stmt_until($3, $5);
	}
	| TOK_WHILE '(' clause ')' block {
		$$ = stmt_while($3, $5);
	}
	| TOK_DO block TOK_UNTIL '(' clause ')' ';' {
		$$ = stmt_dountil($2, $5);
	}
	| TOK_DO block TOK_WHILE '(' clause ')' ';' {
		$$ = stmt_dowhile($2, $5);
	}
    | type TOK_IDENT ';' { 
		$$ = stmt_decl(var_alloc($2, 0, $1, 0)); 
		free($2);
	}
	| conditional { $$ = $1; }
	| clause ';' { $$ = $1; }
	| error ';' { $$ = 0; }
	| TOK_RETURN expression ';' { $$ = stmt_return($2); }
	| TOK_RETURN ';' { $$ = stmt_return(0); }
	;

clause
    : type TOK_IDENT '=' expression { 
		$$ = stmt_decl(var_alloc($2, 0, $1, $4)); 
		free($2);
	}  
    | expression { 
		$$ = stmt_expr($1); 
	}
	| /* empty */ { $$ = 0; } 
	;

conditional
	: TOK_IF '(' clause ')' block {
		$$ = stmt_conditional($3, $5, 0);
	}
	| TOK_IF '(' clause ')' block TOK_ELSE conditional {
		$$ = stmt_conditional($3, $5, $7);
	}
	| block { $$ = $1; }
	;

expression : assignment { $$ = $1; }

assignment
    : unary '=' assignment { 
		$$ = expr_binary(op_assign, $1, $3); 
	}
    | unary TOK_MUL_ASSIGN assignment { 
		$$ = expr_binary(op_mul_assign, $1, $3); 
	}
    | unary TOK_DIV_ASSIGN assignment { 
		$$ = expr_binary(op_div_assign, $1, $3); 
	}
    | unary TOK_MOD_ASSIGN assignment { 
		$$ = expr_binary(op_mod_assign, $1, $3); 
	}
    | unary TOK_SUB_ASSIGN assignment { 
		$$ = expr_binary(op_sub_assign, $1, $3); 
	}
    | unary TOK_ADD_ASSIGN assignment { 
		$$ = expr_binary(op_add_assign, $1, $3); 
	}
    | unary TOK_BITAND_ASSIGN assignment { 
		$$ = expr_binary(op_bitand_assign, $1, $3); 
	}
    | unary TOK_BITOR_ASSIGN assignment { 
		$$ = expr_binary(op_bitor_assign, $1, $3); 
	}
    | logical_or { $$ = $1; }
    ;

logical_or
    : logical_or TOK_OR logical_and { $$ = expr_binary(op_or, $1, $3); }
    | logical_and { $$ = $1; }
    ;

logical_and
    : logical_and TOK_AND bitwise_or { $$ = expr_binary(op_and, $1, $3); }
    | bitwise_or { $$ = $1; }
    ;

bitwise_or
    : bitwise_or '|' bitwise_and { $$ = expr_binary(op_bitor, $1, $3); }
    | bitwise_or '^' bitwise_and { $$ = expr_binary(op_bitxor, $1, $3); }
    | bitwise_and { $$ = $1; }
    ;

bitwise_and
    : bitwise_and '&' equality { $$ = expr_binary(op_bitand, $1, $3); }
    | equality { $$ = $1; }
    ;

equality
    : equality TOK_EQUAL relation { $$ = expr_binary(op_equal, $1, $3); }
    | equality TOK_NOTEQUAL relation { 
		$$ = expr_binary(op_notequal, $1, $3); 
	}
    | relation { $$ = $1; }
    ;

relation
    : relation '>' shift { $$ = expr_binary(op_greater, $1, $3); }
    | relation '<' shift { $$ = expr_binary(op_less, $1, $3); }
    | relation TOK_GE shift { $$ = expr_binary(op_ge, $1, $3); }
    | relation TOK_LE shift { $$ = expr_binary(op_le, $1, $3); }
    | shift { $$ = $1; }
    ;

shift
    : shift TOK_LSHIFT addition { $$ = expr_binary(op_lshift, $1, $3); }
    | shift TOK_RSHIFT addition { $$ = expr_binary(op_rshift, $1, $3); }
    | addition { $$ = $1; }
    ;

addition
    : addition '+' multiplication { $$ = expr_binary(op_plus, $1, $3); }
    | addition '-' multiplication { $$ = expr_binary(op_minus, $1, $3); }
    | multiplication { $$ = $1; }
    ;

multiplication
    : multiplication '*' unary { $$ = expr_binary(op_mul, $1, $3); }
    | multiplication '/' unary { $$ = expr_binary(op_div, $1, $3); }
    | multiplication '%' unary { $$ = expr_binary(op_mod, $1, $3); }
    | unary { $$ = $1; }
    ;

unary
    : TOK_INC unary { $$ = expr_unary(op_inc, $2); }
    | TOK_DEC unary { $$ = expr_unary(op_dec, $2); }
    | '+' unary { $$ = expr_unary(op_plus, $2); }
    | '-' unary { $$ = expr_unary(op_minus, $2); }
    | '!' unary { $$ = expr_unary(op_bang, $2); }
    | '~' unary { $$ = expr_unary(op_tilde, $2); }
    | '*' unary { $$ = expr_unary(op_star, $2); }
    | postfix { $$ = $1; }
    ;

postfix
    : postfix '(' argument_list ')' { $$ = expr_call($1, $3); }
	| postfix '(' ')' { $$ = expr_call($1, 0); }
    | postfix '[' expression ']' { $$ = expr_index($1, $3); }
    | postfix '.' TOK_IDENT { $$ = expr_member($1, $3); free($3); }
    | postfix TOK_INC { $$ = expr_unary(op_postinc, $1); }
    | postfix TOK_DEC { $$ = expr_unary(op_postdec, $1); }
    | primary { $$ = $1; }
    ;

primary
    : TOK_STRING { $$ = $1; }
    | TOK_NUMBER { $$ = $1; }
    | TOK_IDENT { $$ = expr_var($1); free($1); }
	| type '.' TOK_CONST { $$ = expr_static($1, $3); free($3); }
	| type '.' TOK_IDENT { $$ = expr_static($1, $3); free($3); }
	| type '(' argument_list ')' { $$ = expr_ctor($1, $3); }
	| type '(' ')' { $$ = expr_ctor($1, 0); }
    | '(' expression ')' { $$ = $2; } 
    ;

argument_list
	: expression ',' argument_list { 
		$$ = $3;
		$$->next = $1;
	}
	| expression { $$ = $1; } 
	;
