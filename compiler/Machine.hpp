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
#include <vector>

/* Represents a general-purpose register in a machine architecture */
class Register : public Object {
public:
    Register(const std::string& name, int id, bool allocatable) :
        name_(name),
        id_(id),
        allocatable_(allocatable) {
    }

    const std::string& name() { return name_; }
    int id() const { return id_; }
    bool allocatable() const { return allocatable_; }
    typedef Pointer<Register> Ptr;

private:
    std::string name_;
    int id_; 
    bool allocatable_;
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
    // Caller reg = belongs to caller, must be saved before use
    // Calee reg = belongs to callee, must be saved before call
    
    Machine();
    Register* caller_reg(int index) const { return caller_reg_[index]; }
    Register* callee_reg(int index) const { return callee_reg_[index]; }
    Register* arg_reg(int index) const { return arg_reg_[index]; }
    Register* return_reg(int index) const { return return_reg_[index]; }
    Register* reg(int id) const { return reg_[id]; }
    int caller_regs() const { return caller_reg_.size(); }
    int callee_regs() const { return callee_reg_.size(); }
    int arg_regs() const { return arg_reg_.size(); }
    int return_regs() const { return return_reg_.size(); }
    int regs() const { return reg_.size(); }
    int word_size() const { return word_size_; }
    void caller_reg(Register* reg) { caller_reg_.push_back(reg); }
    void callee_reg(Register* reg) { callee_reg_.push_back(reg); }
    void arg_reg(Register* reg) { arg_reg_.push_back(reg); }
    void return_reg(Register* reg) { return_reg_.push_back(reg); }
    Register* reg(const std::string& name, bool allocatable=true);
    void word_size(int size) { word_size_ = size; }
    typedef Pointer<Machine> Ptr;

    static Machine* intel64();

private:
    std::vector<Register::Ptr> caller_reg_;
    std::vector<Register::Ptr> callee_reg_;
    std::vector<Register::Ptr> arg_reg_;
    std::vector<Register::Ptr> return_reg_;
    std::vector<Register::Ptr> reg_;
    int word_size_;
};

