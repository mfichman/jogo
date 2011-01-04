%{
#include "statement.hpp"
#include "formal.hpp"
#include "expression.hpp"
#include "unit.hpp"
#include "feature.hpp"
#include "parser.hpp"
#include "grammar.hpp"

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
#define YY_INPUT(buf, res, len) { \
	Parser *parser = yyget_extra(yyscanner);\
	parser->input().read(buf, len);\
	res = (parser->input().bad()) ? 0 : parser->input().gcount();\
}
#define YY_USER_ACTION {\
	Parser *parser = yyget_extra(yyscanner);\
	yylloc->first_line = yylineno;\
	yylloc->first_column = parser->column();\
	parser->column(parser->column() + yyleng);\
	yylloc->last_column = parser->column() - 1;\
	yylloc->last_line = yylineno;\
}

%}

%option reentrant 
%option bison-bridge
%option never-interactive
%option noyywrap
%option nounput
%option yylineno
%option bison-locations
%s SEPARATOR

/* Lexer rules */
%%
class BEGIN(INITIAL); return CLASS;
interface BEGIN(INITIAL); return INTERFACE;
struct BEGIN(INITIAL); return STRUCT;
module BEGIN(INITIAL); return MODULE;
import BEGIN(INITIAL); return IMPORT;
def	BEGIN(INITIAL); return DEF;
var BEGIN(INITIAL); return VAR; 
case BEGIN(INITIAL); return CASE;
when BEGIN(INITIAL); return WHEN;
\@init BEGIN(INITIAL); return INIT;
\@destroy BEGIN(INITIAL); return DESTROY;
public BEGIN(INITIAL); return PUBLIC;
private BEGIN(INITIAL); return PRIVATE;
static BEGIN(INITIAL); return STATIC;
native BEGIN(INITIAL); return NATIVE;
while BEGIN(INITIAL); return WHILE;
else BEGIN(INITIAL); return ELSE;
until BEGIN(INITIAL); return UNTIL;
if BEGIN(INITIAL); return IF;
for BEGIN(INITIAL); return FOR;
return BEGIN(SEPARATOR); return RETURN;
-> BEGIN(INITIAL); return RIGHT_ARROW;
\<\- BEGIN(INITIAL); return LEFT_ARROW;
== BEGIN(INITIAL); return EQUAL; 
!= BEGIN(INITIAL); return NOT_EQUAL; 
\|\| BEGIN(INITIAL); return OR; 
\&\& BEGIN(INITIAL); return AND; 
\>\= BEGIN(INITIAL); return GREATER_OR_EQUAL;
\<\= BEGIN(INITIAL); return LESS_OR_EQUAL;
\<\< BEGIN(INITIAL); return LEFT_SHIFT;
\>\> BEGIN(INITIAL); return RIGHT_SHIFT;
\*\= BEGIN(INITIAL); return MULTIPLY_ASSIGN;
\/\= BEGIN(INITIAL); return DIVIDE_ASSIGN;
\-\= BEGIN(INITIAL); return SUBTRACT_ASSIGN; 
\+\= BEGIN(INITIAL); return ADD_ASSIGN;
\%\= BEGIN(INITIAL); return MODULUS_ASSIGN;
\|\= BEGIN(INITIAL); return BIT_OR_ASSIGN;
\&\= BEGIN(INITIAL); return BIT_AND_ASSIGN;
\+\+ BEGIN(INITIAL); return INCREMENT;
\-\- BEGIN(INITIAL); return DECREMENT;
\:\: BEGIN(INITIAL); return SCOPE;
[0-9]+ {
    Name* type = yyextra->environment()->name("Int");
	Name* value = yyextra->environment()->name(yytext);
	yylval->expression = new IntegerLiteral(*yylloc, type, value); 
    BEGIN(SEPARATOR);
	return NUMBER;
}
\"[^"]*\" {
	yytext[strlen(yytext)-1] = 0;
    Name* type = yyextra->environment()->name("String");
	Name* value = yyextra->environment()->name(yytext);
	yylval->expression = new StringLiteral(*yylloc, type, value);
    BEGIN(SEPARATOR);
	return STRING;
}
\'[^']*\' {
    Name* type = yyextra->environment()->name("String");
	Name* value = yyextra->environment()->name(yytext);
	yylval->expression = new StringLiteral(*yylloc, type, value); 
    BEGIN(SEPARATOR);
	return STRING;
}
[a-z][A-Za-z0-9_][?!]? {
	yylval->name = yyextra->environment()->name(yytext);
    BEGIN(SEPARATOR);
	return IDENTIFIER;
}
[A-Z][A-Za-z0-9_]* {
	yylval->name = yyextra->environment()->name(yytext);
    BEGIN(SEPARATOR);
	return TYPE;
}

#.*
[{([,] BEGIN(INITIAL); return yytext[0];
[)\]] BEGIN(SEPARATOR); return yytext[0];
<SEPARATOR>[\n\r] BEGIN(INITIAL); yyget_extra(yyscanner)->column(1); return SEPARATOR;
<INITIAL>[\n\r] yyget_extra(yyscanner)->column(1);
[\t ]
. return yytext[0];
%%


//  struct yyguts_t *yyg = (struct yyguts_t *)self->scanner;
//	yylineno = 1;
