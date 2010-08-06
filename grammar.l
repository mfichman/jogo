%{
#include "parser.hpp"

#define YY_INPUT(buffer, result, max_size) \
    yyinput(yyget_extra(yyscanner), buffer, result, max_size);

%}

%option reentrant bison-bridge
%option noyywrap
%option nounput
%option yylineno

/* Lexer rules */
%%
class TOK_CLASS
interface TOK_INTERFACE
struct TOK_STRUCT
module TOK_MODULE
import TOK_IMPORT
def	TOK_DEF
[a-z]|[a-z][0-9a-z_]*[0-9a-z] TOK_IDENT
\@init TOK_INIT
\@destroy TOK_DESTROY
public TOK_PUBLIC
private TOK_PRIVATE
protected TOK_PROTECTED
static TOK_STATIC
native TOK_NATIVE
int TOK_INT
uint TOK_UINT
short TOK_SHORT
ushort TOK_USHORT
byte TOK_BYTE
ubyte TOK_UBYTE
long TOK_LONG
ulong TOK_ULONG
[A-Z][A-Za-z0-9] TOK_TYPE
== TOK_EQUAL
!= TOK_NOTEQUAL
:: TOK_SCOPE
\|\| TOK_OR
\&\& TOK_AND
>= TOK_GE
<= TOK_LE
<< TOK_LSHIFT
>> TOK_RSHIFT
*= TOK_MUL_ASSIGN
/= TOK_DIV_ASSIGN
-= TOK_SUB_ASSIGN
+= TOK_ADD_ASSIGN
%= TOK_MOD_ASSIGN
|= TOK_BITOR_ASSIGN
&= TOK_BITAND_ASSIGN
++ TOK_INC
-- TOK_DEC
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

