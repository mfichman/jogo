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
#include "IrBlock.hpp"
#include "Object.hpp"
#include "Machine.hpp"
#include "Stream.hpp"
#include <iostream>
#include <fstream>
#include <set>

/* Generates code for Intel 64 machines */
class Nasm64Generator : public TreeNode::Functor {
public:
    Nasm64Generator(Environment* env);
    typedef Pointer<Nasm64Generator> Ptr;

    Stream* out() const { return out_; }
    void out(Stream* out) { out_ = out; }
    void operator()(File* file);

private:
    void operator()(Class* feature);
    void operator()(Function* function);
    void operator()(IrBlock* block);
    void string(String* string);
    void instr(const char* instr, Operand r1);
    void instr(const char* instr, Operand r1, Operand r2);
    void instr(const char* instr);
    void instr(const char* instr, Operand r1, const char* label);

    void dispatch_table(Class* feature);
    void neg(Operand r1, Operand r2);
    void arith(Instruction const& instr);
    void load(Operand r1, Operand r2);
    void store(Operand r1, Operand r2);

    void be(RegisterId a1, RegisterId a2, String* label);
    void bne(RegisterId a1, RegisterId a2, String* label);
    void bz(RegisterId a1, String* label);
    void bnz(RegisterId a1, String* label);
    void bg(RegisterId a1, RegisterId a2, String* label);
    void bge(RegisterId a1, RegisterId a2, String* label);
    void bl(RegisterId a1, RegisterId a2, String* label);
    void ble(RegisterId a1, RegisterId a2, String* label);

    void operand(Operand op);
    void reg(Operand op);
    void addr(Operand addr);
    void literal(Operand lit);
    void label(Operand string);
    void label(std::string const& string);
    void align();

    void store_hack(Operand a1, Operand a2);
    void load_hack(Operand res, Operand a1);


    Environment::Ptr env_;
    Machine::Ptr machine_;
    Class::Ptr class_;
    Stream::Ptr out_;
    Opcode opcode_;
    std::set<std::string> symbol_;
    std::set<std::string> definition_;
};
