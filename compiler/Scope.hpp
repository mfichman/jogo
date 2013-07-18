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

#include "Jogo.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "IrBlock.hpp"
#include <vector>
#include <map>

class Scope;

/* Variable structure, used to store info about assigned vars in a scope */
class Variable : public Object {
public:
    Variable(String* name, Type* type, bool immut=false) :
        name_(name),
        type_(type),
        scope_(0),
        is_immutable_(immut) {
    }

    String* name() const { return name_; }
    Type* type() const { return type_; } // null == no cleanup on stack unwind
    Scope* scope() const { return scope_; }
    bool is_immutable() const { return is_immutable_; }
    void scope(Scope* scope) { scope_ = scope; }
    typedef Pointer<Variable> Ptr;

private:
    String::Ptr name_;
    Type::Ptr type_; 
    Scope* scope_;
    bool is_immutable_;
};

/* Scope structure for recording end-of-scope cleanup info */
class Scope : public Object {
public:
    void variable(Variable* variable);
    Variable* variable(String* name) const;
    Variable* variable(int index) const;
    int variables() const { return variable_.size(); }

    typedef Pointer<Scope> Ptr;
private:
    std::vector<Variable::Ptr> variable_;
};
