/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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
#include "Object.hpp"
#include "Feature.hpp"
#include "String.hpp"
#include <map>

/* Compilation environment; contains symbol table and compilation units */
class Environment : public Object {
public:
    Environment();
    String* name(const std::string& str);
    String* number(const std::string& str);
    String* string(const std::string& str);
    Module* module(String* scope);
    Module* modules() const { return modules_; }
    Module* unit(String* scope);
    Module* root() const { return root_; }
    Module* builtins() const { return builtins_; }
    int errors() { return errors_; }
    void module(Module* module);
    void unit(Module* module);
    void error(const std::string& error) { errors_++; }
    const Location& location() const;
    typedef Pointer<Environment> Ptr;

    Type* void_type() const { return void_type_; }
    Type* boolean_type() const { return boolean_type_; }
    Type* integer_type() const { return integer_type_; }
    Type* string_type() const { return string_type_; }
    Type* no_type() const { return no_type_; }
    Type* float_type() const { return float_type_; }
    Type* self_type() const { return self_type_; }

private:
    void init_integer();
    void init_boolean();
    void init_string();
    void init_float();

    std::map<std::string, String::Ptr> name_;
    std::map<std::string, String::Ptr> number_;
    std::map<std::string, String::Ptr> string_;
    std::map<String::Ptr, Module::Ptr> module_;
    std::map<String::Ptr, Module::Ptr> unit_;
    Module::Ptr root_;
    Module::Ptr builtins_;
    Module::Ptr modules_;
    Type::Ptr void_type_;
    Type::Ptr boolean_type_;
    Type::Ptr integer_type_;
    Type::Ptr string_type_;
    Type::Ptr no_type_;
    Type::Ptr float_type_;
    Type::Ptr self_type_;
    int errors_;
};

