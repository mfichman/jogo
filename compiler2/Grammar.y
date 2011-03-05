%{
#include "Parser.hpp"
#include "Expression.hpp"
#include "Statement.hpp"
#include "Formal.hpp"
#include "Feature.hpp"
#include "Unit.hpp"
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
%union { Name* identifier; }
%union { Unit* unit; }
%union { Feature* feature; }
%union { Formal* formal; }
%union { Type* type; }
%union { Generic* generic; }
%union { Variable* variable; }
%union { int null; }
%union { int flag; }

%pure_parser
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
%left '<' '>' LESS_OR_EQUAL GREATER_OR_EQUAL
%left LEFT_SHIFT RIGHT_SHIFT
%left '+' '-'
%left '*' '/' '%'
%left '^'
%left '!' '~'
%left INCREMENT DECREMENT
%left '.' '['

/* BISON declarations */
%token <name> IDENTIFIER TYPE
%token <expression> STRING NUMBER BOOLEAN 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token CLASS INTERFACE STRUCT MODULE
%token IMPORT INIT DESTROY
%token SEPARATOR
%token WHEN CASE WHILE ELSE UNTIL IF DO FOR RETURN
%token RIGHT_ARROW LEFT_ARROW
%token EQUAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL
%token OR AND
%token LEFT_SHIFT RIGHT_SHIFT
%token MULTIPLY_ASSIGN DIVIDE_ASSIGN SUBTRACT_ASSIGN ADD_ASSIGN
%token MODULUS_ASSIGN BIT_OR_ASSIGN BIT_AND_ASSIGN BIT_XOR_ASSIGN
%token INCREMENT DECREMENT
%token SCOPE LET IN

%type <feature> feature feature_list attribute
%type <feature> constructor destructor function prototype native
%type <type> type
%type <generic> generic_list generic
%type <flag> modifiers
%type <formal> formal_signature formal_list formal
%type <statement> block when_list when statement_list statement conditional
%type <variable> variable_list variable
%type <expression> expression expression_list storage assignment


/* The Standard Apollo Grammar, version 2010 */
%%

module
    : unit_list {
    }
	| /* empty is an error */ { 
		yyerror(&@$, parser, scanner, "Input file is empty"); 
		YYERROR;
	}
    ;

unit_list
    : unit unit_list {
    }
    | unit {
    }
    ;

unit
    : CLASS type '{' feature_list '}' {
        parser->environment()->unit(new Class(@$, $2, $4)); 
    }
    | INTERFACE type '{' feature_list '{' {
        parser->environment()->unit(new Interface(@$, $2, $4)); 
    }
    | STRUCT type '{' feature_list '}' {
        parser->environment()->unit(new Structure(@$, $2, $4));    
    }
    | IMPORT type SEPARATOR { 
		//$$ = new Import(@$, $2);
        $2 = 0;
        assert("Not supported");
	}
    | function {
        $1 = 0;
        assert("Not supported");
    }
	| error { }
    ;

feature_list
    : feature feature_list { 
        $$ = $1;
        $$->next($2);
    }
    | /* empty */ {
        $$ = 0;
    }
    ;

feature
    : attribute { $$ = $1; }
    | constructor { $$ = $1; }
    | destructor { $$ = $1; }
    | function { $$ = $1; }
	| native { $$ = $1; }
	| prototype { $$ = $1; }
    ;

attribute
    : IDENTIFIER ':' type '=' expression SEPARATOR {
		// TODO: Set symbol table for class-level
		$$ = new Attribute(@$, $1, $3, $5);
    }
    | IDENTIFIER ':' type SEPARATOR {
		$$ = new Attribute(@$, $1, $3, new Empty(@$));
    }
	;

constructor
    : INIT formal_signature block { 
        Name* name = parser->environment()->name("@init");
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, name, $2, type, $3);
	}
    ;

destructor
    : DESTROY '(' ')' block { 
        Name* name = parser->environment()->name("@destroy");
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, name, 0, type, $4);
	}
    ;

function
    : IDENTIFIER formal_signature ':' type modifiers block {
        $$ = new Function(@$, $1, $2, $4, $6);
    }
	| IDENTIFIER formal_signature modifiers block {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, $4);
	}
	;

prototype
	: IDENTIFIER formal_signature ':' type modifiers SEPARATOR {
        $$ = new Function(@$, $1, $2, $4, 0);
	}
	| IDENTIFIER formal_signature modifiers SEPARATOR {
        Type* type = parser->environment()->void_type();
        $$ = new Function(@$, $1, $2, type, 0);
	}
    ;

native
	: IDENTIFIER formal_signature ':' type modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $1, $2, $4, 0);
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
    : IDENTIFIER ':' type {
        $$ = new Formal(@$, $1, $3);
    }

modifiers
	: PRIVATE { $$ = 0; }
	| /* empty */ { $$ = 0; }
	;

type
    : type SCOPE TYPE { 
        $$ = new Type($1, $3, 0, parser->environment()); 
    }
    | type SCOPE TYPE '[' generic_list ']' {
        $$ = new Type($1, $3, $5, parser->environment());
    }
    | TYPE {
        $$ = new Type(0, $1, 0, parser->environment()); 
    }
    | TYPE '[' generic_list ']' {
        $$ = new Type(0, $1, $3, parser->environment());
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
	| RIGHT_ARROW RETURN expression SEPARATOR { 
        $$ = new Return(@$, $3); 
    }
	| RIGHT_ARROW RETURN SEPARATOR { 
        $$ = new Return(@$, new Empty(@$)); 
    }
	| RIGHT_ARROW expression SEPARATOR { 
		$$ = new Simple(@$, $2); 
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
	: FOR IDENTIFIER ':' type IN expression block {
		$$ = new For(@$, $2, $4, $6, $7);
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
    | variable SEPARATOR {
        $$ = $1;
    }
    | expression SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
    | assignment SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
    | conditional {
        $$ = $1;
    }
    ;

conditional
	: IF expression block {
		$$ = new Conditional(@$, $2, $3, 0);
	}
    | IF expression block ELSE block {
        $$ = new Conditional(@$, $2, $3, $5);
    }
    | IF expression block ELSE conditional {
        $$ = new Conditional(@$, $2, $3, $5);
    }
	;

storage
    : expression '.' IDENTIFIER { 
		/* Member variable access */
		$$ = new Member(@$, $1, $3); 
	}
    | expression '[' expression ']' { 
        $1->next($3);
        Environment* env = parser->environment();
        $$ = new Dispatch(@$, env->name("@index"), $1);
	}

assignment
    : storage '=' expression { 
        $$ = new Assignment(@$, $1, $3); 
	}
    | storage POWER_ASSIGN expression {
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* power = new Dispatch(@$, env->name("@power"), $1);
        $$ = new Assignment(@$, $1, power);
    }
    | storage MULTIPLY_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* multiply = new Dispatch(@$, env->name("@multiply"), $1);
        $$ = new Assignment(@$, $1, multiply);
	}
    | storage DIVIDE_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* divide = new Dispatch(@$, env->name("@divide"), $1);
        $$ = new Assignment(@$, $1, divide);
	}
    | storage MODULUS_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* modulus = new Dispatch(@$, env->name("@modulus"), $1);
        $$ = new Assignment(@$, $1, modulus);
	}
    | storage SUBTRACT_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* subtract = new Dispatch(@$, env->name("@subtract"), $1);
        $$ = new Assignment(@$, $1, subtract);
	}
    | storage ADD_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* add = new Dispatch(@$, env->name("@add"), $1);
        $$ = new Assignment(@$, $1, add);
	}
    | storage BIT_AND_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* child = new Dispatch(@$, env->name("@bitand"), $1);
        $$ = new Assignment(@$, $1, child);
	}
    | storage BIT_OR_ASSIGN expression { 
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* child = new Dispatch(@$, env->name("@bitor"), $1);
        $$ = new Assignment(@$, $1, child);
	}
    | storage BIT_XOR_ASSIGN expression {
        $1->next($3);
        Environment* env = parser->environment();
        Dispatch* child = new Dispatch(@$, env->name("@bitxor"), $1);
        $$ = new Assignment(@$, $1, child);
    }
    ;


expression
	: expression '?' expression { 
		$$ = new Binary(@$, parser->environment()->name("?"), $1, $3);
        assert("Not implemented");
	}
    | expression OR expression { 
		$$ = new Binary(@$, parser->environment()->name("||"), $1, $3); 
	}
    | expression AND expression { 
		$$ = new Binary(@$, parser->environment()->name("&&"), $1, $3); 
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
    | '!' expression { 
        $$ = new Unary(@$, parser->environment()->name("!"), $2); 
    }
    | '~' expression { 
        $$ = new Dispatch(@$, parser->environment()->name("@complement"), $2); 
    }
    | expression INCREMENT { 
		$$ = new Dispatch(@$, parser->environment()->name("@increment"), $1); 
	}
    | expression DECREMENT { 
		$$ = new Dispatch(@$, parser->environment()->name("@decrement"), $1); 
	}
    | IDENTIFIER '(' expression_list ')' {
		$$ = new Call(@$, 0, $1, $3);
    }
	| IDENTIFIER '(' ')' {
		/* Call on an object expression */
		$$ = new Call(@$, 0, $1, 0);
	}
    | type SCOPE IDENTIFIER '(' expression_list ')' {
        $$ = new Call(@$, $1, $3, $5);
    } 
    | type SCOPE IDENTIFIER '(' ')' {
        $$ = new Call(@$, $1, $3, 0);
    }
    | expression '.' IDENTIFIER '(' expression_list ')' {
        $1->next($5);
        $$ = new Dispatch(@$, $3, $1);
    }
    | expression '.' IDENTIFIER '(' ')' {
        $$ = new Dispatch(@$, $3, $1);
    }
    | storage { $$ = $1; }
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
    | variable {
        $$ = $1;
    }
    ;

variable
    : IDENTIFIER ':' type {
        $$ = new Variable(@$, $1, $3, new Empty(@$));
    }
    | IDENTIFIER ':' type '=' expression {
        $$ = new Variable(@$, $1, $3, $5);
    }
    ;

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
    : WHEN IDENTIFIER ':' type block {
        $$ = new When(@$, $2, $4, $5);
    }
    ;


