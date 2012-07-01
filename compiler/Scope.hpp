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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
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
#include "String.hpp"
#include "BasicBlock.hpp"
#include <vector>
#include <map>

class Scope;

/* Variable structure, used to store info about assigned vars in a scope */
class Variable : public Object {
public:
    Variable(String* name, Operand op, Type* type, bool immut=false) :
        name_(name),
        operand_(op),
        type_(type),
        scope_(0),
        is_immutable_(immut) {
    }

    String* name() const { return name_; }
    Operand operand() const { return operand_; }
    Type* type() const { return type_; }
    Scope* scope() const { return scope_; }
    bool is_immutable() const { return is_immutable_; }
    void scope(Scope* scope) { scope_ = scope; }
    typedef Pointer<Variable> Ptr;

private:
    String::Ptr name_;
    Operand operand_;
    Type::Ptr type_; 
    Scope* scope_;
    bool is_immutable_;
};

/* Scope structure for recording end-of-scope cleanup info */
class Scope : public Object {
public:
    Scope() : has_return_(0) {}

    void return_val(Operand val) { return_val_ = val; }
    void has_return(bool ret) { has_return_ = ret; }
    void variable(Variable* variable);
    Operand return_val() const { return return_val_; }
    bool has_return() const { return has_return_; }
    Variable* variable(String* name) const;
    Variable* variable(int index) const;
    int variables() const { return variable_.size(); }

    typedef Pointer<Scope> Ptr;

private:
    bool has_return_;
    Operand return_val_;
    std::vector<Variable::Ptr> variable_;
};
