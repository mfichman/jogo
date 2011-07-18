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

#include "Machine.hpp"

Machine::Machine() {
    reg_.push_back(0); // Zeroth temporary name is always null
}

Register* Machine::reg(const std::string& name) {
    reg_.push_back(new Register(name, reg_.size()));
    return reg_.back();
}

Machine::Ptr Machine::intel64() {
    // Creates an Intel64 machine description (a.k.a. AMD64, x86-64, but my
    // dad worked at Intel for 18 years, so in this compiler, we're callin it
    // Intel64!).
    static Machine::Ptr machine;
    
    if (machine) { return machine; }

    machine = new Machine;

    Register::Ptr rbx = machine->reg("rbx"); machine->caller_reg(rbx);
    Register::Ptr rcx = machine->reg("rcx"); machine->callee_reg(rcx);
    Register::Ptr rdx = machine->reg("rdx"); machine->callee_reg(rdx);
    Register::Ptr rdi = machine->reg("rdi"); machine->callee_reg(rdi);
    Register::Ptr rsi = machine->reg("rsi"); machine->callee_reg(rsi);
    Register::Ptr r8 = machine->reg("r8"); machine->callee_reg(r8);
    Register::Ptr r9 = machine->reg("r9"); machine->callee_reg(r9);
    Register::Ptr r10 = machine->reg("r10"); machine->callee_reg(r10);
    Register::Ptr r11 = machine->reg("r11"); machine->callee_reg(r11);
    Register::Ptr r12 = machine->reg("r12"); machine->caller_reg(r12);
    Register::Ptr r13 = machine->reg("r13"); machine->caller_reg(r13);
    Register::Ptr r14 = machine->reg("r14"); machine->caller_reg(r14);
    Register::Ptr r15 = machine->reg("r15"); machine->caller_reg(r15);
        
    machine->arg_reg(rdi);
    machine->arg_reg(rsi);
    machine->arg_reg(rdx);
    machine->arg_reg(rcx);
    machine->arg_reg(r8);
    machine->arg_reg(r9);

    return machine;
}
