%{
#include <apparser.h>
#include <apexpr.h>
#include <aptype.h>
#include <apgrammar.h>

#define YYSTYPE apnode_t
#define YYLTYPE aploc_t
#define YY_EXTRA_TYPE apparser_t *
#define YY_NO_INPUT
#define YYERROR_VERBOSE
#define YY_INPUT(buf, res, len) { \
	res = apparser_read(yyget_extra(yyscanner), buf, len);\
	res = (res == -1) ? 0 : res;\
}
#define YY_USER_ACTION {\
	apparser_t *parser = yyget_extra(yyscanner);\
	yylloc->first_line = yylineno;\
	yylloc->first_column = parser->column;\
	parser->column += yyleng;\
	yylloc->last_column = parser->column - 1;\
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

/* Lexer rules */
%%
class return TOK_CLASS;
interface return TOK_INTERFACE;
struct return TOK_STRUCT;
module return TOK_MODULE;
import return TOK_IMPORT;
def	return TOK_DEF;
\@init return TOK_INIT;
\@destroy return TOK_DESTROY;
public return TOK_PUBLIC;
private return TOK_PRIVATE;
protected return TOK_PROTECTED;
static return TOK_STATIC;
native return TOK_NATIVE;
while return TOK_WHILE;
else return TOK_ELSE;
until return TOK_UNTIL;
if return TOK_IF;
do return TOK_DO;
for return TOK_FOR;
return return TOK_RETURN;
== return TOK_EQUAL; 
!= return TOK_NOTEQUAL; 
:: return TOK_SCOPE; 
\|\| return TOK_OR; 
\&\& return TOK_AND; 
\>\= return TOK_GE;
\<\= return TOK_LE;
\<\< return TOK_LSHIFT;
\>\> return TOK_RSHIFT;
\*\= return TOK_MUL_ASSIGN;
\/\= return TOK_DIV_ASSIGN;
\-\= return TOK_SUB_ASSIGN; 
\+\= return TOK_ADD_ASSIGN;
\%\= return TOK_MOD_ASSIGN;
\|\= return TOK_BITOR_ASSIGN;
\&\= return TOK_BITAND_ASSIGN;
\+\+ return TOK_INC;
\-\- return TOK_DEC;
int yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
uint yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE; 
short yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
ushort yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
byte yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
ubyte yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
long yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
ulong yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
bool yylval->type = aptype_primitive(strdup(yytext)); return TOK_PRIMITIVE;
[0-9]+ {
	yylval->expr = apexpr_literal(yylloc, aptype_primitive(strdup("int")), 
		strdup(yytext)); 
	return TOK_NUMBER;
}
\"[^"]*\" {
	yytext[strlen(yytext)-1] = 0;
	yylval->expr = apexpr_literal(yylloc, aptype_object(strdup("String")), 
		strdup(yytext+1));
	return TOK_STRING;
}
\'[^']*\' {
	yylval->expr = apexpr_literal(yylloc, aptype_object(strdup("String")), 
		strdup(yytext)); 
	return TOK_STRING;
}
[A-Z][A-Z0-9]* {
	yylval->string = strdup(yytext);
	return TOK_CONST;
}
[A-Z][A-Za-z0-9]* {
	yylval->type = aptype_object(strdup(yytext)); 
	return TOK_TYPE; 
}
([a-z]|[a-z][0-9a-z_]*[0-9a-z])(\?|\!)? {
	yylval->string = strdup(yytext); 
	return TOK_IDENT;
}
#.*
[\n\r] yyget_extra(yyscanner)->column = 1;
[\t ]
. return yytext[0];
%%

