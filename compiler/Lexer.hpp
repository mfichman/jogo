/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#pragma once

#include "Apollo.hpp"
#include "Environment.hpp"
#include "Feature.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "Stream.hpp"
#include <fstream>
#include <map>

#undef VOID
#undef FLOAT
#undef ERROR
#undef IN // gdmf windows...
#undef CONSTANT

/* Token type, containing an enumeration for all valid Apollo tokens */
class Token { 
public:
    enum Type {
        OR, AND, XORB, ANDB, ORB, ASSIGN, EQUAL, NOT_EQUAL, LESS, GREATER,
        LESS_OR_EQ, GREATER_OR_EQ, COMPARE, LEFT_SHIFT, RIGHT_SHIFT, ADD, SUB, 
        MUL, DIV, MOD, NOT, POW, INCREMENT, DECREMENT, IDENTIFIER, TYPE, 
        OPERATOR, COMMENT, TYPEVAR, STRING, PUBLIC, STATIC, NATIVE, 
        WEAK, IMMUTABLE, IMPORT, VOID, SEPARATOR, BACKTICK, SEMICOLON,         
        WHILE, ELSE, UNTIL, IF, DO, FOR, RETURN, SCOPE, LET, IN, 
        YIELD, FORK, ERROR, END, NONE, DOT, FLOAT, INTEGER, COMMA, COLON,
        LEFT_BRACKET, RIGHT_BRACKET, LEFT_PARENS, RIGHT_PARENS, COMPL, 
        STRING_BEGIN, STRING_END, CHAR, LEFT_BRACE, RIGHT_BRACE, MATCH, FUNC, 
        PRIVATE, BOOL, NIL, EOF_LITERAL, IS, CONSTANT, WITH, MATCH_OP, REGEX
    };

    Token() : type_(NONE) {}
    Token(const Type& type) : type_(type) {}
    const Token& operator=(const Type& type) { 
        type_ = type; 
        return *this; 
    }
    operator Type() const { return type_; }
    Type type() const { return type_; }
    std::string& value() { return value_; }
    const Location& location() const { return location_; }
    const std::string& value() const { return value_; }
    bool is_operator() const;
    void value(const std::string& value) { value_ = value; }
    void location(const Location& loc) { location_ = loc; }

private:
    Type type_;
    std::string value_;
    Location location_;
}; 

Stream::Ptr operator<<(Stream::Ptr out, const Token& token);

#define LEXER_LOOKAHEAD 4

/* Generates a stream of tokens for the parser */
class Lexer : public Object {
public:
    Lexer(Environment* env);
    void input(File* file);
    void next();
    const std::string& value(int index=0) const;
    const Token& token(int index=0) const;
    const Location& loc(int index=0) const;
    typedef Pointer<Lexer> Ptr;
    
private:
    void token(Token::Type type) { token_[front_] = type; }
    void value(const std::string& value) { token_[front_].value(value); }

    void regex();
    void number_or_dot();
    void special();
    void ident_or_keyword();
    void operator_or_typevar();
    void type_or_const(); 
    void string_or_char();
    void string();
    void comment();
    void read();

    Environment::Ptr env_;
    Stream::Ptr err_;
    std::ifstream input_;
    Token token_[LEXER_LOOKAHEAD];
    Location location_;
    int front_;
    int char_;
    int line_;
    int column_;
    std::map<std::string, Token::Type> keyword_; 
    int string_level_;
    bool ignore_newline_;
    bool expect_comment_;
};
