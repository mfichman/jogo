%{
#include "Parser.hpp"
#include "Expression.hpp"
#include "Statement.hpp"
#include "Formal.hpp"
#include "Feature.hpp"
#include "Type.hpp"
#include <iostream>
#include <cassert>

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(ParseNode *node, Location *loc, void *scanner);
void yyerror(Location *loc, Parser *parser, void *scanner, const char *message);

%}

%union { Expression* expression; }
%union { Statement* statement; }
%union { Name* name; }
%union { Feature* feature; }
%union { Formal* formal; }
%union { Type* type; }
%union { Generic* generic; }
%union { Variable* variable; }
%union { int flag; }

%pure-parser
%locations
%parse-param { Parser* parser }
%parse-param { void* scanner }
%lex-param { yyscan_t* scanner }

%destructor { delete $$; $$ = 0; } <expression>
%destructor { delete $$; $$ = 0; } <identifier>
%destructor { delete $$; $$ = 0; } <statement>
%destructor { delete $$; $$ = 0; } <unit>
%destructor { delete $$; $$ = 0; } <feature>
%destructor { delete $$; $$ = 0; } <formal>
%destructor { delete $$; $$ = 0; } <type>
%destructor { delete $$; $$ = 0; } <generic>
%destructor { delete $$; $$ = 0; } <variable>


%left '?'
%left BIT_AND_ASSIGN BIT_OR_ASSIGN BIT_XOR_ASSIGN
%left ADD_ASSIGN SUBTRACT_ASSIGN
%left '=' MULTIPLY_ASSIGN DIVIDE_ASSIGN MODULUS_ASSIGN POWER_ASSIGN 
%left OR
%left AND
%left '|' XOR
%left '&'
%left EQUAL NOT_EQUAL
%left '<' '>' LESS_OR_EQUAL GREATER_OR_EQUAL COMPARE
%left LEFT_SHIFT RIGHT_SHIFT
%left '+' '-'
%left '*' '/' '%'
%left '^'
%left NOT '!' '~'
%left INCREMENT DECREMENT
%left '.' '['

/* BISON declarations */
%token <name> IDENTIFIER TYPE OPERATOR
%token <expression> STRING NUMBER BOOLEAN 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token IMPORT
%token SEPARATOR SEMICOLON
%token WHEN CASE WHILE ELSE UNTIL IF DO FOR RETURN
%token RIGHT_ARROW LEFT_ARROW
%token EQUAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL
%token OR AND
%token LEFT_SHIFT RIGHT_SHIFT
%token MULTIPLY_ASSIGN DIVIDE_ASSIGN SUBTRACT_ASSIGN ADD_ASSIGN
%token MODULUS_ASSIGN BIT_OR_ASSIGN BIT_AND_ASSIGN BIT_XOR_ASSIGN
%token INCREMENT DECREMENT
%token SCOPE LET IN YIELD FORK

%type <feature> member member_list attribute class
%type <feature> operator function prototype native 
%type <type> type type_list
%type <name> scope scope_prefix 
%type <generic> generic_list generic
%type <flag> modifiers
%type <formal> formal_signature formal_list formal
%type <statement> block when_list when statement_list statement conditional
%type <variable> variable_list variable
%type <expression> call expression expression_list


/* The Standard Apollo Grammar, version 2010 */
%%

input
    : class input {
        parser->module()->feature($1);
    }
    | import input {
//        parser->module()->feature($1);
    }
    | function input {
        parser->module()->feature($1);
    }
    | /* empty */ { }
    ;

import
    : IMPORT import_list SEPARATOR {
    }
    ;

import_list
    : scope ',' import_list {
        // TODO: Generic in the import? That's probably bad.
        parser->module()->feature(new Import(@$, $1));
    }
    | scope {
        parser->module()->feature(new Import(@$, $1));
    }
    ;

class
    : type '<' type_list '{' member_list '}' {
        $$ = new Class(@$, $1, $5);
        delete $3; // FixMe
    }
    | error {
        $$ = 0;    
    }
    ;

member_list
    : member member_list { 
        $$ = $1;
        $$->next($2);
    }
    | /* empty */ {
        $$ = 0;
    }
    ;

member
    : attribute { $$ = $1; }
    | operator { $$ = $1; }
    | function { $$ = $1; }
    | native { $$ = $1; }
    | prototype { $$ = $1; }
    ;

attribute
    : IDENTIFIER type '=' expression SEPARATOR {
        // TODO: Set symbol table for class-level
        $$ = new Attribute(@$, $1, $2, $4);
    }
    | IDENTIFIER '=' expression SEPARATOR {
        // TODO: Set symbol table for class-level
        $$ = new Attribute(@$, $1, parser->environment()->void_type(), $3);
    }
    | IDENTIFIER type SEPARATOR {
        $$ = new Attribute(@$, $1, $2, new Empty(@$));
    }
    ;

function
    : IDENTIFIER formal_signature type modifiers block {
        $$ = new Function(@$, $1, $2, $3, $5);
    }
    | IDENTIFIER formal_signature modifiers block {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, $4);
    }
    ;

operator
    : OPERATOR formal_signature type modifiers block {
        $$ = new Function(@$, $1, $2, $3, $5);
    }
    | OPERATOR formal_signature modifiers block {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, $4);
    }
    ;

prototype
    : IDENTIFIER formal_signature type modifiers SEPARATOR {
        $$ = new Function(@$, $1, $2, $3, 0);
    }
    | IDENTIFIER formal_signature modifiers SEPARATOR {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, 0);
    }
    ;

native
    : IDENTIFIER formal_signature type modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $1, $2, $3, 0);
    }
    | IDENTIFIER formal_signature modifiers NATIVE SEPARATOR {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, 0);
    }
    ;
    
formal_signature
    : '(' formal_list ')' { $$ = $2; }
    | '(' ')' { $$ = 0; }
    ;

formal_list
    : formal ',' formal_list { 
        $$ = $1;
        $$->next($3);
    }
    | formal { 
        $$ = $1;
    }
    ;

formal
    : IDENTIFIER type {
        $$ = new Formal(@$, $1, $2);
    }

modifiers
    : PRIVATE { $$ = 0; }
    | /* empty */ { $$ = 0; }
    ;

scope_prefix
    : TYPE SCOPE scope_prefix {
        $$ = parser->environment()->name($1->string() + "::" + $3->string()); 
    }
    | TYPE SCOPE {
        $$ = $1;
    }

scope
    : TYPE SCOPE scope {
        $$ = parser->environment()->name($1->string() + "::" + $3->string()); 
    }
    | TYPE {
        $$ = $1; 
    }

type_list
    : type ',' type_list {
        $$ = $1;
        $$->next($3);
    }
    | type {
        $$ = $1;
    }
    ;

type
    : scope '[' generic_list ']' {
        $$ = new Type($1, $3, parser->environment());
    }
    | scope {
        $$ = new Type($1, 0, parser->environment()); 
    }
    ;

generic_list
    : generic ',' generic_list {
        $$ = $1;
        $$->next($3);
    }
    | generic {
        $$ = $1;
    }
    ;

generic
    : type { $$ = new Generic($1); }
    ;

block
    : '{' statement_list '}' { 
        $$ = new Block(@$, $2); 
    }
    ;

statement_list
    : statement statement_list { 
        $$ = $1;
        $$->next($2);
    }
    | /* empty */ {
        $$ = 0;
    }
    | error statement_list { 
        $$ = $2; 
    }
    ;


statement
    : FOR IDENTIFIER IN expression block {
        $$ = new For(@$, $2, $4, $5);
    }
    | LET variable_list block {
        $$ = new Let(@$, $2, $3);
    }
    | UNTIL expression block {
        // TODO: FIX UNTIL
        Name* op = parser->environment()->name("!");
        $$ = new While(@$, new Unary(@$, op, $2), $3);
        
    }
    | WHILE expression block {
        $$ = new While(@$, $2, $3);
    }
    | CASE expression '{' when_list '}' {
        $$ = new Case(@$, $2, static_cast<When*>($4));
    }
    | RETURN expression SEPARATOR { 
        $$ = new Return(@$, $2);
    }
    | RETURN SEPARATOR { 
        $$ = new Return(@$, new Empty(@$));
    }
    | YIELD expression SEPARATOR {
        $$ = new Yield(@$, $2);
    }
    | FORK call SEPARATOR {
        $$ = new Fork(@$, $2);
    }
    | variable SEPARATOR {
        $$ = $1;
    }
    | expression SEPARATOR { 
        $$ = new Simple(@$, $1); 
    }
    | expression '[' expression_list ']' '=' expression SEPARATOR {
        /* Indexed assignment */
        Environment* env = parser->environment();
        $1->next($3);
        Expression* last = $3;
        while (last->next()) {
            last = last->next();
        }
        last->next($6);

        /* Invokes the special @insert method, a la Lua and Python */
        $$ = new Simple(@$, new Dispatch(@$, env->name("@insert"), $1));
    }
    | expression '.' IDENTIFIER '=' expression SEPARATOR {
        /* Attribute assignment, calls the mutator function */
        Name* name = parser->environment()->name($3->string() + "=");
        $1->next($5);
        $$ = new Simple(@$, new Dispatch(@$, name, $1));
    }
    | conditional {
        $$ = $1;
    }
    ;

conditional
    : IF expression block {
        $$ = new Conditional(@$, $2, $3, new Block(@$, 0));
    }
    | IF expression block ELSE block {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    | IF expression block ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    ;
expression
    : expression '?' expression { 
        $$ = new Binary(@$, parser->environment()->name("?"), $1, $3);
        assert(false && "Not implemented");
    }
    | expression OR expression { 
        $$ = new Binary(@$, parser->environment()->name("or"), $1, $3); 
    }
    | expression AND expression { 
        $$ = new Binary(@$, parser->environment()->name("and"), $1, $3); 
    }
    | expression XOR expression {
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@bitxor"), $1);
    }
    | expression '|' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@bitor"), $1); 
    }
    | expression '&' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@bitand"), $1); 
    }
    | expression EQUAL expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@equal"), $1); 
    }
    | expression NOT_EQUAL expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* equal = new Dispatch(@$, env->name("@equal"), $1);
        $$ = new Unary(@$, env->name("!"), equal);
    }
    | expression COMPARE expression {
        $1->next($3);
        Environment* env = parser->environment();
        $$ = new Dispatch(@$, env->name("@compare"), $1);
    }
    | expression '>' expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch(@$, env->name("@less"), $1); 
        Dispatch* equal = new Dispatch(@$, env->name("@equal"), $1);
        Binary* child = new Binary(@$, env->name("||"), less, equal); 
        $$ = new Unary(@$, env->name("!"), child); 
    }
    | expression '<' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@less"), $1); 
    }
    | expression GREATER_OR_EQUAL expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch(@$, env->name("@less"), $1);
        $$ = new Unary(@$, env->name("!"), less); 
    }
    | expression LESS_OR_EQUAL expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch(@$, env->name("@less"), $1);
        Dispatch* equal = new Dispatch(@$, env->name("@equal"), $1);
        $$ = new Binary(@$, env->name("||"), less, equal);  
    }
    | expression LEFT_SHIFT expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@shift"), $1); 
    }
    | expression RIGHT_SHIFT expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@unshift"), $1); 
    }
    | expression '^' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@power"), $1); 
    }
    | expression '+' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@add"), $1); 
    }
    | expression '-' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@subtract"), $1); 
    }
    | expression '*' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@multiply"), $1); 
    }
    | expression '/' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@divide"), $1); 
    }
    | expression '%' expression { 
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@modulus"), $1); 
    }
    | NOT expression { 
        $$ = new Unary(@$, parser->environment()->name("!"), $2); 
    }
    | '~' expression { 
        $$ = new Dispatch(@$, parser->environment()->name("@complement"), $2); 
    }
    | expression '[' expression_list ']' {
        /* Invokes the special method, a la Lua and Python */
        $1->next($3);
        $$ = new Dispatch(@$, parser->environment()->name("@index"), $1);
    }
    | expression '.' IDENTIFIER {
        /* Attribute read, calls the accessor function */
        $$ = new Dispatch(@$, $3, $1);
    }
    | expression '.' IDENTIFIER '(' expression_list ')' {
        $1->next($5);
        $$ = new Dispatch(@$, $3, $1);
    }
    | expression '.' IDENTIFIER '(' ')' {
        $$ = new Dispatch(@$, $3, $1);
    }
    | call { $$ = $1; }
    | '(' expression ')' { $$ = $2; } 
    | STRING { $$ = $1; }
    | NUMBER { $$ = $1; }
    | BOOLEAN { $$ = $1; }
    | IDENTIFIER { $$ = new Identifier(@$, $1); }
    ;

expression_list
    : expression ',' expression_list { 
        $$ = $1;
        $$->next($3);
    }
    | expression { $$ = $1; } 
    ;

variable_list
    : variable ',' variable_list {
        $$ = $1;
        $$->next($3);
    }
    | expression ',' variable_list {
        Name* name = parser->environment()->name("__unused");
        $$ = new Variable(@$, name, parser->environment()->void_type(), $1);
        $$->next($3);
    }
    | variable {
        $$ = $1;
    }
    | expression {
        Name* name = parser->environment()->name("__unused");
        $$ = new Variable(@$, name, parser->environment()->void_type(), $1);
    }
    ;

variable
    : IDENTIFIER type {
        $$ = new Variable(@$, $1, $2, new Empty(@$));
    }
    | IDENTIFIER type '=' expression {
        $$ = new Variable(@$, $1, $2, $4);
    }
    | IDENTIFIER '=' expression {
        $$ = new Variable(@$, $1, parser->environment()->void_type(), $3);
    }
    ;

call
    : IDENTIFIER '(' expression_list ')' {
        $$ = new Call(@$, 0, $1, $3);
    }
    | IDENTIFIER '(' ')' {
        /* Call on an object expression */
        $$ = new Call(@$, 0, $1, 0);
    }
    | scope_prefix IDENTIFIER '(' expression_list ')' {
        $$ = new Call(@$, $1, $2, $4);
    } 
    | scope_prefix IDENTIFIER '(' ')' {
        $$ = new Call(@$, $1, $2, 0);
    }
    | type '(' expression_list ')' {
        $$ = new Construct(@$, $1, $3);
    }
    | type '(' ')' {
        $$ = new Construct(@$, $1, 0);
    } 

when_list
    : when when_list {
        $$ = $1;
        $$->next($2);
    }
    | when {
        $$ = $1;
    }
    ;

when
    : WHEN IDENTIFIER type block {
        $$ = new When(@$, $2, $3, $4);
    }
    ;
