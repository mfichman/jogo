%{
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "parser.hpp"
#include "regex.y.hpp"
using namespace std;

#define YY_INPUT(buffer, result, max_size) \
    yyinput(yyget_extra(yyscanner), buffer, result, max_size);

%}

%option reentrant bison-bridge
%option noyywrap
%option nounput
%option yylineno

/* Lexer rules */
%%
\?:                         return GROUP;
\?!                         return NLOOKAHEAD;
\?=                         return PLOOKAHEAD;
\?<!                        return NLOOKBEHIND;
\?<=                        return PLOOKBEHIND;
[\n\r]                      return NEWLINE;
^#.*?$                      return COMMENT;
\\[0-9]+                    *yylval = Node::backref(yytext+1); return BACKREF; 
\\.                         *yylval = Node::escape(yytext); return CHAR;
[^[\\\^$|?*+(){}]           *yylval = Node::literal(yytext); return CHAR;
\{[0-9]*,[0-9]+\}           return RANGE;
\{[0-9]+,[0-9]*\}           return RANGE;  
\[[^]]*?[^\\]\]             *yylval = Node::cclass(yytext); return CCLASS;
\\x[0-9a-fA-F]{2}           *yylval = Node::hex(yytext); return CHAR;
.                           return yytext[0];
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

/*\\[[\\\^$|?*+(){}]          *yylval = Node::escape(yytext); return CHAR; */
