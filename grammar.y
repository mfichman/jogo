%{
#include "parser.hpp"
%}

%pure_parser
%parse-param { parser_t *parser }
%parse-param { void *scanner }
%lex-param { scanner *scanner }

%expect 2

/* BISON declarations */
%token TOK_CLASS
%token TOK_INTERFACE
%token TOK_STRUCT
%token TOK_MODULE
%token TOK_IMPORT
%token TOK_DEF
%token TOK_IDENT
%token TOK_INIT
%token TOK_DESTROY
%token TOK_PUBLIC
%token TOK_PRIVATE
%token TOK_PROTECTED
%token TOK_STATIC
%token TOK_NATIVE
%token TOK_INT
%token TOK_UINT
%token TOK_SHORT
%token TOK_USHORT
%token TOK_BYTE
%token TOK_UBYTE
%token TOK_LONG
%token TOK_ULONG
%token TOK_TYPE
%token TOK_EQUAL
%token TOK_NOTEQUAL
%token TOK_SCOPE
%token TOK_OR
%token TOK_AND
%token TOK_GE
%token TOK_LE
%token TOK_LSHIFT
%token TOK_RSHIFT
%token TOK_STRING
%token TOK_NUMBER 
%token TOK_MUL_ASSIGN
%token TOK_DIV_ASSIGN
%token TOK_SUB_ASSIGN
%token TOK_ADD_ASSIGN
%token TOK_MOD_ASSIGN
%token TOK_BITOR_ASSIGN
%token TOK_BITAND_ASSIGN
%token TOK_INC
%token TOK_DEC


/* Grammar */
%%
translation_unit
    : TOK_CLASS qualified_name ';' class_member_list {
        parser_class(parser);
    }
    | TOK_INTERFACE qualified_name ';' interface_member_list {
        parser_interface(parser);
    }
    | TOK_STRUCT qualified_name ';' struct_member_list {
        parser_struct(parser);
    }
    | TOK_MODULE qualified_name ';' module_member_list {
        parser_module(parser);
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
    }
    ;

def
    : TOK_DEF type TOK_TYPE ';' {
        parser_def(parser, $2, $3);
    }
    ;

variable
    : access storage type TOK_IDENT '=' assignment ';' {
        parser_varinit(parser, $1, $2, $3);
    }
    | access storage type TOK_IDENT ';' {
        parser_vardecl(parser, $1, $2, $3);
    }
    ;

constructor
    : TOK_INIT argument_list access compound_statement {
        parser_ctor(parser, $1, $2, $3);
    }
    ;

destructor
    : TOK_DESTROY argument_list compound_statement {
        parser_dtor(parser, $1, $2);
    }
    ;

function
    : TOK_IDENT argument_list access storage native type compound_statement {
        parser_func(parser, $1, $2, $3, $4, $5, $6, $7);
    }
    ;

prototype
    : TOK_IDENT argument_list type ';' {
        parser_proto(parser, $1, $2, $3);
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
    : TOK_UINT { $$ = expr_string("uint"); }
    | TOK_INT { $$ = expr_string("int"); }
    | TOK_USHORT { $$ = expr_string("ushort"); }
    | TOK_SHORT { $$ = expr_string("short"); }
    | TOK_UBYTE { $$ = expr_string("ubyte"); }
    | TOK_BYTE  { $$ = expr_string("byte"); }
    | TOK_ULONG { $$ = expr_string("ulong"); }
    | TOK_LONG { $$ = expr_string("long"); }
    | qualified_name { $$ = $1; }
    ;

qualified_name
    : TOK_TYPE TOK_SCOPE qualified_name { $$ = expr_strcat2($$, "::", $3); }
    | TOK_TYPE { $$ = $1; } 
    ;
    
compound_statement
    : '{' statement_list '}' { $$ = $1; }
    ;

statement_list
    : statement statement_list { $$ = expr_prepend($2, $1); }
    | /* empty */ { $$ = expr_list(); }
    ;

statement
    : assignment ';' { $$ = $1; }
    | type TOK_IDENT ';' { $$ = expr_pair($1, $2); }
    | type TOK_IDENT '=' assignment ';' { 
		$$ = expr_binary(expr_pair($1, $2), $3, $4); 
	}  
    ;

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
    | postfix '[' assignment ']' { $$ = expr_index($1, $3); }
    | postfix '.' TOK_IDENT { $$ = expr_member($1, $3); }
    | postfix TOK_INC { $$ = expr_unary(op_postinc, $1); }
    | postfix TOK_DEC { $$ = expr_unary(op_preinc, $1); }
    | primary { $$ = $1; }
    ;

primary
    : TOK_STRING { $$ = $1; }
    | TOK_NUMBER { $$ = $1; }
    | TOK_IDENT { $$ = $1; }
    | '(' assignment ')' { $2; } 
    ;
