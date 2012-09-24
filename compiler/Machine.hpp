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
#include "Stream.hpp"
#include "BasicBlock.hpp"
#include <vector>

/* Represents a general-purpose register in a machine architecture */
class Register : public Object {
public:
    Register(std::string const& name, RegisterId id) :
        name_(name),
        id_(id) {
    }

    std::string const& name() { return name_; }
    RegisterId id() const { return id_; }
    bool is_float() const { return id_.is_float(); }
    bool is_int() const { return id_.is_int(); }
    typedef Pointer<Register> Ptr;

private:
    std::string name_;
    RegisterId id_; 
};

inline Stream::Ptr operator<<(Stream::Ptr out, Register* reg) {
    return out << reg->name();
}

/* Records the different registers available for a machine */
class Machine : public Object {
public:
    // Note: Caller registers (a.k.a. callee-saved) belong to the caller and
    // must be saved/restored by the callee if they are used.  To do this,
    // the register allocator generates a phony "def" at the beinning of the
    // function, and a phony "use" at the end of the function to generate a 
    // def-use chain. 
    // 
    // Note: Callee registers (a.k.a. caller-saved) belong to the callee and
    // must be saved/restored by the caller if they are to be used.  To do
    // this, the allocator adds an edge to the interference graph for each
    // callee register that is live during a CALL instruction.
    // 
    // Note: The zero-id register is ALWAYS invalid.  It is not a real
    // register, and the allocator does not assign temporaries to it.
    //
    // Caller reg = belongs to caller, must be saved before use, nonvolatile
    // Calee reg = belongs to callee, must be saved before call, volatile
    
    Machine();
    Register* caller_reg(int index) const { return caller_reg_[index]; }
    Register* callee_reg(int index) const { return callee_reg_[index]; }
    Register* int_arg_reg(int index) const { return int_arg_reg_[index]; }
    Register* int_return_reg(int index) const { return int_return_reg_[index]; }
    Register* float_arg_reg(int index) const { return float_arg_reg_[index]; }
    Register* float_return_reg(int index) const { return float_arg_reg_[index]; }
    Register* sp_reg() const { return sp_reg_; }
    Register* float_reg(std::string const& name);
    Register* int_reg(std::string const& name);
    Register* reg(std::string const& name, RegisterId id);
    Register* reg(RegisterId id) const;
    int regs() const { return reg_.size(); }
    int caller_regs() const { return caller_reg_.size(); }
    int callee_regs() const { return callee_reg_.size(); }
    int int_arg_regs() const { return int_arg_reg_.size(); }
    int int_return_regs() const { return int_return_reg_.size(); }
    int float_arg_regs() const { return float_arg_reg_.size(); }
    int float_return_regs() const { return float_return_reg_.size(); }
    int word_size() const { return word_size_; }
    RegisterIdSet const& arg_set() const { return arg_set_; }
    RegisterIdSet const& return_set() const { return return_set_; }
    RegisterIdSet const& caller_set() const { return caller_set_; } 
    void caller_reg(Register* reg) { caller_reg_.push_back(reg); }
    void callee_reg(Register* reg) { callee_reg_.push_back(reg); }
    void arg_reg(Register* reg);
    void return_reg(Register* reg);
    void sp_reg(Register* reg) { sp_reg_ = reg; }
    void word_size(int size) { word_size_ = size; }
    typedef Pointer<Machine> Ptr;

    static Machine* intel64();

private:
    void init();
    std::vector<Register::Ptr> caller_reg_;
    std::vector<Register::Ptr> callee_reg_;
    std::vector<Register::Ptr> int_arg_reg_;
    std::vector<Register::Ptr> int_return_reg_;
    std::vector<Register::Ptr> float_arg_reg_;
    std::vector<Register::Ptr> float_return_reg_;
    std::vector<Register::Ptr> reg_;
    Register::Ptr sp_reg_;
    int word_size_;
    RegisterIdSet arg_set_;
    RegisterIdSet return_set_;
    RegisterIdSet caller_set_;
};

