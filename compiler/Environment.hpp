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
#include "Name.hpp"
#include <map>

/* Compilation environment; contains symbol table and compilation units */
class Environment : public Object {
public:
    Environment() :
        root_(new Module(Location(), name(""))),
        void_type_(new Type(0, name("Void"), 0, this)),
        boolean_type_(new Type(0, name("Boolean"), 0, this)),
        integer_type_(new Type(0, name("Integer"), 0, this)),
        string_type_(new Type(0, name("String"), 0, this)),
        no_type_(new Type(0, name("<<notype>>"), 0, this)),
        float_type_(new Type(0, name("Float"), 0, this)),
        errors_(0) {
    }
    Name* name(const std::string& str);
    Module* root() const { return root_; }
    Module* module(Import* import);
    Module* module(const std::string& filename);
    int errors() { return errors_; }
    void error(const std::string& error) { errors_++; }
    typedef Pointer<Environment> Ptr;

    Type* void_type() const { return void_type_; }
    Type* boolean_type() const { return boolean_type_; }
    Type* integer_type() const { return integer_type_; }
    Type* string_type() const { return string_type_; }
    Type* no_type() const { return no_type_; }
    Type* float_type() const { return float_type_; }

private:
    std::map<std::string, Name::Ptr> name_;
    Module::Ptr root_;
    Type::Ptr void_type_;
    Type::Ptr boolean_type_;
    Type::Ptr integer_type_;
    Type::Ptr string_type_;
    Type::Ptr no_type_;
    Type::Ptr float_type_;
    int errors_;
};

