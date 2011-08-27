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

#include "Apollo.hpp"
#include "Environment.hpp"
#include "BasicBlock.hpp"
#include "Object.hpp"
#include "Machine.hpp"
#include "Stream.hpp"
#include <iostream>
#include <fstream>
#include <set>

/* Generates code for Intel 64 machines */
class Intel64CodeGenerator : public TreeNode::Functor {
public:
    Intel64CodeGenerator(Environment* env, const std::string& file);
    typedef Pointer<Intel64CodeGenerator> Ptr;
    
private:
    void operator()(Class* feature);
    void operator()(Module* feature);
    void operator()(Function* function);
    void operator()(BasicBlock* block);
    void emit(const char* instr, Register* r1);
    void emit(const char* instr, Operand r1);
    void emit(const char* instr, Register* r1, Operand r2);
    void emit(const char* instr, Operand r1, Register* r2);
    void emit(const char* instr, Operand r1, Operand r2);
    void emit(const char* instr);
    void emit(const char* instr, String* label);
    void emit(const char* instr, Operand r1, const char* imm);
    void arith(const char* instr, Operand res, Operand r1, Operand r2);
    void load(Operand r1, Operand r2);
    void store(Operand r1, Operand r2);
    void literal(Operand lit);
    void string(String* string);

    Environment::Ptr environment_;
    Machine::Ptr machine_;
    Stream::Ptr out_;
};
