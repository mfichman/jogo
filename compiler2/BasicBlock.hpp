/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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

#ifndef BASIC_BLOCK_HPP
#define BASIC_BLOCK_HPP

#include "Apollo.hpp"
#include <vector>

/* Class for instructions */
class Instruction {
public:
    enum Opcode { ADD, SUB, MUL, DIV, AND, OR, LOAD, STORE };

    Instruction(Opcode opcode, int t1, int t2, int t3) :
        opcode_(opcode),
        operand1_(t1),
        operand2_(t2),
        result_(t3) {
    }
    
    Opcode opcode() const { return opcode_; }
    int operand1() const { return operand1_; }
    int operand2() const { return operand2_; }
    int result() const { return result_; }

private:
    Opcode opcode_;
    int operand1_;
    int operand2_;
    int result_;
};

/* Class for basic block nodes */
class BasicBlock : public Object {
public:
    enum ControlCode { CALL, JUMP, BNE, BEQ, BEQZ, RETURN, HALT };

    BasicBlock(Name* label) :
        control_code_(HALT),
        label_(label)  {
    }

    BasicBlock* branch1() const { return branch1_; }
    BasicBlock* branch2() const { return branch2_; }
    ControlCode control_code() const { return control_code_; }
    Name* label() const { return label_; }
    const Instruction& instruction(size_t index) const { 
        return instructions_[index];
    }
    size_t instruction_count() const { return instructions_.size(); }
    void branch1(BasicBlock* branch) { branch1_ = branch; }
    void branch2(BasicBlock* branch) { branch2_ = branch; }
    void control_code(ControlCode code) { control_code_ = code; }
    void instruction(const Instruction& inst) { 
        instructions_.push_back(inst); 
    }
    typedef Pointer<BasicBlock> Ptr; 

private:
    std::vector<Instruction> instructions_;
    BasicBlock::Ptr branch1_;
    BasicBlock::Ptr branch2_;
    ControlCode control_code_;
    Name::Ptr label_;
};


#endif

