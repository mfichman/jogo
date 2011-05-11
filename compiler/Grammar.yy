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

#define ID(X) parser->environment()->name((X))
// Shortcut, adds an identifier name to the current environment

#define NOTYPE parser->environment()->no_type()
#define ENV parser->environment()
#define MODULE parser->module()

%}

%union { Expression* expression; }
%union { Statement* statement; }
%union { String* string; }
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
%destructor { delete $$; $$ = 0; } <statement>
%destructor { delete $$; $$ = 0; } <string>
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
%token <string> IDENTIFIER TYPE OPERATOR COMMENT
%token <expression> STRING NUMBER BOOLEAN 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token IMPORT FUNCTION
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

%type <feature> member member_list attribute class method function 
%type <type> type type_list return_signature class_name
%type <string> scope scope_list comment method_name 
%type <generic> generic generic_list
%type <flag> modifiers
%type <formal> formal_signature formal formal_list
%type <statement> block when when_list statement statement_list
%type <statement> conditional method_body
%type <variable> variable variable_list
%type <expression> call expression expression_list


/* The Standard Apollo Grammar, version 2010 */
%%

module
    : class module { MODULE->feature($1); }
    | function module { MODULE->feature($1); }
    | IMPORT import_list SEPARATOR module {}
    | error module {}
    | /* empty */ { }
    ;

class_name
    : TYPE '[' generic_list ']' { $$ = new Type(@$, $1, $3, MODULE, ENV); }
    | TYPE { $$ = new Type(@$, $1, 0, MODULE, ENV); }
    ;

class
    : class_name '<' type_list '{' comment member_list '}' {
        std::string qn = MODULE->name()->string();
        qn += "::" + $1->name()->string();
        Type* type = new Type(@$, ID(qn), $1->generics(), MODULE, ENV);
        delete $1; // Delete the short type name after concat
        $$ = new Class(@$, type, $3, $5, $6);
    }
    ;

function
    : IDENTIFIER formal_signature return_signature modifiers block {
        $$ = new Function(@$, $1, $2, $3, $5);
    }
    ;

import_list
    : scope ',' import_list { MODULE->feature(new Import(@$, $1)); }
    | scope { MODULE->feature(new Import(@$, $1)); }
    ;


member_list
    : member member_list { $$ = $1; $$->next($2); }
    | error member_list { $$ = $2; }
    | /* empty */ { $$ = 0; }
    ;

member
    : attribute { $$ = $1; }
    | method { $$ = $1; }
    ;

attribute
    : IDENTIFIER type '=' expression SEPARATOR {
        $$ = new Attribute(@$, $1, $2, $4);
    }
    | IDENTIFIER '=' expression SEPARATOR {
        $$ = new Attribute(@$, $1, NOTYPE, $3);
    }
    | IDENTIFIER type SEPARATOR {
        $$ = new Attribute(@$, $1, $2, new Empty(@$));
    }
    ;

method_name
    : IDENTIFIER { $$ = $1; }
    | OPERATOR { $$ = $1; }
    ;

method_body
    : block { $$ = $1; }
    | SEPARATOR { $$ = 0; }
    ;

method
    : method_name formal_signature return_signature modifiers method_body {
        Type* type = ENV->self_type();
        Formal* self = new Formal(@$, ID("self"), type);
        self->next($2);
        $$ = new Function(@$, $1, self, $3, $5);
    }
    ;

return_signature
    : /* empty */ { $$ = ENV->void_type(); }
    | type { $$ = $1; }
    ;

formal_signature
    : '(' formal_list ')' { $$ = $2; }
    | '(' ')' { $$ = 0; }
    ;

formal_list
    : formal ',' formal_list { $$ = $1; $$->next($3); }
    | formal { $$ = $1; }
    ;

formal
    : IDENTIFIER type { $$ = new Formal(@$, $1, $2); }
    ;

modifiers
    : PRIVATE { $$ = 0; }
    | PRIVATE NATIVE { $$ = 0; }
    | NATIVE { $$ = 0; }
    | /* empty */ { $$ = 0; }
    ;

scope_list
    : TYPE SCOPE scope_list { $$ = ID($1->string() + "::" + $3->string()); }
    | TYPE SCOPE { $$ = $1; }
    ;

scope
    : TYPE SCOPE scope { $$ = ID($1->string() + "::" + $3->string()); }
    | TYPE { $$ = $1; }
    ;

type_list
    : type ',' type_list { $$ = $1; $$->next($3); }
    | type { $$ = $1; }
    ;

type
    : scope '[' generic_list ']' { $$ = new Type(@$, $1, $3, MODULE, ENV); }
    | scope { $$ = new Type(@$, $1, 0, MODULE, ENV); }
    ;

generic_list
    : generic ',' generic_list { $$ = $1; $$->next($3); }
    | generic { $$ = $1; }
    ;

generic
    : type { $$ = new Generic($1); }
    ;

block
    : '{' comment statement_list '}' { $$ = new Block(@$, $2, $3); }
    ;

comment
    : /* empty */ { $$ = new String(""); }
    | COMMENT comment {
        if ($1->string().empty()) {
            $$ = new String($1->string() + "\n\n" + $2->string());
        } else {
            $$ = new String($1->string() + " " + $2->string());
        }
        delete $1; delete $2;
    }
    ;

statement_list
    : statement statement_list { $$ = $1; $$->next($2); }
    | error statement_list { $$ = $2; }
    | /* empty */ { $$ = 0; }
    ;

statement
    : FOR IDENTIFIER IN expression block { $$ = new For(@$, $2, $4, $5); }
    | LET variable_list block { $$ = new Let(@$, $2, $3); }
    | WHILE expression block { $$ = new While(@$, $2, $3); }
    | RETURN expression SEPARATOR { $$ = new Return(@$, $2); }
    | RETURN SEPARATOR { $$ = new Return(@$, new Empty(@$)); }
    | YIELD expression SEPARATOR { $$ = new Yield(@$, $2); }
    | FORK call SEPARATOR { $$ = new Fork(@$, $2); }
    | variable SEPARATOR { $$ = $1; }
    | expression SEPARATOR { $$ = new Simple(@$, $1); }
    | conditional { $$ = $1; }
    | CASE expression '{' comment when_list '}' {
        $$ = new Case(@$, $2, static_cast<When*>($5));
        delete $4; // '{' may introduce a comment, but we discard it
    }
    | expression '[' expression_list ']' '=' expression SEPARATOR {
        /* Indexed assignment operator */
        Expression* last = $3;
        while (last->next()) {
            last = last->next();
        }
        last->next($6);

        /* Invokes the special @insert method, a la Lua and Python */
        $$ = new Simple(@$, new Dispatch(@$, ID("@insert"), $1, $3));
    }
    | expression '.' IDENTIFIER '=' expression SEPARATOR {
        /* Attribute assignment, calls the mutator function */
        $$ = new Simple(@$, new Dispatch(@$, ID($3->string() + "="), $1, $5));
    }
    ;

expression_list
    : expression ',' expression_list { $1->next($3); $$ = $1; }
    | expression { $$ = $1; } 
    ;


expression
    : expression '?' expression { $$ = new Binary(@$, ID("?"), $1, $3); }
    | expression '^' expression { $$ = new Dispatch(@$, ID("@pow"), $1, $3); } 
    | expression '+' expression { $$ = new Dispatch(@$, ID("@add"), $1, $3); } 
    | expression '-' expression { $$ = new Dispatch(@$, ID("@sub"), $1, $3); } 
    | expression '*' expression { $$ = new Dispatch(@$, ID("@mul"), $1, $3); } 
    | expression '/' expression { $$ = new Dispatch(@$, ID("@div"), $1, $3); } 
    | expression '%' expression { $$ = new Dispatch(@$, ID("@mod"), $1, $3); } 
    | NOT expression { $$ = new Unary(@$, ID("not"), $2); }
    | '~' expression { $$ = new Dispatch(@$, ID("@compl"), $2, 0); } 
    | call { $$ = $1; }
    | '(' expression ')' { $$ = $2; } 
    | STRING { $$ = $1; }
    | NUMBER { $$ = $1; }
    | BOOLEAN { $$ = $1; }
    | IDENTIFIER { $$ = new Identifier(@$, $1); }
    | expression OR expression { $$ = new Binary(@$, ID("or"), $1, $3); }
    | expression AND expression { $$ = new Binary(@$, ID("and"), $1, $3); }
    | expression XOR expression {
        $$ = new Dispatch(@$, ID("@bitxor"), $1, $3);
    }
    | expression '|' expression { 
        $$ = new Dispatch(@$, ID("@bitor"), $1, $3); 
    }
    | expression '&' expression { 
        $$ = new Dispatch(@$, ID("@bitand"), $1, $3); 
    }
    | expression EQUAL expression { 
        $$ = new Dispatch(@$, ID("@equal"), $1, $3); 
    }
    | expression NOT_EQUAL expression { 
        $$ = new Unary(@$, ID("not"), new Dispatch(@$, ID("@equal"), $1, $3));
    }
    | expression COMPARE expression {
        $$ = new Dispatch(@$, ID("@compare"), $1, $3);
    }
    | expression '>' expression { 
        Dispatch* less = new Dispatch(@$, ID("@less"), $1, $3); 
        Dispatch* equal = new Dispatch(@$, ID("@equal"), $1, $3);
        Binary* child = new Binary(@$, ID("||"), less, equal); 
        $$ = new Unary(@$, ID("not"), child); 
    }
    | expression '<' expression {
        $$ = new Dispatch(@$, ID("@less"), $1, $3); 
    }
    | expression GREATER_OR_EQUAL expression { 
        $$ = new Unary(@$, ID("not"), new Dispatch(@$, ID("@less"), $1, $3)); 
    }
    | expression LESS_OR_EQUAL expression { 
        Dispatch* less = new Dispatch(@$, ID("@less"), $1, $3);
        Dispatch* equal = new Dispatch(@$, ID("@equal"), $1, $3);
        $$ = new Binary(@$, ID("||"), less, equal);  
    }
    | expression LEFT_SHIFT expression { 
        $$ = new Dispatch(@$, ID("@shift"), $1, $3); 
    }
    | expression RIGHT_SHIFT expression { 
        $$ = new Dispatch(@$, ID("@unshift"), $1, $3); 
    }
    | expression '[' expression_list ']' {
        /* Invokes the special method, a la Lua and Python */
        $$ = new Dispatch(@$, ID("@index"), $1, $3);
    }
    | expression '.' IDENTIFIER { 
        $$ = new Dispatch(@$, $3, $1, 0);
    }
    | expression '.' IDENTIFIER '(' expression_list ')' {
        $$ = new Dispatch(@$, $3, $1, $5);
    }
    | expression '.' IDENTIFIER '(' ')' {
        $$ = new Dispatch(@$, $3, $1, 0);
    }
    | FUNCTION formal_signature return_signature block {
        // anonymous function!
        //String* name = ID("@call");
        //$$ = new Function(@$, name, $2, $3, $4);
        $$ = new Empty(@$);
        $2 = 0;
        $4 = 0;
        $3 = 0;
        parser->force_separator();
    }
    ;

conditional
    : IF expression block ELSE block { $$ = new Conditional(@$, $2, $3, $5); }
    | IF expression block {
        $$ = new Conditional(@$, $2, $3, new Block(@$, 0, 0));
    }
    | IF expression block ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    ;

call
    : type '(' expression_list ')' { $$ = new Construct(@$, $1, $3); }
    | type '(' ')' { $$ = new Construct(@$, $1, 0); }
    | IDENTIFIER '(' expression_list ')' { $$ = new Call(@$, 0, $1, $3); }
    | IDENTIFIER '(' ')' { $$ = new Call(@$, 0, $1, 0); }
    | scope_list IDENTIFIER '(' ')' { $$ = new Call(@$, $1, $2, 0); }
    | scope_list IDENTIFIER '(' expression_list ')' {
        $$ = new Call(@$, $1, $2, $4);
    } 
    ; 

when_list
    : when when_list { $1->next($2); $$ = $1; }
    | when { $$ = $1; }
    ;


variable_list
    : variable ',' variable_list { $1->next($3); $$ = $1; }
    | variable { $$ = $1; }
    | expression { $$ = new Variable(@$, ID(""), NOTYPE, $1); }
    | expression ',' variable_list {
        $$ = new Variable(@$, ID(""), NOTYPE, $1);
        $$->next($3);
    }
    ;

variable
    : IDENTIFIER type { $$ = new Variable(@$, $1, $2, new Empty(@$)); }
    | IDENTIFIER type '=' expression { $$ = new Variable(@$, $1, $2, $4); }
    | IDENTIFIER '=' expression { $$ = new Variable(@$, $1, NOTYPE, $3); }
    ;

when
    : WHEN expression block { $$ = new When(@$, $2, $3); }
    ;
