#pragma once

enum Token {

    /* Grouping operators */
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,

    /* Equality */
    TOK_EQUAL,
    TOK_NEQUAL,
    TOK_GT,
    TOK_LT,
    TOK_GTEQ,
    TOK_LTEQ,

    /* Math operators */
    TOK_AND,
    TOK_OR,
    TOK_BITAND,
    TOK_BITOR,
    TOK_BITXOR,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_BACKSLASH,
    TOK_MOD,
    TOK_LSHIFT,
    TOK_RSHIFT,
    TOK_TILDE,
    TOK_INC,
    TOK_DEC,

    /* Assignment operators */
    TOK_ASSIGN,
    TOK_MULASSIGN,
    TOK_DIVASSIGN,
    TOK_MODASSIGN,
    TOK_ADDASSIGN,
    TOK_SUBASSIGN,
    TOK_LSHIFTASSIGN,
    TOK_RSHIFTASSIGN,
    TOK_BITANDASSIGN,
    TOK_BITORASSIGN,
    TOK_BITXORASSIGN,
    
    /* Unary operators */
    TOK_BANG,
    TOK_QUESTION,
    TOK_SEMI,
    TOK_COLON,
    TOK_SCOPE,
    TOK_DOT,
    TOK_COMMA,
    TOK_EOF,

    /* Reserved words */
    TOK_CLASS,
    TOK_STRUCT,
    TOK_INTERFACE,
    TOK_MODULE,
    TOK_PUBLIC,
    TOK_PRIVATE,
    TOK_PROTECTED,
    TOK_STATIC,
    TOK_NATIVE,
    TOK_DEF,
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

    /* Literals */
    TOK_NIL,
    TOK_TRUE,
    TOK_FALSE,
    TOK_COMMENT,

    /* Primitives */
    TOK_INT,
    TOK_UINT,
    TOK_LONG,
    TOK_ULONG,
    TOK_SHORT,
    TOK_USHORT,
    TOK_BYTE,
    TOK_UBYTE,    

    /* Other */
    TOK_NUMBER,
    TOK_IDENT,
    TOK_TYPE,
    TOK_STRING,
    TOK_INVALID,

    SYM_POP,
    SYM_TRANSLATION_UNIT,
    SYM_CLASS_MEMBER_LIST,
    SYM_INTERFACE_MEMBER_LIST,
    SYM_STRUCT_MEMBER_LIST,
    SYM_MODULE_MEMBER_LIST,
    SYM_IMPORT,
    SYM_DEF,
    SYM_VARIABLE,
    SYM_CONSTRUCTOR,
    SYM_DESTRUCTOR,
    SYM_FUNCTION,
    SYM_PROTOTYPE,
    SYM_ACCESS,
    SYM_STORAGE,
    SYM_NATIVE,
    SYM_TYPE,
    SYM_INITIALIZER,
    SYM_QUALIFIED_NAME,
    SYM_QUALIFIED_NAME_TAIL,
    SYM_COMPOUND_STATEMENT,
    SYM_STATEMENT_LIST,
    SYM_STATEMENT,
    SYM_ASSIGNMENT,
    SYM_ASSIGNMENT_LIST,
    SYM_LOGICAL_OR,
    SYM_LOGICAL_OR_LIST,
    SYM_LOGICAL_AND,
    SYM_LOGICAL_AND_LIST,
    SYM_BITWISE_OR,
    SYM_BITWISE_OR_LIST,
    SYM_BITWISE_AND,
    SYM_BITWISE_AND_LIST,
    SYM_EQUALITY,
    SYM_EQUALITY_LIST,
    SYM_RELATION,
    SYM_RELATION_LIST,
    SYM_SHIFT,
    SYM_SHIFT_LIST,
    SYM_ADDITION,
    SYM_ADDITION_LIST,
    SYM_MULTIPLICATION,
    SYM_MULTIPLICATION_LIST,
    SYM_UNARY,
    SYM_POSTFIX,
    SYM_POSTFIX_LIST,
    SYM_PRIMARY
};

