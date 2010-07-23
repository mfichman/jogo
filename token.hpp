#pragma once

enum Token {

    /* Operators */
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_EQUAL,
    TOK_CARET,
    TOK_ASTERISK,
    TOK_PLUS,
    TOK_MINUS,
    TOK_BANG,
    TOK_QUESTION,
    TOK_SEMI,
    TOK_COLON,
    TOK_SCOPE,
    TOK_DOT,
    TOK_COMMA,
    TOK_EOF,
    TOK_OPBEGIN = TOK_LPAREN,
    TOK_OPEND = TOK_EOF,

    /* Reserved words */
    TOK_CLASS,
    TOK_STRUCT,
    TOK_INTERFACE,
    TOK_PUBLIC,
    TOK_PRIVATE,
    TOK_PROTECTED,
    TOK_STATIC,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_FOR,
    TOK_WHILE,
    TOK_UNTIL,
    TOK_SWITCH,
    TOK_CASE,
    TOK_DEFAULT,
    TOK_DO,
    TOK_TRY,
    TOK_CATCH,
    TOK_FINALLY,
    TOK_THROW,
    TOK_VOID,
    TOK_PACKAGE,
    TOK_IMPORT,
    TOK_SELF,
    TOK_INIT,
    TOK_DESTROY,
    TOK_RWORDBEGIN = TOK_CLASS,
    TOK_RWORDEND = TOK_DESTROY,

    /* Literals */
    TOK_NIL,
    TOK_TRUE,
    TOK_FALSE,
    TOK_COMMENT,
    TOK_LITBEGIN = TOK_NIL,
    TOK_LITEND = TOK_FALSE,

    /* Primitives */
    TOK_INT,
    TOK_UINT,
    TOK_LONG,
    TOK_ULONG,
    TOK_SHORT,
    TOK_USHORT,
    TOK_BYTE,
    TOK_UBYTE,    
    TOK_PRIMEND = TOK_UBYTE,
    TOK_PRIMBEGIN = TOK_INT,

    /* Other */
    TOK_NUMBER,
    TOK_IDENT,
    TOK_TYPE,
    TOK_STRING,
    TOK_INVALID,
    
    TOK_BEGIN = TOK_LPAREN,
    TOK_END = TOK_INVALID,

    SYM_POP,
    SYM_FILE,
    SYM_IMPORTS,
    SYM_IMPORT,
    SYM_DEFS,
    SYM_DEF,
    SYM_TYPENAME,
    SYM_TYPETOKEN,
    SYM_TYPE,
    SYM_INIT,
    SYM_FUNCTION,
    SYM_EXPR
};

inline bool isoperator(Token token) {
    return token >= TOK_OPBEGIN && token <= TOK_OPEND;
}

inline bool isrword(Token token) {
    return token >= TOK_RWORDBEGIN && token <= TOK_RWORDEND;
}

inline bool isliteral(Token token) {
    return token >= TOK_LITBEGIN && token <= TOK_LITEND;
}

inline bool isprimitive(Token token) {
    return token >= TOK_PRIMBEGIN && token <= TOK_PRIMEND;
}
