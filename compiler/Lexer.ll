%{
#include "Statement.hpp"
#include "Formal.hpp"
#include "Expression.hpp"
#include "Feature.hpp"
#include "Parser.hpp"
#include "Grammar.hpp"
#include <iostream>

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
    yylloc->file_name = parser->file()->name();\
}
#define PUSH_SEPARATOR() yy_push_state(SC_SEPARATOR, yyscanner);

using namespace std;

%}

%option reentrant 
%option bison-bridge
%option never-interactive
%option noyywrap
%option yylineno
%option bison-locations
%option stack
%x SC_COMMENT SC_SEPARATOR SC_STRING1 SC_ESCAPE1
%x SC_STRING2 SC_ESCAPE2

/* Lexer rules */
%%
import return IMPORT;
case return CASE;
when return WHEN;
public return PUBLIC;
private return PRIVATE;
static return STATIC;
native return NATIVE;
weak return WEAK;
immutable return IMMUTABLE;
while return WHILE;
else return ELSE;
until return UNTIL;
if return IF;
func return FUNCTION;
for return FOR;
let return LET;
return return RETURN;
ret return RETURN;
xor return XOR;
and return AND;
or return OR;
in return IN;
not return NOT;
fork return FORK;
yield return YIELD;
Void return VOID;
-> return RIGHT_ARROW;
\<\- return LEFT_ARROW;
== return EQUAL; 
!= return NOT_EQUAL; 
\>\= return GREATER_OR_EQUAL;
\<\> return COMPARE;
\<\= return LESS_OR_EQUAL;
\<\< return LEFT_SHIFT;
\>\> return RIGHT_SHIFT;
\^\= return POWER_ASSIGN;
\*\= return MULTIPLY_ASSIGN;
\/\= return DIVIDE_ASSIGN;
\-\= return SUBTRACT_ASSIGN; 
\+\= return ADD_ASSIGN;
\%\= return MODULUS_ASSIGN;
\|\= return BIT_OR_ASSIGN;
xor\= return BIT_XOR_ASSIGN;
\&\= return BIT_AND_ASSIGN;
\+\+ return INCREMENT;
\-\- return DECREMENT;
\:\: return SCOPE;
; return SEMICOLON;
: return COLON;
` return BACKTICK;

[0-9]+\.[0-9]* {
    String* value = yyextra->environment()->floating(yytext);
    yylval->expression = new FloatLiteral(*yylloc, value);
    return FLOAT;
}
\.[0-9]+ {
    String* value = yyextra->environment()->floating(yytext);
    yylval->expression = new FloatLiteral(*yylloc, value);
    return FLOAT;
}
[0-9]+ {
	String* value = yyextra->environment()->integer(yytext);
	yylval->expression = new IntegerLiteral(*yylloc, value); 
	return INTEGER;
}
(true) {
    String* value = yyextra->environment()->integer("1");
    yylval->expression = new BooleanLiteral(*yylloc, value);
    return BOOLEAN;
}
(false) {
    String* value = yyextra->environment()->integer("0");
    yylval->expression = new BooleanLiteral(*yylloc, value);
    return BOOLEAN;
}
\" {
    yyextra->string_start(yylloc->first_column);
    yy_push_state(SC_STRING1, yyscanner);
}
\' {
    yyextra->string_start(yylloc->first_column);    
    yy_push_state(SC_STRING2, yyscanner);
}
@[a-z][a-z]* {
    yylval->string = yyextra->environment()->name(yytext);
    return OPERATOR;
}
[a-z][A-Za-z0-9_]*[?!=]? {
	yylval->string = yyextra->environment()->name(yytext);
	return IDENTIFIER;
}
[A-Z][A-Za-z0-9]* {
	yylval->string = yyextra->environment()->name(yytext);
	return TYPE;
}
\:[a-z][a-z0-9_]* {
    yylval->string = yyextra->environment()->name(yytext);
    return TYPEVAR;
}
[{] {
    yy_push_state(SC_COMMENT, yyscanner);
    return yytext[0];
}
[([,] return yytext[0]; 
[)\]] return yytext[0];
[}] {
    if (yy_top_state(yyscanner) == SC_STRING1) {
        yy_pop_state(yyscanner);
    } else {
        yy_push_state(SC_SEPARATOR, yyscanner);
        return yytext[0];
    }
}
[\n\r] {
    yyextra->column(1);
    yy_push_state(SC_SEPARATOR, yyscanner);
    return SEPARATOR;
}
#.*
[\t ]
. return yytext[0];

<SC_ESCAPE2>\' {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_STRING2);
}
<SC_ESCAPE2>. {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_STRING2);
}
<SC_STRING2>\\ {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_ESCAPE2);
}
<SC_STRING2>\' {
	String* value = yyextra->environment()->string(yyextra->string());
    yyextra->string("");
    yylloc->first_column = yyextra->string_start();
	yylval->expression = new StringLiteral(*yylloc, value); 
    yy_pop_state(yyscanner);
	return STRING;
}
<SC_STRING2>. {
    yyextra->string_char(yytext[0]);
}
<SC_ESCAPE1>\" {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_STRING1);
}
<SC_ESCAPE1># {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_STRING1);
}
<SC_ESCAPE1>. {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_STRING1);
}
<SC_STRING1>\\ {
    yyextra->string_char(yytext[0]);
    BEGIN(SC_ESCAPE1);
}
<SC_STRING1>#\{ {
    String* value = yyextra->environment()->string(yyextra->string());
    yyextra->string("");
    yylloc->first_column = yyextra->string_start();
    yylval->expression = new StringLiteral(*yylloc, value);
    yy_push_state(INITIAL, yyscanner);
    return SBEGIN;
}
<SC_STRING1>\" {
    String* value = yyextra->environment()->string(yyextra->string());
    yyextra->string("");
    yylloc->first_column = yyextra->string_start();
    yylval->expression = new StringLiteral(*yylloc, value);
    yy_pop_state(yyscanner);
    return STRING;
}
<SC_STRING1>. {
    yyextra->string_char(yytext[0]);
}
<SC_SEPARATOR>[\n\r] {
    yyextra->column(1); 
}
<SC_SEPARATOR>#.* 
<SC_SEPARATOR>[\t ]
<SC_SEPARATOR>. {
    yy_pop_state(yyscanner);
    yyunput(yytext[0], 0, yyscanner);
	yyextra->column(yyextra->column() - 1);
}
<SC_COMMENT>[\n\r] {
    yyextra->column(1);
}
<SC_COMMENT>#.* {
    const char* comment = yytext + 1;    
    while (isspace(*comment)) {
        comment++;
    }
    yylval->string = new String(comment);
    return COMMENT;
}
<SC_COMMENT>[^\t ] {
    yy_pop_state(yyscanner);
    yyunput(yytext[0], 0, yyscanner);
	yyextra->column(yyextra->column() - 1);
}
<SC_COMMENT>. 
%%

void Parser::force_separator() {
    struct yyguts_t *yyg = (struct yyguts_t *)scanner_;
    while (SC_SEPARATOR == yy_top_state(scanner_)) {
        //yy_push_state(SC_SEPARATOR, scanner_);
        yy_pop_state(scanner_);
    }
}
