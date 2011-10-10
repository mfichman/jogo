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
#include "Lexer.hpp"
#include <fstream>
#include <set>

/* Primary parser structure; creates compilation units */
class Parser : public Object {
public:
    Parser(Environment* env);
    Environment* environment() const { return env_; }
    Module* module() const { return module_; }
	File* file() const { return file_; }
    void input(const std::string& file);
    typedef Pointer<Parser> Ptr;

private:
    void file(const std::string& prefix, const std::string& file);
    void dir(const std::string& prefix, const std::string& dir);

    Location loc() const { return lexer_->loc(); }
    Token token() const { return lexer_->token(); }
    void next() { lexer_->next(); }
    void error() { error_++; env_->error(); }
    bool expect(Token token);
    Expression* op(Expression* left, const std::string& op, Expression* right);
    Expression* op(const std::string& op, Expression* expr);

    Module* module();
    Class* clazz();
    Feature* features();
    Feature* feature();
    Function* function();
    Attribute* attribute();
    Import* import();
    Function* method();
    String* identifier();
    Type* type();
    Block* block();
    String* scope();
    String* comment();
    Feature::Flags flags();

    Statement* statement();

    Expression* expression();
    Expression* member();
    Expression* call();
    Expression* increment();
    Expression* unary();
    Expression* pow();
    Expression* mult();
    Expression* addition();
    Expression* shift();
    Expression* relational();
    Expression* equality();
    Expression* logical_and();
    Expression* logical_or();
    Expression* bitwise_and();
    Expression* bitwise_or();
    Expression* bitwise_xor();
    Expression* assignment();
    Expression* literal();


    Environment::Ptr env_;
    Module::Ptr module_;
    File::Ptr file_;
    Stream::Ptr err_;
    Lexer::Ptr lexer_;
    bool is_input_file_;
    int error_;

};

/* Union used to return parser values */
union ParseNode {
	Expression* expression;
	Statement* statement;
	Formal* formal;
	When* when;
	Feature* feature;
	String* string;
    Type* type;
    Generic* generic;
    Assignment* assignment;
    Block* block;
	Feature::Flags flag;
};

