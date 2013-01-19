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
    reg_.resize(1);
}

Register* Machine::reg(std::string const& name, RegisterId id) {
    assert("Register already exists"&&!reg(name));
    Register::Ptr newreg(new Register(name, id));
    regmap_.insert(std::make_pair(name, newreg));
    reg_.resize(id.id()+1);
    return reg_[id.id()] = newreg;
}

Register* Machine::int_reg(std::string const& name) {
    RegisterId id(reg_.size(), 0);
    return reg(name, id);
}

Register* Machine::special_reg(std::string const& name) {
    RegisterId id(reg_.size(), RegisterId::SPECIAL);
    return reg(name, id);
}

Register* Machine::float_reg(std::string const& name) {
    RegisterId id(reg_.size(), RegisterId::FLOAT);
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
    Machine::Ptr static m;
    
    if (m) { return m; }

    m = new Machine;

    // Volatile/temporary register = callee reg. Not saved by the callee, so
    // not safe to be live across function calls
    
    Register::Ptr rax = m->int_reg("rax"); m->callee_reg(rax); //2 VOL
    Register::Ptr rcx = m->int_reg("rcx"); m->callee_reg(rcx); //4 VOL
    Register::Ptr rdx = m->int_reg("rdx"); m->callee_reg(rdx); //5 VOL
    Register::Ptr rbx = m->int_reg("rbx"); m->caller_reg(rbx); //3

    Register::Ptr rsp = m->special_reg("rsp"); m->sp_reg(rsp); // 1
    Register::Ptr rbp = m->special_reg("rbp");

#ifdef WINDOWS
    Register::Ptr rsi = m->int_reg("rsi"); m->caller_reg(rsi); //7
    Register::Ptr rdi = m->int_reg("rdi"); m->caller_reg(rdi); //6
#else
    Register::Ptr rsi = m->int_reg("rsi"); m->callee_reg(rsi); //7
    Register::Ptr rdi = m->int_reg("rdi"); m->callee_reg(rdi); //6
#endif
    Register::Ptr r8 = m->int_reg("r8"); m->callee_reg(r8); //8 VOL
    Register::Ptr r9 = m->int_reg("r9"); m->callee_reg(r9); //9 VOL
    Register::Ptr r10 = m->int_reg("r10"); m->callee_reg(r10);//10 VOL
    Register::Ptr r11 = m->int_reg("r11"); m->callee_reg(r11); //11 VOL
    Register::Ptr r12 = m->special_reg("r12"); m->caller_reg(r12); //12
    // FIXME: R12 has some weird behavior when indirect addressing is used.  To
    // simplify code generation, don't use R12.  When the code generator is more
    // sophisticated, this can be re-enabled.
    Register::Ptr r13 = m->int_reg("r13"); m->caller_reg(r13); //13
    Register::Ptr r14 = m->int_reg("r14"); m->caller_reg(r14); //14
    Register::Ptr r15 = m->int_reg("r15"); m->caller_reg(r15); //15

    Register::Ptr xmm0 = m->float_reg("xmm0"); m->callee_reg(xmm0);
    // Use xmm0 as an accumulator, like rax for integer instructions
    Register::Ptr xmm1 = m->float_reg("xmm1"); m->callee_reg(xmm1);
    Register::Ptr xmm2 = m->float_reg("xmm2"); m->callee_reg(xmm2);
    Register::Ptr xmm3 = m->float_reg("xmm3"); m->callee_reg(xmm3);
    Register::Ptr xmm4 = m->float_reg("xmm4"); m->callee_reg(xmm4);
    Register::Ptr xmm5 = m->float_reg("xmm5"); m->callee_reg(xmm5);
    Register::Ptr xmm6 = m->float_reg("xmm6"); m->callee_reg(xmm6);
    Register::Ptr xmm7 = m->float_reg("xmm7"); m->callee_reg(xmm7);
    Register::Ptr xmm8 = m->float_reg("xmm8"); m->callee_reg(xmm8);
    Register::Ptr xmm9 = m->float_reg("xmm9"); m->callee_reg(xmm9);
    Register::Ptr xmm10 = m->float_reg("xmm10"); m->callee_reg(xmm10);
    Register::Ptr xmm11 = m->float_reg("xmm11"); m->callee_reg(xmm11);
    Register::Ptr xmm12 = m->float_reg("xmm12"); m->callee_reg(xmm12);
    Register::Ptr xmm13 = m->float_reg("xmm13"); m->callee_reg(xmm13);
    Register::Ptr xmm14 = m->float_reg("xmm14"); m->callee_reg(xmm14);
    Register::Ptr xmm15 = m->float_reg("xmm15"); m->callee_reg(xmm15);
    // Disabled b/c NASM doesn't support xmm8-xmm15
    
    m->return_reg(rax); // Return reg isn't used as a gp reg?
#ifdef WINDOWS
    m->arg_reg(rcx);
    m->arg_reg(rdx);
    m->arg_reg(r8);
    m->arg_reg(r9);
#else
    m->arg_reg(rdi);
    m->arg_reg(rsi);
    m->arg_reg(rdx);
    m->arg_reg(rcx);
    m->arg_reg(r8);
    m->arg_reg(r9);
#endif
    
    m->return_reg(xmm0);
    m->arg_reg(xmm0);
    m->arg_reg(xmm1);
    m->arg_reg(xmm2);
    m->arg_reg(xmm3);
    m->arg_reg(xmm4);
    m->arg_reg(xmm5);
    m->arg_reg(xmm6);
    m->arg_reg(xmm7);

    m->word_size(8); // 64 bits = 8 bytes
    m->init();
    return m;
}

Register* Machine::reg(RegisterId id) const {
    return id.id() < reg_.size() ? reg_[id.id()].pointer() : 0; 
}

void Machine::init() {
    arg_set_ = RegisterIdSet(regs());
    for (int k = 0; k < int_arg_regs(); k++) {
        arg_set_.set(int_arg_reg(k)->id());
    }
    for (int k = 0; k < float_arg_regs(); k++) {
        arg_set_.set(float_arg_reg(k)->id());
    }

    return_set_ = RegisterIdSet(regs());
    for (int k = 0; k < int_return_regs(); k++) {
        return_set_.set(int_return_reg(k)->id());
    }
    for (int k = 0; k < float_return_regs(); k++) {
        return_set_.set(float_return_reg(k)->id());
    }

    caller_set_ = RegisterIdSet(regs());
    for (int k = 0; k < caller_regs(); k++) {
        caller_set_.set(caller_reg(k)->id());
    }

    callee_set_ = RegisterIdSet(regs());
    for (int k = 0; k < callee_regs(); k++) {
        callee_set_.set(callee_reg(k)->id());
    }
}

Register* Machine::caller_reg(int index) const { 
    if (index >= caller_reg_.size()) { return 0; }
    return caller_reg_[index]; 
}

Register* Machine::callee_reg(int index) const { 
    if (index >= callee_reg_.size()) { return 0; }
    return callee_reg_[index]; 
}

Register* Machine::int_arg_reg(int index) const { 
    if (index >= int_arg_reg_.size()) { return 0; }
    return int_arg_reg_[index]; 
}

Register* Machine::int_return_reg(int index) const { 
    if (index >= int_return_reg_.size()) { return 0; }
    return int_return_reg_[index]; 
}

Register* Machine::float_arg_reg(int index) const { 
    if (index >= float_arg_reg_.size()) { return 0; }
    return float_arg_reg_[index];
}

Register* Machine::float_return_reg(int index) const {  
    if (index >= float_return_reg_.size()) { return 0; }
    return float_return_reg_[index]; 
}
