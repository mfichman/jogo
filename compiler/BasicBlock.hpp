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

#pragma once

#include "Apollo.hpp"
#include "String.hpp"
#include <vector>

/* Operands for three-address code SSA instructions */
class Operand {
public:
    Operand(String* literal) : literal_(literal), temporary_(0) {}
    Operand() : temporary_(0) {}
    const Operand& operator++() { temporary_++; return *this; }
    String::Ptr literal() const { return literal_; }
    int temporary() const { return temporary_; }

private:
    String::Ptr literal_;
    int temporary_;
};

inline std::ostream& operator<<(std::ostream& out, const Operand& op) {
    if (op.literal()) {
        out << "'" << op.literal() << "'";
    } else {
        out << "t" << op.temporary();
    }
    return out;
}

/* Enumeration of opcodes available to the TAC code */
enum Opcode { 
    ADD, SUB, MUL, DIV, ANDL, ORL, ANDB, ORB, PUSH, POP, LOAD, STORE, LI,
    NOTL, CALL, JUMP, BNE, BEQ, BEQZ, BNEQZ, RET, HALT
};


/* Class for three-address code instructions */
class Instruction {
public:
    Instruction(Opcode op, Operand result, Operand first, Operand second):
        opcode_(op),
        first_(first),
        second_(second),
        result_(result) {
    }

    Opcode opcode() const { return opcode_; }
    Operand first() const { return first_; }
    Operand second() const { return second_; }
    Operand result() const { return result_; }

private:
    Opcode opcode_;
    Operand first_;
    Operand second_;
    Operand result_;
};

/* Class for basic block nodes */
class BasicBlock : public Object {
public:
    BasicBlock* branch() const { return branch_; }
    BasicBlock* next() const { return next_; }
    String* label() const { return label_; }
    const Instruction& instr(size_t index) const { 
        return instrs_[index];
    }
    size_t length() const { return instrs_.size(); }
    bool terminated() const {
        if (instrs_.empty()) {
            return false;
        }
        Opcode op = instrs_.back().opcode();
        if (op == RET || op == JUMP || op == HALT) {
            return true;
        }
        return false;
    }
    void branch(BasicBlock* branch) { branch_ = branch; }
    void next(BasicBlock* branch) { next_ = branch; }
    void label(String* label) { label_ = label; }
    void instr(const Instruction& inst) { 
        instrs_.push_back(inst); 
    }
    void instr(Opcode op, Operand first, Operand second, Operand result) {
        instrs_.push_back(Instruction(op, first, second, result));
    }
    typedef Pointer<BasicBlock> Ptr; 

private:
    std::vector<Instruction> instrs_;
    BasicBlock::Ptr branch_;
    BasicBlock::Ptr next_;
    String::Ptr label_;
};


