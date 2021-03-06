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
    current.file = parser->file();\
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
%union { Assignment* assignment; }
%union { Block* block; }
%union { Feature::Flags flag; }

%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <expression>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <statement>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <string>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <feature>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <formal>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <type>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <generic>
%destructor { if ($$ && !$$->refcount()) delete $$; $$ = 0; } <assignment>

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
%token <string> IDENTIFIER TYPE OPERATOR COMMENT TYPEVAR
%token <expression> STRING LITERAL SBEGIN 
%token <flag> PUBLIC PRIVATE STATIC NATIVE WEAK IMMUTABLE
%token IMPORT FUNCTION VOID
%token SEPARATOR SEMICOLON BACKTICK
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
%type <type> type type_list return_sig class_name
%type <string> scope scope_prefix comment method_name 
%type <generic> generic generic_list
%type <flag> method_flags function_flags attribute_flags
%type <formal> formal_sig formal formal_list
%type <statement> conditional when when_list statement statement_list
%type <block> block function_body
%type <assignment> assignment assignment_list
%type <expression> closure_formal closure operation pair pair_list item_list 
%type <expression> call string expression expression_list


/* The Standard Apollo Grammar, version 2010 */
%%

module
    : class module { MODULE->feature($1); }
    | function module { MODULE->feature($1); }
    | IMPORT import SEPARATOR module {}
    | SEPARATOR class module { MODULE->feature($2); }
    | SEPARATOR function module { MODULE->feature($2); }
    | SEPARATOR IMPORT import SEPARATOR module {}
    | /* empty */ {}
    ;

class_name
    : TYPE '[' generic_list ']' { $$ = new Type(@$, $1, $3, UNIT, ENV); }
    | TYPE { $$ = new Type(@$, $1, 0, UNIT, ENV); }
    ;

class
    : class_name '<' type_list '{' comment member_list '}' SEPARATOR {
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

function_body
    : block SEPARATOR { $$ = $1; }
    | SEPARATOR { $$ = 0; }
    ;

function_flags
    : NATIVE { $$= Feature::NATIVE; }
    | /* empty */ { $$ = 0; }
    ;

function
    : IDENTIFIER formal_sig function_flags return_sig function_body {
        $$ = new Function(@$, $1, $2, $3, $4, $5);
    }
    ;

import
    : scope ',' import { UNIT->feature(new Import(@$, $1, false)); }
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

attribute_flags
    : IMMUTABLE { $$ = Feature::IMMUTABLE; }
    | WEAK IMMUTABLE { $$ = Feature::WEAK | Feature::IMMUTABLE; }
    | WEAK { $$ = Feature::WEAK; }
    | PRIVATE WEAK { $$ = Feature::WEAK | Feature::PRIVATE; }
    | PRIVATE { $$ = Feature::PRIVATE; }
    | /* empty */ { $$ = 0; }
    ;

attribute
    : IDENTIFIER attribute_flags type '=' expression SEPARATOR {
        $$ = new Attribute(@$, $1, $2, $3, $5);
    }
    | IDENTIFIER attribute_flags '=' expression SEPARATOR {
        $$ = new Attribute(@$, $1, $2, 0, $4);
    }
    | IDENTIFIER attribute_flags type SEPARATOR {
        $$ = new Attribute(@$, $1, $2, $3, new Empty(@$));
    }
    | error SEPARATOR {
        $$ = new Attribute(@$, ID(""), 0, 0, new Empty(@$));
        yyerrok;
    }
    ;

method_name
    : IDENTIFIER { $$ = $1; }
    | OPERATOR { $$ = $1; }
    ;

method_flags
    : PRIVATE { $$ = Feature::PRIVATE; }
    | PRIVATE NATIVE { $$ = Feature::PRIVATE | Feature::NATIVE; }
    | NATIVE { $$ = Feature::NATIVE; }
    | /* empty */ { $$ = 0; }
    ;

method
    : method_name formal_sig method_flags return_sig function_body {
        if ($1->string() != "@init") {
            Formal* self = new Formal(@$, ID("self"), ENV->self_type());
            self->next($2);
            $$ = new Function(@$, $1, self, $3, $4, $5);
        } else {
            $$ = new Function(@$, $1, $2, $3, $4, $5);
        }
    }
    | error block SEPARATOR {
        Formal* self = new Formal(@$, ID("self"), ENV->self_type());
        $$ = new Function(@$, ID(""), self, 0, 0, $2);
        yyerrok;
    }
    ;

return_sig
    : /* empty */ { $$ = ENV->void_type(); }
    | type { $$ = $1; }
    ;

formal_sig
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

scope_prefix
    : TYPE SCOPE scope_prefix { $$ = ID($1->string() + "::" + $3->string()); }
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
    | TYPEVAR { $$ = new Type(@$, $1, 0, UNIT, ENV); }
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
    : LET assignment_list block SEPARATOR { $$ = new Let(@$, $2, $3); } 
    | WHILE expression block SEPARATOR { $$ = new While(@$, $2, $3); }
    | RETURN expression SEPARATOR { $$ = new Return(@$, $2); }
    | RETURN SEPARATOR { $$ = new Return(@$, new Empty(@$)); }
    | YIELD SEPARATOR { $$ = new Yield(@$, 0); }
    | FORK call SEPARATOR { $$ = new Fork(@$, $2); }
    | operation SEPARATOR { $$ = new Simple(@$, $1); }
    | conditional { $$ = $1; }
    | IDENTIFIER type SEPARATOR { 
        $$ = new Simple(@$, new Assignment(@$, $1, $2, new Empty(@$)));
    }
    | FOR IDENTIFIER IN expression block SEPARATOR { 
        //$$ = new For(@$, $2, $4, $5);

        // _i = $4.iter()
        Dispatch* t1 = new Dispatch(@$, ID("iter"), $4, 0);
        Assignment* t2 = new Assignment(@$, ID("_i"), 0, t1);
        
        // while (_i.more())
        Identifier* t3 = new Identifier(@$, ID("_i"));
        Dispatch* t4 = new Dispatch(@$, ID("more?"), t3, 0); 

        // $2 = _i.next()
        Identifier* t5 = new Identifier(@$, ID("_i"));
        Dispatch* t6 = new Dispatch(@$, ID("next"), t5, 0); 
        Simple* t7 = new Simple(@$, new Assignment(@$, $2, 0, t6)); 
        
        t7->next($5);
         
        // Whole loop body
        While* t10 = new While(@$, t4, new Block(@$, 0, t7));
         
        $$ = new Let(@$, t2, t10);
    }
    | IDENTIFIER formal_sig return_sig block SEPARATOR {
        // Need a maybe-assign type (e.g., Assignment, Declaration) 
        $$ = new Simple(@$, new Empty(@$));
        $1 = 0;
        $2 = 0;
        $3 = 0;
        $4 = 0;
    }
    | CASE expression '{' comment when_list '}' SEPARATOR {
        $$ = new Case(@$, $2, static_cast<When*>($5));
        delete $4; // '{' may introduce a comment, but we discard it
    }
    | error block SEPARATOR {
        $$ = $2;
        yyerrok;
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
    : IDENTIFIER { $$ = new Identifier(@$, $1); }
    | LITERAL { $$ = $1; }
    | '{' pair_list '}' { $$ = new HashLiteral(@$, $2); }
    | '[' item_list ']' { $$ = new ArrayLiteral(@$, $2); }
    | operation { $$ = $1; } 
    | string { $$ = $1; }
    | closure { $$ = $1; }
    ;

closure_formal
    : /* empty */ { $$ = 0; }
    | closure { $$ = $1; }
    ;

closure
    : FUNCTION formal_sig return_sig block {
        // Anonymous function!
        std::string qn = MODULE->name()->string();
        if (qn.empty()) {
            qn = "_Closure" + stringify($4);
        } else {
            qn += "::_Closure" + stringify($4);
        }
        Formal* self = new Formal(@$, ID("self"), ENV->self_type());
        self->next($2);
        Function* func = new Function(@$, ID("@call"), self, 0, $3, $4);
        Type* type = new Type(@$, ID(qn), 0, UNIT, ENV);
        Type* obj = new Type(@$, ID("Object"), 0, UNIT, ENV);
        Class* clazz = new Class(@$, type, obj, 0, func);
        MODULE->feature(clazz); 
        $$ = new Closure(@$, func, clazz);
    }
    ;

item_list
    : expression ',' item_list { $1->next($3); $$ = $1; }
    | expression SEPARATOR item_list { $1->next($3); $$ = $1; }
    | expression { $$ = $1; }
    | /* empty */ { $$ = 0; }
    ;

pair_list
    : pair ',' pair_list { $1->next($3); $$ = $1; }
    | pair SEPARATOR pair_list { $1->next($3); $$ = $1; }
    | pair { $$ = $1; }
    | /* empty */ { $$ = 0; }
    ;

pair
    : string ':' expression { 
        $1->next($3);
        $$ = new Construct(@$, ENV->pair_type(), $1);
    }
    | IDENTIFIER ':' expression {
        StringLiteral* string = new StringLiteral(@$, $1); 
        string->next($3);
        $$ = new Construct(@$, ENV->pair_type(), string);
    }
    ;

operation
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
    | expression '.' IDENTIFIER '(' expression_list ')' closure_formal {
        $5->last($7);
        $$ = new Dispatch(@$, $3, $1, $5);
    }
    | expression '.' IDENTIFIER '(' ')' closure_formal {
        $$ = new Dispatch(@$, $3, $1, $6);
    }
    | expression '.' IDENTIFIER closure {
        $$ = new Dispatch(@$, $3, $1, $4);
    }
    | '-' expression %prec '*' { 
        if (IntegerLiteral* lit = dynamic_cast<IntegerLiteral*>($2)) {
            String* val = ENV->integer("-" + lit->value()->string());
            $$ = new IntegerLiteral(@$, val);
            delete $2;
        } else if (FloatLiteral* lit = dynamic_cast<FloatLiteral*>($2)) {
            String* val = ENV->floating("-" + lit->value()->string());
            $$ = new FloatLiteral(@$, val);
            delete $2;
        } else {
            $$ = new Dispatch(@$, ID("@neg"), $2, 0); 
        }
    }
    | expression '.' IDENTIFIER '=' expression {
        /* Attribute assignment, calls the mutator function */
        $$ = new Dispatch(@$, ID($3->string() + "="), $1, $5);
    }
    | expression '[' expression_list ']' '=' expression {
        /* Indexed assignment operator */
        $3->last($6);

        /* Invokes the special @insert method, a la Lua and Python */
        $$ = new Dispatch(@$, ID("@insert"), $1, $3);
    }
    | INCREMENT IDENTIFIER {
        Expression* t1 = new IntegerLiteral(@$, ID("1"));
        Expression* t2 = new Identifier(@$, $2);
        Expression* t3 = new Dispatch(@$, ID("@add"), t2, t1);
        $$ = new Assignment(@$, $2, 0, t3);  
    }
    | DECREMENT IDENTIFIER {
        Expression* t1 = new IntegerLiteral(@$, ID("1"));
        Expression* t2 = new Identifier(@$, $2);
        Expression* t3 = new Dispatch(@$, ID("@sub"), t2, t1);
        $$ = new Assignment(@$, $2, 0, t3);  
    }
    | assignment { $$ = $1; }
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
    : IF expression block ELSE block SEPARATOR { 
        $$ = new Conditional(@$, $2, $3, $5); 
    }
    | IF expression block SEPARATOR ELSE block SEPARATOR { 
        $$ = new Conditional(@$, $2, $3, $6); 
    }
    | IF expression block SEPARATOR {
        $$ = new Conditional(@$, $2, $3, 0);
    }
    | IF expression block ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    | IF expression block SEPARATOR ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $6);
    }
    ;

call
    : type '(' expression_list ')' closure_formal  { 
        $3->last($5);
        $$ = new Construct(@$, $1, $3);
    }
    | type '(' ')' closure_formal { 
        $$ = new Construct(@$, $1, $4);
    }
    | type closure { 
        $$ = new Construct(@$, $1, $2);
    }
    | IDENTIFIER '(' expression_list ')' closure_formal {
        $3->last($5);
        $$ = new Call(@$, UNIT, 0, $1, $3); 
    }
    | IDENTIFIER '(' ')' closure_formal {
        $$ = new Call(@$, UNIT, 0, $1, $4); 
    }
    | IDENTIFIER closure {
        $$ = new Call(@$, UNIT, 0, $1, $2); 
    }
    | scope_prefix IDENTIFIER '(' expression_list ')' closure_formal {
        $4->last($6);
        UNIT->feature(new Import(@$, $1, true));
        $$ = new Call(@$, UNIT, $1, $2, $4);
    } 
    | scope_prefix IDENTIFIER '(' ')' closure_formal { 
        UNIT->feature(new Import(@$, $1, true));
        $$ = new Call(@$, UNIT, $1, $2, $5); 
    }
    | scope_prefix IDENTIFIER closure { 
        UNIT->feature(new Import(@$, $1, true));
        $$ = new Call(@$, UNIT, $1, $2, $3); 
    }
    ; 

assignment_list
    : assignment ',' assignment_list { $1->next($3); $$ = $1; }
    | assignment { $$ = $1; }
    ;

assignment
    : IDENTIFIER type '=' expression { $$ = new Assignment(@$, $1, $2, $4); }
    | IDENTIFIER '=' expression { $$ = new Assignment(@$, $1, 0, $3); }
    ;

when_list
    : when when_list { $1->next($2); $$ = $1; }
    | when { $$ = $1; }
    ;

when
    : WHEN expression block { $$ = new When(@$, $2, $3); }
    ;
