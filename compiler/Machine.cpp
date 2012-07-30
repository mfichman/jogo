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

Machine::Machine() : word_size_(0) {
}

Register* Machine::reg(std::string const& name, RegisterId id) {
    Register* reg = new Register(name, id);
    regs_ = append(regs_, reg);
    reg_[id] = reg;
    return reg;
}

Register* Machine::int_reg(std::string const& name) {
    RegisterId id(reg_.size()+1, RegisterId::COLORED);
    return reg(name, id);
}

Register* Machine::float_reg(std::string const& name) {
    RegisterId id(reg_.size()+1, RegisterId::COLORED|RegisterId::FLOAT);
    return reg(name, id);
}

void Machine::arg_reg(Register* reg) {
    if (reg->is_float()) { 
        float_arg_reg_.push_back(reg);
    } else {
        int_arg_reg_.push_back(reg);
    }
}

void Machine::return_reg(Register* reg) {
    if (reg->is_float()) { 
        float_return_reg_.push_back(reg);
    } else {
        int_return_reg_.push_back(reg);
    }
}

Machine* Machine::intel64() {
    // Creates an Intel64 machine description (a.k.a. AMD64, x86-64, but my
    // dad worked at Intel for 18 years, so in this compiler, we're callin it
    // Intel64!).
    Machine::Ptr static machine;
    
    if (machine) { return machine; }

    machine = new Machine;

    // Volatile = callee reg
    
    Register::Ptr rax = machine->int_reg("rax"); machine->callee_reg(rax); //1 VOL
    Register::Ptr rbx = machine->int_reg("rbx"); machine->caller_reg(rbx); //2
    Register::Ptr rcx = machine->int_reg("rcx"); machine->callee_reg(rcx); //3 VOL
    Register::Ptr rdx = machine->int_reg("rdx"); machine->callee_reg(rdx); //4 VOL

#ifdef WINDOWS
    Register::Ptr rdi = machine->int_reg("rdi"); machine->caller_reg(rdi); //5
#else
    Register::Ptr rdi = machine->int_reg("rdi"); machine->callee_reg(rdi); //5
#endif
    Register::Ptr rsi = machine->int_reg("rsi"); machine->callee_reg(rsi); //6
    Register::Ptr r8 = machine->int_reg("r8"); machine->callee_reg(r8); //7 VOL
    Register::Ptr r9 = machine->int_reg("r9"); machine->callee_reg(r9); //8 VOL
    Register::Ptr r10 = machine->int_reg("r10"); machine->callee_reg(r10);//9 VOL
    Register::Ptr r11 = machine->int_reg("r11"); machine->callee_reg(r11); //10 VOL
    Register::Ptr r12 = machine->int_reg("r12"); machine->caller_reg(r12); //11
    Register::Ptr r13 = machine->int_reg("r13"); machine->caller_reg(r13); //12
    Register::Ptr r14 = machine->int_reg("r14"); machine->caller_reg(r14); //13
    Register::Ptr r15 = machine->int_reg("r15"); machine->caller_reg(r15); //14

    machine->return_reg(rax);

#ifdef WINDOWS
    machine->arg_reg(rcx);
    machine->arg_reg(rdx);
    machine->arg_reg(r8);
    machine->arg_reg(r9);
#else
    machine->arg_reg(rdi);
    machine->arg_reg(rsi);
    machine->arg_reg(rdx);
    machine->arg_reg(rcx);
    machine->arg_reg(r8);
    machine->arg_reg(r9);
#endif

    machine->word_size(8); // 64 bits = 8 bytes

    return machine;
}
