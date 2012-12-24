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
#include "OutputFormat.hpp"
#include "IrBlock.hpp"
#include "Environment.hpp"
#include "Stream.hpp"
#include "Machine.hpp"

/* Emits Intel64 (x86-64) machine code */
class Intel64Generator : public TreeNode::Functor {
public:
    Intel64Generator(Environment* env);
    typedef Pointer<Intel64Generator> Ptr;
    Stream::Ptr out() const { return out_; }
    void operator()(File* file);
    void operator()(Class* feature);
    void operator()(Module* feature);
    void operator()(Function* feature);
    void operator()(IrBlock* block);
    void out(Stream::Ptr out) { out_ = out; }
    void format(OutputFormat::Ptr format) { format_ = format; }

private:
    void mov(Operand dst, Operand src);
    void push(RegisterId reg);
    void pop(RegisterId reg);
    void cmp(RegisterId arg1, RegisterId arg2);
    void jne(Operand op);
    void je(Operand op);
    void jg(Operand op);
    void jge(Operand op);
    void jl(Operand op);
    void jle(Operand op);
    void call(Operand op);
    void jmp(Operand op);
    void movsd(RegisterId dst, RegisterId src);
    void lea(RegisterId dst, Operand op);
    void bnot(RegisterId src);
    void band(RegisterId src);
    void bor(RegisterId src);
    void nop();
    void leave();
    void ret();
    void cqo();
    void idiv(RegisterId reg);
    void addsd(RegisterId dst, RegisterId src);
    void subsd(RegisterId dst, RegisterId src);
    void mulsd(RegisterId dst, RegisterId src);
    void divsd(RegisterId dst, RegisterId src);
    void add(RegisterId dst, RegisterId src);
    void sub(RegisterId dst, RegisterId src);
    void imul(RegisterId dst, RegisterId src);
    void div(RegisterId dst, RegisterId src);
    void neg(RegisterId reg);

private:
    Environment::Ptr env_;
    Stream::Ptr out_;
    OutputFormat::Ptr format_;
    Machine::Ptr machine_;
};
