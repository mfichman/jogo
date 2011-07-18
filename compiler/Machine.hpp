/*
 * Copyright (c) 2011 Matt Fichman
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
#include <vector>

class Register : public Object {
public:
    Register(const std::string& name, int id) :
        name_(name),
        id_(id) {
    }

    const std::string& name();
    int id();
    typedef Pointer<Register> Ptr;

private:
    std::string name_;
    int id_; 
};

class Machine : public Object {
public:
    Machine();
    Register* caller_reg(int index) const { return caller_reg_[index]; }
    Register* callee_reg(int index) const { return callee_reg_[index]; }
    Register* arg_reg(int index) const { return arg_reg_[index]; }
    Register* reg(int id) const { return reg_[id]; }
    int caller_regs() const { return caller_reg_.size(); }
    int callee_regs() const { return callee_reg_.size(); }
    int arg_regs() const { return arg_reg_.size(); }
    int regs() const { return reg_.size() - 1; }
    void caller_reg(Register* reg) { caller_reg_.push_back(reg); }
    void callee_reg(Register* reg) { callee_reg_.push_back(reg); }
    void arg_reg(Register* reg) { arg_reg_.push_back(reg); }
    Register* reg(const std::string& name);
    typedef Pointer<Machine> Ptr;

    static Machine::Ptr intel64();

private:
    std::vector<Register::Ptr> caller_reg_;
    std::vector<Register::Ptr> callee_reg_;
    std::vector<Register::Ptr> arg_reg_;
    std::vector<Register::Ptr> reg_;
};

