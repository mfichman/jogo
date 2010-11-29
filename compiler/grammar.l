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
%s SEPARATOR

/* Lexer rules */
%%
class BEGIN(INITIAL); return TOK_CLASS;
interface BEGIN(INITIAL); return TOK_INTERFACE;
struct BEGIN(INITIAL); return TOK_STRUCT;
module BEGIN(INITIAL); return TOK_MODULE;
import BEGIN(INITIAL); return TOK_IMPORT;
def	BEGIN(INITIAL); return TOK_DEF;
var BEGIN(INITIAL); return TOK_VAR; 
\@init BEGIN(INITIAL); return TOK_INIT;
\@destroy BEGIN(INITIAL); return TOK_DESTROY;
public BEGIN(INITIAL); return TOK_PUBLIC;
private BEGIN(INITIAL); return TOK_PRIVATE;
static BEGIN(INITIAL); return TOK_STATIC;
native BEGIN(INITIAL); return TOK_NATIVE;
while BEGIN(INITIAL); return TOK_WHILE;
else BEGIN(INITIAL); return TOK_ELSE;
until BEGIN(INITIAL); return TOK_UNTIL;
if BEGIN(INITIAL); return TOK_IF;
do BEGIN(INITIAL); return TOK_DO;
for BEGIN(INITIAL); return TOK_FOR;
return BEGIN(SEPARATOR); return TOK_RETURN;
== BEGIN(INITIAL); return TOK_EQUAL; 
!= BEGIN(INITIAL); return TOK_NOTEQUAL; 
\|\| BEGIN(INITIAL); return TOK_OR; 
\&\& BEGIN(INITIAL); return TOK_AND; 
\>\= BEGIN(INITIAL); return TOK_GE;
\<\= BEGIN(INITIAL); return TOK_LE;
\<\< BEGIN(INITIAL); return TOK_LSHIFT;
\>\> BEGIN(INITIAL); return TOK_RSHIFT;
\*\= BEGIN(INITIAL); return TOK_MUL_ASSIGN;
\/\= BEGIN(INITIAL); return TOK_DIV_ASSIGN;
\-\= BEGIN(INITIAL); return TOK_SUB_ASSIGN; 
\+\= BEGIN(INITIAL); return TOK_ADD_ASSIGN;
\%\= BEGIN(INITIAL); return TOK_MOD_ASSIGN;
\|\= BEGIN(INITIAL); return TOK_BITOR_ASSIGN;
\&\= BEGIN(INITIAL); return TOK_BITAND_ASSIGN;
\+\+ BEGIN(INITIAL); return TOK_INC;
\-\- BEGIN(INITIAL); return TOK_DEC;
[0-9]+ {
	yylval->expr = apexpr_literal(yylloc, aptype_object(strdup("Int")), 
	strdup(yytext)); 
    BEGIN(SEPARATOR);
	return TOK_NUMBER;
}
\"[^"]*\" {
	yytext[strlen(yytext)-1] = 0;
	yylval->expr = apexpr_literal(yylloc, aptype_object(strdup("String")), 
	strdup(yytext+1));
    BEGIN(SEPARATOR);
	return TOK_STRING;
}
\'[^']*\' {
	yylval->expr = apexpr_literal(yylloc, aptype_object(strdup("String")), 
	strdup(yytext)); 
    BEGIN(SEPARATOR);
	return TOK_STRING;
}
[A-Za-z0-9][A-Za-z0-9_]*(::[A-Za-z0-9][A-Za-z0-9_]*)*[?!]? {
	yylval->string = strdup(yytext); 
    BEGIN(SEPARATOR);
	return TOK_IDENT;
}
#.*
[{([,] BEGIN(INITIAL); return yytext[0];
[)\]] BEGIN(SEPARATOR); return yytext[0];
<SEPARATOR>[\n\r] BEGIN(INITIAL); yyget_extra(yyscanner)->column = 1; return TOK_SEP;
<INITIAL>[\n\r] yyget_extra(yyscanner)->column = 1;
[\t ]
. return yytext[0];
%%


void apparser_newline(apparser_t *self) {
	struct yyguts_t *yyg = (struct yyguts_t *)self->scanner;

	yylineno = 1;
}
