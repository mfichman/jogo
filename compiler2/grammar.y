%{
#include "parser.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "formal.hpp"
#include "feature.hpp"
#include "unit.hpp"
#include "type.hpp"
#include <iostream>

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


%left '?'
%left BIT_AND_ASSIGN BIT_OR_ASSIGN BIT_XOR_ASSIGN
%left ADD_ASSIGN SUBTRACT_ASSIGN
%left '=' MULTIPLY_ASSIGN DIVIDE_ASSIGN MODULUS_ASSIGN 
%left OR
%left AND
%left '|' '^'
%left '&'
%left EQUAL NOT_EQUAL
%left '<' '>' LESS_OR_EQUAL GREATER_OR_EQUAL
%left LEFT_SHIFT RIGHT_SHIFT
%left '+' '-'
%left '*' '/' '%'
%left '!' '~'
%left INCREMENT DECREMENT
%left '.' '['

/* BISON declarations */
%token <name> IDENTIFIER TYPE
%token <expression> STRING NUMBER 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token CLASS INTERFACE STRUCT MODULE
%token IMPORT DEF VAR INIT DESTROY
%token SEPARATOR
%token WHEN CASE WHILE ELSE UNTIL IF DO FOR RETURN
%token RIGHT_ARROW LEFT_ARROW
%token EQUAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL
%token OR AND
%token LEFT_SHIFT RIGHT_SHIFT
%token MULTIPLY_ASSIGN DIVIDE_ASSIGN SUBTRACT_ASSIGN ADD_ASSIGN
%token MODULUS_ASSIGN BIT_OR_ASSIGN BIT_AND_ASSIGN BIT_XOR_ASSIGN
%token INCREMENT DECREMENT
%token SCOPE

%type <feature> feature feature_list attribute import define
%type <feature> constructor destructor function prototype native
%type <type> type
%type <generic> generic_list generic
%type <flag> modifiers
%type <formal> formal_signature formal_list formal
%type <statement> block when_list when statement_list statement
%type <expression> expression expression_list storage assignment


/* The Standard Apollo Grammar, version 2010 */
%%
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
    | MODULE type '{' feature_list '}' {
        parser->environment()->unit(new Module(@$, $2, $4));    
    }
	| /* empty is an error */ { 
		yyerror(&@$, parser, scanner, "Input file is empty"); 
		YYERROR;
	}
	| error { }
    ;

feature_list
    : feature feature_list { 
        $$ = $1;
        $$->next($2);
    }
    | feature {
        $$ = $1;
    }
    ;

feature
    : import { $$ = $1; }
    | define { $$ = $1; }
    | attribute { $$ = $1; }
    | constructor { $$ = $1; }
    | destructor { $$ = $1; }
    | function { $$ = $1; }
	| native { $$ = $1; }
	| prototype { $$ = $1; }
    ;

import
    : IMPORT type SEPARATOR { 
		$$ = new Import(@$, $2);
	}
    ;

define
    : DEF type TYPE SEPARATOR { 
		$$ = new Define(@$, $3, $2);
	}
    ;

attribute
    : VAR IDENTIFIER ':' type '=' expression SEPARATOR {
		// TODO: Set symbol table for class-level
		$$ = new Attribute(@$, $2, $4, $6);
    }
    | VAR IDENTIFIER ':' type SEPARATOR {
		$$ = new Attribute(@$, $2, $4, 0);
    }
	;

constructor
    : DEF INIT formal_signature block { 
        Name* name = parser->environment()->name("@init");
        $$ = new Function(@$, name, $3, 0, $4);
	}
    ;

destructor
    : DEF DESTROY '(' ')' block { 
        Name* name = parser->environment()->name("@destroy");
        $$ = new Function(@$, name, 0, 0, $5);
	}
    ;

function
    : DEF IDENTIFIER formal_signature ':' type modifiers block {
        $$ = new Function(@$, $2, $3, $5, $7);
    }
	| DEF IDENTIFIER formal_signature modifiers block {
        $$ = new Function(@$, $2, $3, 0, $5);
	}
	;

prototype
	: DEF IDENTIFIER formal_signature ':' type modifiers SEPARATOR {
        $$ = new Function(@$, $2, $3, $5, 0);
	}
	| DEF IDENTIFIER formal_signature modifiers SEPARATOR {
        $$ = new Function(@$, $2, $3, 0, 0);
	}
    ;

native
	: DEF IDENTIFIER formal_signature ':' type modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $2, $3, $5, 0);
	}
	| DEF IDENTIFIER formal_signature modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $2, $3, 0, 0);
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
	: PUBLIC { $$ = 0; }
	| PRIVATE { $$ = 0; }
	| STATIC { $$ = 0; }
	| PUBLIC STATIC { $$ = 0; }
	| PRIVATE STATIC { $$ = 0; }
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
        $$ = new Return(@$, 0); 
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
	: FOR IDENTIFIER ':' type LEFT_ARROW expression block {
		$$ = new For(@$, $2, $4, $6, $7);
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
    | VAR IDENTIFIER ':' type { 
		$$ = new Variable(@$, $2, $4, 0); 
	}
    | VAR IDENTIFIER ':' type '=' expression SEPARATOR { 
        $$ = new Variable(@$, $2, $4, $6); 
	}  
	| RETURN expression SEPARATOR { 
        $$ = new Return(@$, $2);
    }
	| RETURN SEPARATOR { 
        $$ = new Return(@$, 0);
    }
    | expression SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
    | assignment SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
	| IF expression block {
		$$ = new Conditional(@$, $2, $3, 0);
	}
    | IF expression block ELSE block {
        $$ = new Conditional(@$, $2, $3, $5);
    }
	;

storage
    : expression '.' IDENTIFIER { 
		/* Member variable access */
		$$ = new Member(@$, $1, $3); 
	}
    | expression '[' expression ']' { 
		$$ = new Index(@$, $1, $3);
	}

assignment
    : storage '=' expression { 
        $$ = new Assignment(@$, $1, $3); 
	}
    | storage MULTIPLY_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@multiply"), $1, $3));
	}
    | storage DIVIDE_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@divide"), $1, $3));
	}
    | storage MODULUS_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@modulus"), $1, $3));
	}
    | storage SUBTRACT_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@subtract"), $1, $3));
	}
    | storage ADD_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@add"), $1, $3));
	}
    | storage BIT_AND_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@bitwise_and"), $1, $3));
	}
    | storage BIT_OR_ASSIGN expression { 
        $$ = new Assignment(@$, $1, new Binary(@$, 
            parser->environment()->name("@bitwise_or"), $1, $3));
	}
    | storage BIT_XOR_ASSIGN expression {
        $$ = new Assignment(@$, $1, new Binary(@$,
            parser->environment()->name("@bitwise_xor"), $1, $3));
    }
    ;


expression
	: expression '?' expression { 
		$$ = new Binary(@$, parser->environment()->name("?"), $1, $3);
	}
    | expression OR expression { 
		$$ = new Binary(@$, parser->environment()->name("||"), $1, $3); 
	}
    | expression AND expression { 
		$$ = new Binary(@$, parser->environment()->name("&&"), $1, $3); 
	}
    | expression '|' expression { 
		$$ = new Binary(@$, 
            parser->environment()->name("@bitwise_or"), $1, $3); 
	}
    | expression '^' expression { 
		$$ = new Binary(@$, 
            parser->environment()->name("@bitwise_xor"), $1, $3); 
	}
    | expression '&' expression { 
		$$ = new Binary(@$, 
            parser->environment()->name("@bitwise_and"), $1, $3); 
	}
    | expression EQUAL expression { 
		$$ = new Binary(@$, parser->environment()->name("@equal"), $1, $3); 
	}
    | expression NOT_EQUAL expression { 
        $$ = new Binary(@$, parser->environment()->name("@not_equal"), $1, $3);
	}
    | expression '>' expression { 
		$$ = new Binary(@$, parser->environment()->name("@greater"), $1, $3); 
	}
    | expression '<' expression { 
		$$ = new Binary(@$, parser->environment()->name("@less"), $1, $3); 
	}
    | expression GREATER_OR_EQUAL expression { 
		$$ = new Binary(@$, 
            parser->environment()->name("@greater_or_equal"), $1, $3); 
	}
    | expression LESS_OR_EQUAL expression { 
		$$ = new Binary(@$, 
            parser->environment()->name("@less_or_equal"), $1, $3); 
	}
    | expression LEFT_SHIFT expression { 
		$$ = new Binary(@$, parser->environment()->name("@shift"), $1, $3); 
	}
    | expression RIGHT_SHIFT expression { 
		$$ = new Binary(@$, parser->environment()->name("@unshift"), $1, $3); 
	}
    | expression '+' expression { 
		$$ = new Binary(@$, parser->environment()->name("@add"), $1, $3); 
	}
    | expression '-' expression { 
		$$ = new Binary(@$, parser->environment()->name("@subtract"), $1, $3); 
	}
    | expression '*' expression { 
		$$ = new Binary(@$, parser->environment()->name("@multiply"), $1, $3); 
	}
    | expression '/' expression { 
		$$ = new Binary(@$, parser->environment()->name("@divide"), $1, $3); 
	}
    | expression '%' expression { 
		$$ = new Binary(@$, parser->environment()->name("@modulus"), $1, $3); 
	}
    | '!' expression { 
        $$ = new Unary(@$, parser->environment()->name("!"), $2); 
    }
    | '~' expression { 
        $$ = new Unary(@$, parser->environment()->name("~"), $2); 
    }
    | expression INCREMENT { 
		$$ = new Unary(@$, parser->environment()->name("@increment"), $1); 
	}
    | expression DECREMENT { 
		$$ = new Unary(@$, parser->environment()->name("@decrement"), $1); 
	}
    | IDENTIFIER '(' expression_list ')' {
		$$ = new Call(@$, $1, $3);
    }
	| IDENTIFIER '(' ')' {
		/* Call on an object expression */
		$$ = new Call(@$, $1, 0);
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
	| IDENTIFIER { $$ = new Identifier(@$, $1); }
    ;

expression_list
	: expression ',' expression_list { 
		$$ = $1;
        $$->next($3);
	}
	| expression { $$ = $1; } 
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


