%{
#include <parser.h>
#include <expr.h>
#include <type.h>
#include <grammar.h>
#include <ctype.h>

#define YY_INPUT(buf, res, len) { \
	res = parser_read(yyget_extra(yyscanner), buf, len);\
}

inline char *strdup(const char* str) {
	char *copy = malloc(strlen(str) + 1);
	strcpy(copy, str);
	return copy;
}

%}

%option reentrant 
%option bison-bridge
%option never-interactive
%option noyywrap
%option nounput
%option yylineno

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
foreach return TOK_FOREACH;
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
int yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
uint yylval->type = type_primitive(yytext); return TOK_PRIMITIVE; 
short yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
ushort yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
byte yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
ubyte yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
long yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
ulong yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
bool yylval->type = type_primitive(yytext); return TOK_PRIMITIVE;
[0-9]+ yylval->string = strdup(yytext); return TOK_NUMBER;
\"[^"]*\" yylval->string = strdup(yytext); return TOK_STRING;
\'[^']*\' yylval->string = strdup(yytext); return TOK_STRING;
[A-Z][A-Z0-9]* {
	yylval->string = strdup(yytext);
	return TOK_CONST;
}
[A-Z][A-Za-z0-9]* yylval->type = type_name(yytext); return TOK_TYPE; 
([a-z]|[a-z][0-9a-z_]*[0-9a-z])(\?|\!)? {
	yylval->string = strdup(yytext); 
	return TOK_IDENT;
}
#.*
[\t\n\r ]
. return yytext[0];
%%


void yyerror(parser_t *self, void* scanner, const char *message) {
	struct yyguts_t *yyg = (struct yyguts_t*)scanner;
	

	fprintf(stderr, "%c", toupper(message[0]));
	fprintf(stderr, "%s (line %d)\n", message + 1, yylineno);
}
