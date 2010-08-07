%{
#include <parser.hpp>
#include <expr.h>

#define YY_INPUT(buffer, result, max_size) \
    yyinput(yyget_extra(yyscanner), buffer, result, max_size);

%}

%option reentrant bison-bridge
%option noyywrap
%option nounput
%option yylineno

/* Lexer rules */
%%
class { return TOK_CLASS; }
interface { return TOK_INTERFACE; }
struct { return TOK_STRUCT; }
module { return TOK_MODULE; }
import { return TOK_IMPORT; }
def	{ return TOK_DEF; }
\@init { return TOK_INIT; }
\@destroy { return TOK_DESTROY; }
public { return TOK_PUBLIC; }
private { return TOK_PRIVATE; }
protected { return TOK_PROTECTED; }
static { return TOK_STATIC; }
native { return TOK_NATIVE; }
while { return TOK_WHILE; }
else { return TOK_ELSE; }
until { return TOK_UNTIL; }
if { return TOK_IF; }
foreach { return TOK_FOREACH; }
do { return TOK_DO; }
for { return TOK_FOR; }
== { return TOK_EQUAL; } 
!= { return TOK_NOTEQUAL; } 
:: { return TOK_SCOPE; } 
\|\| { return TOK_OR; } 
\&\& { return TOK_AND; } 
\>\= { return TOK_GE; }
\<\= { return TOK_LE; }
\<\< { return TOK_LSHIFT; }
\>\> { return TOK_RSHIFT; }
\*\= { return TOK_MUL_ASSIGN; }
\/\= { return TOK_DIV_ASSIGN; }
\-\= { return TOK_SUB_ASSIGN; } 
\+\= { return TOK_ADD_ASSIGN; }
\%\= { return TOK_MOD_ASSIGN; }
\|\= { return TOK_BITOR_ASSIGN; }
\&\= { return TOK_BITAND_ASSIGN; }
\+\+ { return TOK_INC; }
\-\- { return TOK_DEC; }
int { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
uint { $$ = type_primitive(yylval); return TOK_PRIMITIVE; } 
short { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
ushort { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
byte { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
ubyte { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
long { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
ulong { $$ = type_primitive(yylval); return TOK_PRIMITIVE; }
[A-Z][A-Za-z0-9] { $$ = type_name(yylval); return TOK_TYPE; } 
[a-z]|[a-z][0-9a-z_]*[0-9a-z] { $$ = expr_str(yylval); return TOK_IDENT }
%%


void yyinput(Parser* parser, char* buffer, int& result, int max) {
    parser->input(buffer, result, max);
}

void yyerror(Parser* parser, void* yyscanner, const char* s) {
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    stringstream ss;
    ss << s << " (line " << yylineno << ")";
    throw runtime_error(ss.str());
}

