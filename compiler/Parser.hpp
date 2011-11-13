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
    Location location() const { return lexer_->loc(); }
    Location last_location() const { return last_location_; } 
    void input(const std::string& file);
    typedef Pointer<Parser> Ptr;

private:
    void file(const std::string& prefix, const std::string& file);
    void dir(const std::string& prefix, const std::string& dir);

    String* name(const std::string& name) { return env_->name(name); }
    void next();
    void error() { error_++; env_->error(); }
    bool expect(Token token);

    Module* module();
    Class* clazz();
    Feature* feature_list();
    Feature* feature();
    Constant* constant();
    Function* function();
    Attribute* attribute();
    Function* method();
    String* identifier();
    Type* type();
    Block* block();
    String* scope();
    String* comment();
    Feature::Flags flags();
    void import();

    Statement* statement();
    Statement* conditional();
    Statement* while_loop();
    Statement* for_loop();
    Statement* let();

    Expression* string();
    Expression* expression();
    Expression* expression_list();
    Expression* member();
    Expression* call();
    Expression* construct();
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
    Expression* literal();
    Expression* closure();
    Assignment* assignment();

    Token token(int index=0) const { 
        return lexer_->token(index); 
    }
    const std::string& value(int index=0) const { 
        return lexer_->value(index); 
    }

    Expression* op(const LocationAnchor& loc, const std::string& op, 
                   Expression* left, Expression* right);
    Expression* op(const LocationAnchor& loc, const std::string& op, 
                   Expression* expr);


    Environment::Ptr env_;
    Module::Ptr module_;
    File::Ptr file_;
    Stream::Ptr err_;
    Lexer::Ptr lexer_;
    Location last_location_;
    bool is_input_file_;
    int error_;

};

class LocationAnchor {
public:
    LocationAnchor(Parser* parser) : 
        parser_(parser),
        location_(parser->location()) {
    }
    operator Location() const { 
        Location loc;
        loc.file = location_.file;
        loc.first_line = location_.first_line;
        loc.first_column = location_.first_column;
        loc.last_line = parser_->last_location().last_line;
        loc.last_column = parser_->last_location().last_column;
        return loc;
    }

private:
    Parser* parser_;
    Location location_; 
};

