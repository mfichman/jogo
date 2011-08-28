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
void yyerror(Location *loc, Parser *parser, void *scanner, const char *msg);

#define YYLLOC_DEFAULT(current, rhs, n) {\
    current.first_line = rhs[1].first_line;\
    current.first_column = rhs[1].first_column;\
    current.last_line = rhs[n].last_line;\
    current.last_column = rhs[n].last_column;\
    current.file_name = parser->file()->name();\
}

#define ID(X) parser->environment()->name((X))
// Shortcut, adds an identifier name to the current environment

#define ENV parser->environment()
#define MODULE parser->module()
#define UNIT parser->file()

%}

%union { Expression* expression; }
%union { Statement* statement; }
%union { String* string; }
%union { Feature* feature; }
%union { Formal* formal; }
%union { Type* type; }
%union { Generic* generic; }
%union { Variable* variable; }
%union { Block* block; }
%union { Feature::Flags flag; }

%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <expression>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <statement>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <string>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <feature>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <formal>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <type>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <generic>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <variable>

%pure-parser
%locations
%parse-param { Parser* parser }
%parse-param { void* scanner }
%lex-param { yyscan_t* scanner }

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
%token <expression> STRING FLOAT INTEGER BOOLEAN SBEGIN 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token IMPORT FUNCTION VOID
%token SEPARATOR SEMICOLON COLON BACKTICK
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
%type <statement> conditional when when_list statement statement_list
%type <block> block function_body
%type <variable> variable variable_list
%type <expression> call string expression expression_list


/* The Standard Apollo Grammar, version 2010 */
%%

module
    : class module { MODULE->feature($1); }
    | function module { MODULE->feature($1); }
    | IMPORT import_list SEPARATOR module {}
    | SEPARATOR class module { MODULE->feature($2); }
    | SEPARATOR function module { MODULE->feature($2); }
    | SEPARATOR IMPORT import_list SEPARATOR module {}
    | /* empty */ {}
    ;

class_name
    : TYPE '[' generic_list ']' { $$ = new Type(@$, $1, $3, UNIT, ENV); }
    | TYPE { $$ = new Type(@$, $1, 0, UNIT, ENV); }
    ;

class
    : class_name '<' type_list '{' comment member_list '}' {
        std::string qn = MODULE->name()->string();
        if (qn.empty()) {
            qn = $1->name()->string();
        } else {
            qn += "::" + $1->name()->string();
        }
        Type* type = new Type(@$, ID(qn), $1->generics(), UNIT, ENV);
        delete $1; // Delete the short type name after concat
        $$ = new Class(@$, type, $3, $5, $6);
    }
    ;

function
    : IDENTIFIER formal_signature modifiers return_signature function_body {
        $$ = new Function(@$, $1, $2, $3, $4, $5);
    }
    ;

function_body
    : block { $$ = $1; }
    | SEPARATOR { $$ = 0; }
    ;

import_list
    : scope ',' import_list { UNIT->feature(new Import(@$, $1, false)); }
    | scope { UNIT->feature(new Import(@$, $1, false)); }
    ;


member_list
    : member member_list { $$ = $1; $$->next($2); }
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
        $$ = new Attribute(@$, $1, 0, $3);
    }
    | IDENTIFIER type SEPARATOR {
        $$ = new Attribute(@$, $1, $2, new Empty(@$));
    }
    | error SEPARATOR {
        $$ = new Attribute(@$, ID(""), 0, new Empty(@$));
        yyerrok;
    }
    ;

method_name
    : IDENTIFIER { $$ = $1; }
    | OPERATOR { $$ = $1; }
    ;

method
    : method_name formal_signature modifiers return_signature function_body {
        Formal* self = new Formal(@$, ID("self"), ENV->self_type());
        self->next($2);
        $$ = new Function(@$, $1, self, $3, $4, $5);
    }
    | error block {
        Formal* self = new Formal(@$, ID("self"), ENV->self_type());
        $$ = new Function(@$, ID(""), self, 0, 0, $2);
        yyerrok;
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
    : PRIVATE { $$ = Feature::PRIVATE; }
    | PRIVATE NATIVE { $$ = Feature::PRIVATE | Feature::NATIVE; }
    | NATIVE { $$ = Feature::NATIVE; }
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
    : scope '[' generic_list ']' { $$ = new Type(@$, $1, $3, UNIT, ENV); }
    | scope { $$ = new Type(@$, $1, 0, UNIT, ENV); }
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
    | /* empty */ { $$ = 0; }
    ;

statement
    : LET variable_list block { $$ = new Let(@$, $2, $3); }
    | WHILE expression block { $$ = new While(@$, $2, $3); }
    | RETURN expression SEPARATOR { $$ = new Return(@$, $2); }
    | RETURN SEPARATOR { $$ = new Return(@$, new Empty(@$)); }
    | YIELD expression SEPARATOR { $$ = new Yield(@$, $2); }
    | FORK call SEPARATOR { $$ = new Fork(@$, $2); }
    | variable SEPARATOR { $$ = $1; }
    | expression SEPARATOR { $$ = new Simple(@$, $1); }
    | conditional { $$ = $1; }
    | FOR IDENTIFIER IN expression block { 
        //$$ = new For(@$, $2, $4, $5);

        // _i = $4.iterator()
        Dispatch* t1 = new Dispatch(@$, ID("iterator"), $4, 0);
        Variable* t2 = new Variable(@$, ID("_i"), 0, t1);
        
        // _i.more()
        Identifier* t3 = new Identifier(@$, ID("_i"));
        Dispatch* t4 = new Dispatch(@$, ID("more"), t3, 0); 

        // $2 = _i.value()
        Identifier* t5 = new Identifier(@$, ID("_i"));
        Dispatch* t6 = new Dispatch(@$, ID("value"), t5, 0); 
        Variable* t7 = new Variable(@$, $2, 0, t6); 

        // i.next()
        Identifier* t8 = new Identifier(@$, ID("_i"));
        Simple* t9 = new Simple(@$, new Dispatch(@$, ID("next"), t8, 0)); 
        
        t7->next(t9);
        t9->next($5);
         
        // Whole loop body
        While* t10 = new While(@$, t4, new Block(@$, 0, t7));
         
        $$ = new Let(@$, t2, t10);
    }
    | IDENTIFIER formal_signature return_signature block {
        $$ = new Simple(@$, new Empty(@$));
        $1 = 0;
        $2 = 0;
        $3 = 0;
        $4 = 0;
    }
    | CASE expression '{' comment when_list '}' {
        $$ = new Case(@$, $2, static_cast<When*>($5));
        delete $4; // '{' may introduce a comment, but we discard it
    }
    | error block {
        $$ = $2;
        yyerrok;
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
    | error SEPARATOR {
        $$ = new Simple(@$, new Empty(@$));
        yyerrok;
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
    | '-' expression %prec '*' { $$ = new Dispatch(@$, ID("@neg"), $2, 0); }
    | call { $$ = $1; }
    | '(' expression ')' { $$ = $2; } 
    | string { $$ = $1; }
    | INTEGER { $$ = $1; }
    | FLOAT { $$ = $1; }
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
        $$ = new Dispatch(@$, ID("@comp"), $1, $3);
    }
    | expression '>' expression { 
        Dispatch* less = new Dispatch(@$, ID("@less"), $1, $3); 
        Dispatch* equal = new Dispatch(@$, ID("@equal"), $1, $3);
        Binary* child = new Binary(@$, ID("or"), less, equal); 
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
        $$ = new Binary(@$, ID("or"), less, equal);  
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
        $$ = new Dispatch(@$, ID($3->string() + "?"), $1, 0);
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

string
    : SBEGIN expression string {
        Dispatch* string = new Dispatch(@$, ID("str?"), $2, 0);
        Dispatch* concat = new Dispatch(@$, ID("@add"), $1, string);
        $$ = new Dispatch(@$, ID("@add"), concat, $3);
    }
    | STRING {
        $$ = $1;
    }   
    ;

conditional
    : IF expression block ELSE block { $$ = new Conditional(@$, $2, $3, $5); }
    | IF expression block {
        $$ = new Conditional(@$, $2, $3, 0);
    }
    | IF expression block ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    ;

call
    : type '(' expression_list ')' { $$ = new Construct(@$, $1, $3); }
    | type '(' ')' { $$ = new Construct(@$, $1, 0); }
    | IDENTIFIER '(' expression_list ')' { $$ = new Call(@$, UNIT, 0, $1, $3); }
    | IDENTIFIER '(' ')' { $$ = new Call(@$, UNIT, 0, $1, 0); }
    | scope_list IDENTIFIER '(' ')' { $$ = new Call(@$, UNIT, $1, $2, 0); }
    | scope_list IDENTIFIER '(' expression_list ')' {
        $$ = new Call(@$, UNIT, $1, $2, $4);
    } 
    ; 

variable_list
    : variable ',' variable_list { $1->next($3); $$ = $1; }
    | variable { $$ = $1; }
    | expression { $$ = new Variable(@$, ID(""), 0, $1); }
    | expression ',' variable_list {
        $$ = new Variable(@$, ID(""), 0, $1);
        $$->next($3);
    }
    ;

variable
    : IDENTIFIER type { $$ = new Variable(@$, $1, $2, new Empty(@$)); }
    | IDENTIFIER type '=' expression { $$ = new Variable(@$, $1, $2, $4); }
    | IDENTIFIER '=' expression { $$ = new Variable(@$, $1, 0, $3); }
    ;

when_list
    : when when_list { $1->next($2); $$ = $1; }
    | when { $$ = $1; }
    ;

when
    : WHEN expression block { $$ = new When(@$, $2, $3); }
    ;
