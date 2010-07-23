#include <lexer.hpp>
#include <cassert>
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

static std::map<char, Token> table_op();
static std::map<std::string, Token> table_word();
static const std::map<char, Token> op_ = table_op();
static const std::map<std::string, Token> word_ = table_word();

Lexer::Lexer(Input::Ptr input) :
    input_(input),
    line_number_(1),
    char_number_(0) {

}

const std::string& Lexer::value() {
    return value_;
}

Token Lexer::token() {
    char c;
    value_ = "";

    /* Read any leading whitespace characters */
    while (!input_->eof() && isspace(c = input_->top())) {
        if (c == '\r' || c == '\n') {
            char_number_ = 0;
            line_number_++;
        } else {
            char_number_++;
        }
    }

    /* Check for EOF character */
    if (input_->eof()) {
        return TOK_EOF;
    }

    /* Check for the :: token, special case */
    if (c == ':') {
        char d = input_->top();
        if (c == ':') {
            char_number_++;
            return TOK_SCOPE;
        } else {
            input_->top(d);
        }
    }

    /* Check for single operatators */
    std::map<char, Token>::const_iterator i = op_.find(c);
    if (i != op_.end()) {
        char_number_++;
        return i->second;
    }
    input_->top(c);

    /* Check for literal strings */
    if (c == '"' || c == '\'') {
        return token_string();
    }
        
    /* Check for alphanumeric keywords and identifiers */
    if (islower(c) || c == '@') {
        return token_word();
    }

    /* Check for type words */
    if (isupper(c)) {
        return token_type();
    }

    /* Check for numbers */
    if (isdigit(c)) {
        return token_number();
    }
    
    return TOK_INVALID;
}

Token Lexer::token_string() {
    std::string string;
    char start = input_->top();
    char_number_++;

    /* Scan for the end of the string */
    while (true) {
        char c = input_->top();
        if (c == '\\') {
            string += input_->top();
            char_number_++;
        } else if (c != start) {
            string += c;
            char_number_++;
        } else {
            break;
        }
    }   
    
    /* 
     * This should never happen, because this function only gets called when
     * there is at least one character in the input queue.
     */
    assert(!string.empty());
    
    return TOK_STRING;
}

Token Lexer::token_word() {
    std::string word;

    /* Scan for alphanumeric characters or underscores */
    while (true) {
        char c = input_->top();
        if (islower(c) || c == '_' || c == '!' || c == '?' || c == '@') {
            word += c;
            char_number_++;
        } else if (isupper(c)) {
            throw std::runtime_error("Illegal identifier");
        } else {
            input_->top(c);
            break;
        }
    }

    cout << "WORD: " << word << endl;

    /* 
     * This should never happen, because this function only gets called when
     * there is at least one character on the input queue.
     */
    assert(!word.empty());
    
    /* Search for reserved words */
    std::map<std::string, Token>::const_iterator i = word_.find(word); 
    if (i != word_.end()) {
        return i->second;
    }

    /* Return as an identifier */
    value_ = word;
    return TOK_IDENT;
}

Token Lexer::token_type() {
    std::string word;

    /* Scan for alphanumeric characters */
    while (true) {
        char c = input_->top();
        if (isalnum(c)) {
            word += c;
            char_number_++;
        } else if (islower(c) || c == '_') {
            throw std::runtime_error("Illegal class or package name");
        } else {
            input_->top(c);
            break;
        }
    }
    cout << "TYPE: " << word << endl;

    /* Type token or package*/
    value_ = word;
    return TOK_TYPE;
}

Token Lexer::token_number() {
    std::string number;

    /* Scan for numeric characters */
    while (true) {
        char c = input_->top();
        if (isdigit(c)) {
            number += c;
            char_number_++;
        } else {
            input_->top(c);
            break;
        }
    }

    /* 
     * This should never assert false, because this function only gets called
     * when there is at least one character on the input queue.
     */
    assert(!number.empty());

    /* Return as a number*/
    value_ = number;
    return TOK_NUMBER;
}

size_t Lexer::line_number() const {
    return line_number_;
}

size_t Lexer::char_number() const {
    return char_number_;
}

std::map<char, Token> table_op() {
    std::map<char, Token> op;

    op['('] = TOK_LPAREN;
    op[')'] = TOK_RPAREN;
    op['{'] = TOK_LBRACE;
    op['}'] = TOK_RBRACE;
    op['['] = TOK_LBRACKET;
    op[']'] = TOK_RBRACKET;
    op['='] = TOK_EQUAL;
    op['^'] = TOK_CARET;
    op['*'] = TOK_ASTERISK;
    op['+'] = TOK_PLUS;
    op['-'] = TOK_MINUS;
    op['!'] = TOK_BANG;
    op['?'] = TOK_QUESTION;
    op[';'] = TOK_SEMI;
    //op[':'] = TOK_COLON;
    op['.'] = TOK_DOT;

    return op;
}

std::map<std::string, Token> table_word() {
    std::map<std::string, Token> word;

    word["class"] = TOK_CLASS;
    word["struct"] = TOK_STRUCT;
    word["interface"] = TOK_INTERFACE;
    word["module"] = TOK_MODULE;
    word["public"] = TOK_PUBLIC;
    word["private"] = TOK_PRIVATE;
    word["protected"] = TOK_PROTECTED;
    word["static"] = TOK_STATIC;
    word["native"] = TOK_NATIVE;
    word["def"] = TOK_DEF;
    word["if"] = TOK_IF;
    word["then"] = TOK_THEN;
    word["else"] = TOK_ELSE;
    word["while"] = TOK_WHILE;
    word["until"] = TOK_UNTIL;
    word["switch"] = TOK_SWITCH;
    word["case"] = TOK_CASE;
    word["default"] = TOK_DEFAULT;
    word["do"] = TOK_DO;
    word["try"] = TOK_TRY;
    word["catch"] = TOK_CATCH;
    word["finally"] = TOK_FINALLY;
    word["throw"] = TOK_THROW;
    word["void"] = TOK_VOID;
    word["package"] = TOK_PACKAGE;
    word["import"] = TOK_IMPORT;
    word["self"] = TOK_SELF;
    word["@init"] = TOK_INIT;
    word["@destroy"] = TOK_DESTROY;
    word["nil"] = TOK_NIL;
    word["true"] = TOK_TRUE;
    word["false"] = TOK_FALSE;
    word["int"] = TOK_INT;
    word["uint"] = TOK_UINT;
    word["long"] = TOK_LONG;
    word["ulong"] = TOK_ULONG;
    word["short"] = TOK_SHORT;
    word["ushort"] = TOK_USHORT;
    word["byte"] = TOK_BYTE;
    word["ubyte"] = TOK_UBYTE;

    return word;
}

