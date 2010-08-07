%{
#include <parser.h>
#include <expr.h>
%}

%pure_parser
%parse-param { parser_t *parser }
%parse-param { void *scanner }
%lex-param { scanner *scanner }

%destructor { expr_free($$); } <expr_t *>
%destructor { statement_free($$); } <statement_t *>
%destructor { type_free($$); } <type_t *>
%destructor { free($$); } <const char *>
%union { expr_t *expr; }
%union { statement_t *statement; }
%union { type_t *type; }
%union { const char *string; }


/* BISON declarations */
%token <type_t *> TOK_TYPE
%token <type_t *> TOK_PRIMITIVE
%token <const char *> TOK_IDENT
%token <const char *> TOK_STRING
%token <const char *> TOK_NUMBER 
%token TOK_CLASS
%token TOK_INTERFACE
%token TOK_STRUCT
%token TOK_MODULE
%token TOK_IMPORT
%token TOK_DEF
%token TOK_INIT
%token TOK_DESTROY
%token TOK_PUBLIC
%token TOK_PRIVATE
%token TOK_PROTECTED
%token TOK_STATIC
%token TOK_NATIVE
%token TOK_WHILE
%token TOK_ELSE
%token TOK_UNTIL
%token TOK_IF
%token TOK_FOREACH
%token TOK_DO
%token TOK_FOR
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

%type <void> translation_unit
%type <void> import
%type <void> def
%type <void> variable
%type <void> constructor
%type <void> destructor
%type <void> function
%type <void> prototype
%type <void> argument_list
%type <void> access
%type <void> storage
%type <void> native
%type <type_t *> type
%type <type_t *> qualified_name 
%type <statement_t *> block
%type <statement_t *> statement_list
%type <statement_t *> statement
%type <statement_t *> conditional
%type <expr_t *> expression
%type <expr_t *> assignment
%type <expr_t *> logical_or
%type <expr_t *> logical_and
%type <expr_t *> bitwise_or
%type <expr_t *> bitwise_and
%type <expr_t *> equality
%type <expr_t *> relation
%type <expr_t *> shift
%type <expr_t *> addition
%type <expr_t *> multiplication
%type <expr_t *> unary
%type <expr_t *> postfix
%type <expr_t *> primary


/* Standard M++ grammar */
%%
translation_unit
    : TOK_CLASS qualified_name ';' class_member_list {
		$$ = 0;
        parser_class(parser, $2);
    }
    | TOK_INTERFACE qualified_name ';' interface_member_list {
		$$ = 0;
        parser_interface(parser, $2);
    }
    | TOK_STRUCT qualified_name ';' struct_member_list {
		$$ = 0;
        parser_struct(parser, $2);
    }
    | TOK_MODULE qualified_name ';' module_member_list {
		$$ = 0;
        parser_module(parser, $2);
    }
    ;

class_member_list
    : import class_member_list
    | def class_member_list    
    | variable class_member_list
    | constructor class_member_list
    | destructor class_member_list
    | function class_member_list
    | /* empty */
    ;

interface_member_list
    : import interface_member_list 
    | def interface_member_list
    | prototype interface_member_list
    | /* empty */
    ;

struct_member_list
    : import struct_member_list
    | def struct_member_list
    | variable struct_member_list
    | constructor struct_member_list
    | function struct_member_list
    | /* empty */
    ;

module_member_list
    : import module_member_list
    | def module_member_list
    | function module_member_list
    | /* empty */
    ;
    
import 
    : TOK_IMPORT qualified_name ';' {
        parser_import(parser, $2);
		$$ = 0;
    }
    ;

def
    : TOK_DEF type TOK_TYPE ';' {
        parser_def(parser, $2, $3);
		$$ = 0;
    }
    ;

variable
    : access storage type TOK_IDENT '=' expression ';' {
        parser_varinit(parser, $1, $2, $3, $4, $6);
		$$ = 0;
    }
    | access storage type TOK_IDENT ';' {
        parser_vardecl(parser, $1, $2, $3, $4);
		$$ = 0;
    }
    ;

constructor
    : TOK_INIT argument_list access block {
        parser_ctor(parser, $2, $3, $4);
		$$ = 0;
    }
    ;

destructor
    : TOK_DESTROY argument_list block {
        parser_dtor(parser, $2, $3);
		$$ = 0;
    }
    ;

function
    : TOK_IDENT argument_list access storage native type block {
        parser_func(parser, $1, $2, $3, $4, $5, $6, $7);
		$$ = 0;
    }
    ;

prototype
    : TOK_IDENT argument_list type ';' {
        parser_proto(parser, $1, $2, $3);
		$$ = 0;
    }
    ;

argument_list
    : '(' ')' { $$ = 0; }
    ;

access 
    : TOK_PUBLIC { $$ = expr_string("public"); }
    | TOK_PRIVATE { $$ = expr_string("private"); }
    | TOK_PROTECTED { $$ = expr_string("protected"); }
    | /* empty */ { $$ = 0; }
    ;

storage
    : TOK_STATIC { $$ = expr_string("static"); }
    | /* empty */ { $$ = 0; }
    ;
    
native
    : TOK_NATIVE { $$ = expr_string("native"); }
    | /* empty */ { $$ = 0; }
    ;

type 
    : TOK_PRIMITIVE { $$ = $1; } 
    | qualified_name { $$ = $1; }
    ;

qualified_name
    : TOK_TYPE TOK_SCOPE qualified_name { $$ = type_concat($1, $3); }
    | TOK_TYPE { $$ = $1; } 
    ;
    
block
    : '{' statement_list '}' { $$ = $2; }
    ;

statement_list
    : statement statement_list { $$ = statement_append($2, $1); }
    | /* empty */ { $$ = statement_list(); }
    ;

statement
	: TOK_FOR '(' expression ';' expression ';' expression ')' block {
		$$ = statement_for($3, $5, $7, $9);
	}
	| TOK_FOREACH '(' type TOK_IDENT ':' expression ')' block {
		$$ = statement_foreach($3, $4, $6, $8);
	}
	| TOK_UNTIL '(' expression ')' block {
		$$ = statement_until($3, $5);
	}
	| TOK_WHILE '(' expression ')' block {
		$$ = statement_while($3, $5);
	}
	| TOK_DO block TOK_UNTIL '(' expression ')' ';' {
		$$ = statement_dountil($5, $2);
	}
	| TOK_DO block TOK_WHILE '(' expression ')' ';' {
		$$ = statement_dowhile($5, $2);
	}
    | type TOK_IDENT '=' expression ';' { 
		$$ = statement_decl($1, $2, $4); 
	}  
    | type TOK_IDENT ';' { $$ = expr_decl($1, $2); }
	| conditional { $$ = $1; }
    | expression ';' { $$ = $1; }
	;

conditional
	: TOK_IF '(' expression ')' block {
		$$ = statement_conditional($3, $5, 0);
	}
	| TOK_IF '(' expression ')' block TOK_ELSE conditional {
		$$ = statement_conditional($3, $5, $7);
	}
	| block { $$ = $1; }
	;

expression : assignment { $$ = $1; }

assignment
    : unary '=' assignment { $$ = expr_binary(op_assign, $1, $3); }
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
    : postfix argument_list { $$ = expr_call($1, $2); }
    | postfix '[' expression ']' { $$ = expr_index($1, $3); }
    | postfix '.' TOK_IDENT { $$ = expr_member($1, $3); }
    | postfix TOK_INC { $$ = expr_unary(op_postinc, $1); }
    | postfix TOK_DEC { $$ = expr_unary(op_preinc, $1); }
    | primary { $$ = $1; }
    ;

primary
    : TOK_STRING { $$ = $1; }
    | TOK_NUMBER { $$ = $1; }
    | TOK_IDENT { $$ = $1; }
    | '(' expression ')' { $$ = $2; } 
    ;
