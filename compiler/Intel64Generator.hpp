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
    bool is_extended_reg(RegisterId id) const;
    void dispatch_table(Class* feature);
    void operator()(File* file);
    void operator()(Class* feature);
    void operator()(Function* feature);
    void operator()(IrBlock* block);
    void out(Stream::Ptr out) { out_ = out; }
    void format(OutputFormat::Ptr format);

    static RegisterId const RAX;
    static RegisterId const RSP;
    static RegisterId const RBP;
    static uint8_t const MOV_RM_REG = 0x89;
    static uint8_t const MOV_REG_RM = 0x8b;
    static uint8_t const MOV_IMM = 0xb8;
    static uint8_t const MODRM_DIRECT = 0xc0;
    static uint8_t const MODRM_DISP8 = 0x40; // 0100 0000
    static uint8_t const MODRM_DISP32 = 0x80; // 1000 0000
    static uint8_t const MODRM_INDIRECT = 0x00; // 1000 0000
    static uint8_t const MODRM_REG = 0x38;
    static uint8_t const MODRM_RM = 0x07;
    static uint8_t const NOP = 0x90;
    static uint8_t const REX_PREFIX = 0x40;
    static uint8_t const REX_W = 0x08; // 64-bit operand 
    static uint8_t const REX_R = 0x04; // MODRM.reg extension
    static uint8_t const REX_X = 0x02; // ??
    static uint8_t const REX_B = 0x01; // MODRM.rm extension

private:
    void string(String::Ptr lit);
    void instr(uint8_t op, uint8_t ext, RegisterId reg, uint32_t imm);
    void instr(uint8_t op, uint8_t ext, RegisterId reg);
    void instr(uint8_t op, RegisterId reg, RegisterId rm);
    void instr(uint8_t op, RegisterId reg, String* label);
    void instr(uint8_t op, RegisterId reg, Operand mem);
    void load(RegisterId res, Operand a1);
    void store(Operand a1, Operand a2);
    void arith(Instruction const& instr);

    void mov(RegisterId reg, RegisterId rm);
    void mov(RegisterId reg, Operand rm);
    void mov(Operand rm, RegisterId reg);
    void mov(RegisterId dst, uint64_t imm);
    void mov(RegisterId dst, String* label);
    void mov(String* label, RegisterId dst);
    void push(RegisterId reg);
    void pop(RegisterId reg);
    void cmp(RegisterId arg1, RegisterId arg2);
    void test(RegisterId arg1, RegisterId arg2);
    void jne(String* label);
    void je(String* label);
    void jg(String* label);
    void jge(String* label);
    void jl(String* label);
    void jle(String* label);
    void jz(String* label);
    void jnz(String* label);
    void jmp(String* label);
    void call(Operand target);
    void movsd(RegisterId dst, RegisterId src);
    void lea(RegisterId dst, Operand op);
    void bnot(RegisterId src);
    void band(RegisterId dst, RegisterId src);
    void bor(RegisterId dst, RegisterId src);
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
    void sub(RegisterId dst, uint64_t imm);
    void imul(RegisterId dst, RegisterId src);
    void div(RegisterId dst, RegisterId src);
    void neg(RegisterId reg);

private:
    Environment::Ptr env_;
    Stream::Ptr out_;
    OutputFormat::Ptr format_;
    Machine::Ptr machine_;
    Section::Ptr text_;
    Section::Ptr data_;
    Function::Ptr function_;
    std::set<String*> string_;
};
