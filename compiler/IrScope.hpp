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
#include "Environment.hpp"
#include "Object.hpp"
#include "IrBlock.hpp"
#include <vector>
#include <map>

/* Represents a variable name/value pair */
class IrVariable  {
public:
    IrVariable() : name_(0), value_(0) {}
    IrVariable(String* name, IrValue* val) :
        name_(name),
        value_(val) {

        assert("Literal or label in variable" && !val->operand().object());
        assert("Address in variable" && !val->operand().addr());
    } 
    IrValue* value() const { return value_; }
    String* name() const { return name_; }

private:
    String::Ptr name_;
    IrValue::Ptr value_;
};


/* Keeps track of variables in the current scope */
class IrScope : public Object {
public:
    ~IrScope();
    void variable(IrVariable const& var) { variable_.push_back(var); }
    void ret(IrValue* ret) { ret_ = ret; }
    IrVariable const& variable(int index) const { return variable_[index]; }
    IrValue* variable(String* name) const;
    IrValue* ret() const { return ret_; }
    int variables() const { return variable_.size(); }

    typedef Pointer<IrScope> Ptr;
private:
    IrValue::Ptr ret_;
    std::vector<IrVariable> variable_;
};

