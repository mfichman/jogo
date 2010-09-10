%{
#include <apparser.h>
#include <apexpr.h>
#include <aptype.h>
#include <apstmt.h>
#include <apvar.h>
#include <apfunc.h>
#include <apunit.h>
#include <apimport.h>
#include <apsymtab.h>
#include <apdef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYSTYPE apnode_t
#define YYLTYPE aploc_t
#define YY_EXTRA_TYPE apparser_t *
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(apnode_t *node, aploc_t *loc, void *scanner);
void yyerror(aploc_t *loc, apparser_t *parser, void *scanner, const char *message);

%}

%union { apexpr_t *expr; }
%union { apstmt_t *stmt; }
%union { aptype_t *type; }
%union { apunit_t *unit; }
%union { apvar_t *var; }
%union { apfunc_t *func; }
%union { apimport_t *import; }
%union { apdef_t *def; }
%union { char *string; }
%union { int null; }
%union { int flag; }

%pure_parser
%locations
%parse-param { apparser_t *parser }
%parse-param { void *scanner }
%lex-param { yyscan_t *scanner }

%destructor { apexpr_free($$); $$ = 0; } <expr>
%destructor { apstmt_free($$); $$ = 0; } <stmt>
%destructor { aptype_free($$); $$ = 0; } <type>
%destructor { apunit_free($$); $$ = 0; } <unit>
%destructor { apvar_free($$); $$ = 0; } <var>
%destructor { apfunc_free($$); $$ = 0; } <func>
%destructor { free($$); $$ = 0; } <string>


/* BISON declarations */
%token <type> TOK_PRIMITIVE
%token <string> TOK_IDENT
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
%token <flag> TOK_STATIC
%token <flag> TOK_NATIVE
%token TOK_WHILE
%token TOK_ELSE
%token TOK_UNTIL
%token TOK_IF
%token TOK_DO
%token TOK_FOR
%token TOK_RETURN
%token TOK_EQUAL
%token TOK_NOTEQUAL
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
%type <func> prototype
%type <func> native
%type <flag> modifiers
%type <var> parameter_signature
%type <var> parameter_list
%type <type> type
%type <stmt> block
%type <stmt> statement_list
%type <stmt> statement
%type <stmt> clause
%type <stmt> conditional
%type <expr> argument_list
%type <expr> expression
%type <expr> nullable
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
    : TOK_CLASS TOK_IDENT ';' class {
        apunit_name($4, $2); 
		apparser_class(parser, $4);
		
    }
    | TOK_INTERFACE TOK_IDENT ';' interface {
        apunit_name($4, $2); 
		apparser_interface(parser, $4);
    }
    | TOK_STRUCT TOK_IDENT ';' struct {
        apunit_name($4, $2); 
		apparser_struct(parser, $4);
    }
    | TOK_MODULE TOK_IDENT ';' module {
        apunit_name($4, $2);
		apparser_module(parser, $4);
    }
	| /* empty is an error */ { 
		yyerror(&@$, parser, scanner, "Input file is empty"); 
		YYERROR;
	}
	| error { }
    ;

class
    : import class { $$ = $2; apunit_import($$, $1); }
    | def class { $$ = $2; apunit_def($$, $1); }
    | variable class { $$ = $2; apunit_var($$, $1); }
    | constructor class { $$ = $2; apunit_ctor($$, $1); }
    | destructor class { $$ = $2; apunit_dtor($$, $1); }
    | function class { $$ = $2; apunit_func($$, $1); }
	| native class { $$ = $2; apunit_func($$, $1); }
	| error class { $$ = $2; }
    | /* empty */ { 
		$$ = apunit_alloc(strdup(parser->filename), APUNIT_TYPE_CLASS); 
	}
    ;

interface
    : import interface { $$ = $2; apunit_import($$, $1); }
    | def interface { $$ = $2; apunit_def($$, $1); }
	| prototype interface { $$ = $2; apunit_func($$, $1); }
	| variable interface { 
		$$ = $2;
		apvar_free($1);
		yyerror(&@$, parser, scanner, "Instance variable in interface");
		YYERROR;
	}
    | constructor interface {
		$$ = $2;
		apfunc_free($1);
		yyerror(&@$, parser, scanner, "Interfaces cannot have constructors");
		YYERROR;
	}
    | destructor interface { 
		$$ = $2;
		apfunc_free($1);
		yyerror(&@$, parser, scanner, "Interfaces cannot have destructors");
		YYERROR;
	}
	| error interface { $$ = $2; }
    | /* empty */ { 
		$$ = apunit_alloc(strdup(parser->filename), APUNIT_TYPE_INTERFACE);
	}
    ;

struct
    : import struct { $$ = $2; apunit_import($$, $1); }
    | def struct { $$ = $2; apunit_def($$, $1); }
    | variable struct { $$ = $2; apunit_var($$, $1); }
    | constructor struct { $$ = $2; apunit_ctor($$, $1); }
    | function struct { $$ = $2; apunit_func($$, $1); }
	| prototype struct { $$ = $2; apunit_func($$, $1); }
	| destructor struct {
		$$ = $2;
		apfunc_free($1);
		yyerror(&@$, parser, scanner, "Structs cannot have destructors");
		YYERROR;
	}
	| error struct { $$ = $2; }
    | /* empty */ { 
		$$ = apunit_alloc(strdup(parser->filename), APUNIT_TYPE_STRUCT); 
	}
    ;

module
    : import module { $$ = $2; apunit_import($$, $1); }
    | def module { $$ = $2; apunit_def($$, $1); }
	| variable module { $$ = $2; apunit_var($$, $1); }
	| constructor module {
		$$ = $2;
		apfunc_free($1);
		yyerror(&@$, parser, scanner, "Modules cannot have constructors");
		YYERROR;
	}
	| destructor module {
		$$ = $2;
		apfunc_free($1);
		yyerror(&@$, parser, scanner, "Modules cannot have destructors");
		YYERROR;
	}
    | function module { $$ = $2; apunit_func($$, $1); }
	| error module { $$ = $2; }
    | /* empty */ { 
		$$ = apunit_alloc(strdup(parser->filename), APUNIT_TYPE_MODULE); 
	}
    ;
    
import
    : TOK_IMPORT TOK_IDENT ';' { 
		$$ = apimport_alloc(&@$, strdup(parser->filename), aptype_object($2));
	}
    ;

def
    : TOK_DEF type TOK_IDENT ';' { 
		$$ = apdef_alloc($2, aptype_object($3)); 
	}
    ;

variable
    : type TOK_IDENT '=' expression ';' {
		// TODO: Set symbol table for class-level
		$$ = apvar_alloc($2, 0, $1, $4);
    }
    | type TOK_IDENT ';' {
		$$ = apvar_alloc($2, 0, $1, 0);
    }
	;

constructor
    : TOK_INIT parameter_signature block { 
		$$ = apfunc_alloc(strdup("@init"), $2, 0, $3);
		$$->flags &= APUNIT_FLAG_STATIC;
	}
    ;

destructor
    : TOK_DESTROY '(' ')' block { 
		$$ = apfunc_alloc(strdup("@destroy"), 0, 0, $4); 
	}
	| TOK_DESTROY '(' parameter_list ')' block {
		apvar_free($3);
		apstmt_free($5);
		$$ = 0;
		yyerror(&@$, parser, scanner, "Destructors cannot take arguments"); 
		YYERROR;
	}
    ;

function
    : TOK_IDENT parameter_signature modifiers type block {
		$$ = apfunc_alloc($1, $2, $4, $5);
		$$->flags = $3;
    }
	| TOK_IDENT parameter_signature modifiers block {
		$$ = apfunc_alloc($1, $2, 0, $4);
		$$->flags = $3;
	}
	;

prototype
	: TOK_IDENT parameter_signature modifiers type ';' {
		$$ = apfunc_alloc($1, $2, $4, 0);
		$$->flags = $3;
	}
	| TOK_IDENT parameter_signature modifiers ';' {
		$$ = apfunc_alloc($1, $2, 0, 0);
		$$->flags = $3;
	}
    ;

native
	: TOK_IDENT parameter_signature modifiers TOK_NATIVE type ';' {
		$$ = apfunc_alloc($1, $2, $5, 0);
		$$->flags = $3|APUNIT_FLAG_NATIVE;
	}
	| TOK_IDENT parameter_signature modifiers TOK_NATIVE ';' {
		$$ = apfunc_alloc($1, $2, 0, 0);
		$$->flags = $3|APUNIT_FLAG_NATIVE;
	}
    ;
	


parameter_signature
	: '(' parameter_list ')' { $$ = $2; }
	| '(' ')' { $$ = 0; }
	;

parameter_list
	: type TOK_IDENT ',' parameter_list { 
		$$ = apvar_alloc($2, 0, $1, 0);
		$$->next = $4;
	}
	| type TOK_IDENT { 
		$$ = apvar_alloc($2, 0, $1, 0);
	}
    ;

modifiers
	: TOK_PUBLIC { $$ = APUNIT_FLAG_PUBLIC; }
	| TOK_PRIVATE { $$ = APUNIT_FLAG_PRIVATE; }
	| TOK_STATIC { $$ = APUNIT_FLAG_STATIC; }
	| TOK_PUBLIC TOK_STATIC { $$ = APUNIT_FLAG_PUBLIC|APUNIT_FLAG_STATIC; }
	| TOK_PRIVATE TOK_STATIC { $$ = APUNIT_FLAG_PRIVATE|APUNIT_FLAG_STATIC; }
	| /* empty */ { $$ = 0; }
	;

type
    : TOK_PRIMITIVE { $$ = $1; } 
	| TOK_PRIMITIVE '*' { $$ = $1; $$->flags &= APTYPE_FLAG_ARRAY; }
	| TOK_PRIMITIVE '?' { $$ = $1; $$->flags &= APTYPE_FLAG_NULLABLE; }
    | TOK_IDENT { apparser_resolve_type(parser, $$ = aptype_object($1)); }
    ;

block
    : '{' statement_list '}' { 
		$$ = $2; 
		$$->loc = @$;
	}
    ;

statement_list
    : statement_list statement { 
		$$ = apstmt_append($1, $2); 
	}
	| statement_list error ';' { $$ = $1; }
    | /* empty */ { 
		$$ = apstmt_block(&@$);
	}
    ;

statement
	: TOK_FOR '(' clause ';' clause ';' clause ')' block {
		apstmt_t *gd[] = { $3, $5, $7 };
		$$ = apstmt_for(&@$, gd, $9);
	}
	| TOK_FOR '(' type TOK_IDENT ':' expression ')' block {
		$$ = apstmt_foreach(&@$, apvar_alloc($4, 0, $3, $6), $8);
	}
	| TOK_UNTIL '(' clause ')' block {
		$$ = apstmt_until(&@$, $3, $5);
	}
	| TOK_WHILE '(' clause ')' block {
		$$ = apstmt_while(&@$, $3, $5);
	}
	| TOK_DO block TOK_UNTIL '(' clause ')' ';' {
		$$ = apstmt_dountil(&@$, $2, $5);
	}
	| TOK_DO block TOK_WHILE '(' clause ')' ';' {
		$$ = apstmt_dowhile(&@$, $2, $5);
	}
    | type TOK_IDENT ';' { 
		$$ = apstmt_decl(&@$, apvar_alloc($2, 0, $1, 0)); 
	}
	| conditional { $$ = $1; }
	| clause ';' { $$ = $1; }
	| TOK_RETURN expression ';' { $$ = apstmt_return(&@$, $2); }
	| TOK_RETURN ';' { $$ = apstmt_return(&@$, 0); }
	;

clause
    : type TOK_IDENT '=' expression { 
		$$ = apstmt_decl(&@$, apvar_alloc($2, 0, $1, $4)); 
	}  
    | expression { 
		$$ = apstmt_expr(&@$, $1); 
	}
	| /* empty */ { $$ = 0; } 
	;

conditional
	: TOK_IF '(' clause ')' block {
		$$ = apstmt_cond(&@$, $3, $5, 0);
	}
	| TOK_IF '(' clause ')' block TOK_ELSE conditional {
		$$ = apstmt_cond(&@$, $3, $5, $7);
	}
	| block { $$ = $1; }
	;

expression : nullable { $$ = $1; }

nullable
	: nullable '?' assignment { 
		$$ = apexpr_binary(&@$, strdup("?"), $1, $3);
	}
	| assignment
	;	

assignment
    : unary '=' assignment { 
		$$ = apexpr_binary(&@$, strdup("="), $1, $3); 
	}
    | unary TOK_MUL_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("*="), $1, $3); 
	}
    | unary TOK_DIV_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("/="), $1, $3); 
	}
    | unary TOK_MOD_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("%="), $1, $3); 
	}
    | unary TOK_SUB_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("-="), $1, $3); 
	}
    | unary TOK_ADD_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("+="), $1, $3); 
	}
    | unary TOK_BITAND_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("&="), $1, $3); 
	}
    | unary TOK_BITOR_ASSIGN assignment { 
		$$ = apexpr_binary(&@$, strdup("|="), $1, $3); 
	}
    | logical_or { $$ = $1; }
    ;

logical_or
    : logical_or TOK_OR logical_and { 
		$$ = apexpr_binary(&@$, strdup("||"), $1, $3); 
	}
    | logical_and { $$ = $1; }
    ;

logical_and
    : logical_and TOK_AND bitwise_or { 
		$$ = apexpr_binary(&@$, strdup("&&"), $1, $3); 
	}
    | bitwise_or { $$ = $1; }
    ;

bitwise_or
    : bitwise_or '|' bitwise_and { 
		$$ = apexpr_binary(&@$, strdup("|"), $1, $3); 
	}
    | bitwise_or '^' bitwise_and { 
		$$ = apexpr_binary(&@$, strdup("^"), $1, $3); 
	}
    | bitwise_and { $$ = $1; }
    ;

bitwise_and
    : bitwise_and '&' equality { 
		$$ = apexpr_binary(&@$, strdup("&"), $1, $3); 
	}
    | equality { $$ = $1; }
    ;

equality
    : equality TOK_EQUAL relation { 
		$$ = apexpr_binary(&@$, strdup("=="), $1, $3); 
	}
    | equality TOK_NOTEQUAL relation { 
		$$ = apexpr_binary(&@$, strdup("!="), $1, $3); 
	}
    | relation { $$ = $1; }
    ;

relation
    : relation '>' shift { 
		$$ = apexpr_binary(&@$, strdup(">"), $1, $3); 
	}
    | relation '<' shift { 
		$$ = apexpr_binary(&@$, strdup("<"), $1, $3); 
	}
    | relation TOK_GE shift { 
		$$ = apexpr_binary(&@$, strdup(">="), $1, $3); 
	}
    | relation TOK_LE shift { 
		$$ = apexpr_binary(&@$, strdup("<="), $1, $3); 
	}
    | shift { $$ = $1; }
    ;

shift
    : shift TOK_LSHIFT addition { 
		$$ = apexpr_binary(&@$, strdup("<<"), $1, $3); 
	}
    | shift TOK_RSHIFT addition { 
		$$ = apexpr_binary(&@$, strdup(">>"), $1, $3); 
	}
    | addition { $$ = $1; }
    ;

addition
    : addition '+' multiplication { 
		$$ = apexpr_binary(&@$, strdup("+"), $1, $3); 
	}
    | addition '-' multiplication { 
		$$ = apexpr_binary(&@$, strdup("-"), $1, $3); 
	}
    | multiplication { $$ = $1; }
    ;

multiplication
    : multiplication '*' unary { 
		$$ = apexpr_binary(&@$, strdup("*"), $1, $3); 
	}
    | multiplication '/' unary { 
		$$ = apexpr_binary(&@$, strdup("/"), $1, $3); 
	}
    | multiplication '%' unary { 
		$$ = apexpr_binary(&@$, strdup("%"), $1, $3); 
	}
    | unary { $$ = $1; }
    ;

unary
    : TOK_INC unary { $$ = apexpr_unary(&@$, strdup("++"), $2); }
    | TOK_DEC unary { $$ = apexpr_unary(&@$, strdup("--"), $2); }
    | '+' unary { $$ = apexpr_unary(&@$, strdup("+"), $2); }
    | '-' unary { $$ = apexpr_unary(&@$, strdup("-"), $2); }
    | '!' unary { $$ = apexpr_unary(&@$, strdup("!"), $2); }
    | '~' unary { $$ = apexpr_unary(&@$, strdup("~"), $2); }
    | '*' unary { $$ = apexpr_unary(&@$, strdup("*"), $2); }
    | postfix { $$ = $1; }
    ;

postfix
	: postfix '(' argument_list ')' {
		/* Call on an object expression */
		$$ = apexpr_call(&@$, $1, $3);
	}
	| postfix '(' ')' {
		/* Call on an object expression */
		$$ = apexpr_call(&@$, $1, 0);
	}
    | postfix '.' TOK_IDENT { 
		/* Member variable access */
		$$ = apexpr_member(&@$, $1, $3); 
	}
    | postfix '[' expression ']' { 
		$$ = apexpr_index(&@$, $1, $3); 
	}
    | postfix TOK_INC { 
		$$ = apexpr_unary(&@$, strdup("++"), $1); 
	}
    | postfix TOK_DEC { 
		$$ = apexpr_unary(&@$, strdup("--"), $1); 
	}
    | primary { $$ = $1; }
    ;

primary
	: '(' expression ')' { $$ = $2; } 
    | TOK_STRING { $$ = $1; }
    | TOK_NUMBER { $$ = $1; }
	| TOK_IDENT { $$ = apexpr_ident(&@$, $1); }
    ;

argument_list
	: expression ',' argument_list { 
		$$ = $1;
		$$->next = $3;
	}
	| expression { $$ = $1; } 
	;
