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

#include "Jogo.hpp"
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
    typedef int Flags;

    Parser(Environment* env);
    Environment* environment() const { return env_; }
    Module* module() const { return module_; }
    File* file() const { return file_; }
    Location location() const { return lexer_->loc(); }
    Location last_location() const { return last_location_; } 
    void input(const std::string& file, bool optional=false);
    void library(const std::string& file);
    typedef Pointer<Parser> Ptr;

private:
    void file_alias(const std::string& import); 
    void file(const std::string& prefix, const std::string& file);
    void dir(const std::string& prefix, const std::string& dir);
    void implicit_import(Type* type, Flags flags=Import::QUALIFIED);
    void implicit_import(String* scope, Flags flags=Import::QUALIFIED);
    void module_feature(Feature* feature, String* scope);
    void implicit_self(Feature* func, Type* type);

    String* name(const std::string& name) { return env_->name(name); }
    void next();
    void error() { error_++; env_->error(); }
    bool expect(Token token);

    Class* clazz(String* scope);
    Class* alternate(String* name);
    Module* module();
    Function* function();
    Formal* formal_list();
    Constant* constant();
    Feature* feature_list();
    Feature* feature();
    Generic* generic_list();
    Type* alternate_list(); 
    Feature* constant_list();
    Attribute* composite();
    Attribute* attribute();
    Function* method();
    String* identifier();
    Type* type();
    Block* block();
    Match* match();
    Case* case_list();
    Case* single_case();
    String* maybe_scope();
    String* scope();
    String* comment();
    Feature::Flags flags();
    void import();

    Expression* statement();
    Expression* conditional();
    Expression* while_loop();
    Expression* for_loop();
    Expression* let();

    Assignment* assignment();
    Expression* regex();
    Expression* string();
    Expression* expression();
    Expression* expression_list();
    Expression* member_or_call_or_index();
    Expression* call_suffix(Expression* expr, LocationAnchor const& loc);
    Expression* index_suffix(Expression* expr, LocationAnchor const& loc);
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
    Expression* byte_or_char_literal();
    Expression* string_literal();
    Expression* integer_literal();
    Expression* array_literal();
    Expression* hash_literal();
    Expression* pair();
    Expression* closure();
    bool is_literal();

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


    Type::Ptr type_;
    Environment::Ptr env_;
    Module::Ptr module_;
    File::Ptr file_;
    Stream::Ptr err_;
    Lexer::Ptr lexer_;
    Location last_location_;
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
        loc.first_offset = location_.first_offset;
        loc.last_line = parser_->last_location().last_line;
        loc.last_column = parser_->last_location().last_column;
        loc.last_offset = parser_->last_location().last_offset;
        return loc;
    }

private:
    Parser* parser_;
    Location location_; 
};

