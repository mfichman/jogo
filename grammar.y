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
    : TOK_PUBLIC { $$ = node_string("public"); }
    | TOK_PRIVATE { $$ = node_string("private"); }
    | TOK_PROTECTED { $$ = node_string("protected"); }
    | /* empty */ { $$ = 0; }
    ;

storage
    : TOK_STATIC { $$ = node_string("static"); }
    | /* empty */ { $$ = 0; }
    ;
    
native
    : TOK_NATIVE { $$ = node_string("native"); }
    | /* empty */ { $$ = 0; }
    ;

type 
    : TOK_UINT { $$ = node_string("uint"); }
    | TOK_INT { $$ = node_string("int"); }
    | TOK_USHORT { $$ = node_string("ushort"); }
    | TOK_SHORT { $$ = node_string("short"); }
    | TOK_UBYTE { $$ = node_string("ubyte"); }
    | TOK_BYTE  { $$ = node_string("byte"); }
    | TOK_ULONG { $$ = node_string("ulong"); }
    | TOK_LONG { $$ = node_string("long"); }
    | qualified_name { $$ = $1; }
    ;

qualified_name
    : TOK_TYPE TOK_SCOPE qualified_name { $$ = node_strcat2($$, "::", $3); }
    | TOK_TYPE { $$ = $1; } 
    ;
    
compound_statement
    : '{' statement_list '}' { $$ = $1; }
    ;

statement_list
    : statement statement_list { $$ = node_prepend($2, $1); }
    | /* empty */ { $$ = node_list(); }
    ;

statement
    : assignment ';' { $$ = $1; }
    | type TOK_IDENT ';' { $$ = node_pair($1, $2); }
    | type TOK_IDENT '=' assignment ';' { 
		$$ = node_trinary(node_pair($1, $2), $3, $4); 
	}  
    ;

assignment
    : unary '=' assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_MUL_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_DIV_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_MOD_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_SUB_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_ADD_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_BITAND_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | unary TOK_BITOR_ASSIGN assignment { $$ = node_binary($2, $1, $3); }
    | logical_or { $$ = $1; }
    ;

logical_or
    : logical_or TOK_OR logical_and { $$ = node_binary($2, $1, $3); }
    | logical_and { $$ = $1; }
    ;

logical_and
    : logical_and TOK_OR bitwise_or { $$ = node_binary($2, $1, $3); }
    | bitwise_or { $$ = $1; }
    ;

bitwise_or
    : bitwise_or '|' bitwise_and { $$ = node_binary($2, $1, $3); }
    | bitwise_or '^' bitwise_and { $$ = node_binary($2, $1, $3); }
    | bitwise_and { $$ = $1; }
    ;

bitwise_and
    : bitwise_and '&' equality { $$ = node_binary($2, $1, $3); }
    | equality { $$ = $1; }
    ;

equality
    : equality TOK_EQUAL relation { $$ = node_binary($2, $1, $3); }
    | equality TOK_NOTEQUAL relation { $$ = node_binary($2, $1, $3); }
    | relation { $$ = $1; }
    ;

relation
    : relation '>' shift { $$ = node_binary($2, $1, $3); }
    | relation '<' shift { $$ = node_binary($2, $1, $3); }
    | relation TOK_GE shift { $$ = node_binary($2, $1, $3); }
    | relation TOK_LE shift { $$ = node_binary($2, $1, $3); }
    | shift { $$ = $1; }
    ;

shift
    : shift TOK_LSHIFT addition { $$ = node_binary($2, $1, $3); }
    | shift TOK_RSHIFT addition { $$ = node_binary($2, $1, $3); }
    | addition { $$ = $1; }
    ;

addition
    : addition '+' multiplication { $$ = node_binary($2, $1, $3); }
    | addition '-' multiplication { $$ = node_binary($2, $1, $3); }
    | multiplication { $$ = $1; }
    ;

multiplication
    : multiplication '*' unary { $$ = node_binary($2, $1, $3); }
    | multiplication '/' unary { $$ = node_binary($2, $1, $3); }
    | multiplication '%' unary { $$ = node_binary($2, $1, $3); }
    | unary { $$ = $1; }
    ;

unary
    : TOK_INC unary { $$ = node_unary($1, $2); }
    | TOK_DEC unary { $$ = node_unary($1, $2); }
    | '+' unary { $$ = node_unary($1, $2); }
    | '-' unary { $$ = node_unary($1, $2); }
    | '!' unary { $$ = node_unary($1, $2); }
    | '~' unary { $$ = node_unary($1, $2); }
    | '*' unary { $$ = node_unary($1, $2); }
    | postfix { $$ = $1; }
    ;

postfix
    : postfix argument_list { $$ = node_call($1, $2); }
    | postfix '[' assignment ']' { $$ = node_index($1, $3); }
    | postfix '.' TOK_IDENT { $$ = node_member($1, $3); }
    | postfix TOK_INC { $$ = node_postfix($2, $1); }
    | postfix TOK_DEC { $$ = node_postfix($2, $1); }
    | primary { $$ = $1; }
    ;

primary
    : TOK_STRING { $$ = $1; }
    | TOK_NUMBER { $$ = $1; }
    | TOK_IDENT { $$ = $1; }
    | '(' assignment ')' { $2; } 
    ;
