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
#include "String.hpp"
#include "Expression.hpp"
#include <vector>

/* Operands for three-address code SSA instructions */
class Operand {
public:
    Operand(String* label) : label_(label), temporary_(0) {}
    Operand(Expression* literal) : literal_(literal), temporary_(0) {}
    Operand() : temporary_(0) {}
    Operand(int temporary) : temporary_(temporary) {}
    const Operand& operator++() { temporary_++; return *this; }
    Expression* literal() const { return literal_; }
    String* label() const { return label_; }
    int temporary() const { return temporary_; }

private:
    Expression::Ptr literal_;
    String::Ptr label_;
    int temporary_;
    // If this is negative, then the operand is pre-assigned to a real machine
    // register.  Normally, this only hapens for function parameter passing by
    // register.
};

inline std::ostream& operator<<(std::ostream& out, const Operand& op) {
    if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(op.literal())) {
        return out << le->value();
    }
    if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(op.literal())) {
        return out << le->value();
    }
    if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(op.literal())) {
        return out << le->value();
    }
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(op.literal())) {
        return out << "'" << le->value() << "'";
    }
    if (op.temporary() > 0) {
        return out << "t" << op.temporary();
    }
    return out << "r" << -op.temporary();
}

/* Enumeration of opcodes available to the TAC code */
enum Opcode { 
    MOV, ADD, SUB, MUL, DIV, ANDB, ORB, PUSH, POP, LOAD, STORE, NOTB,
    CALL, JUMP, BNE, BE, BNZ, BZ, BG, BL, BGE, BLE, RET, NOP
    // Note: BNE through BLE must be contiguous
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
    int offset() const { return offset_; }
    void first(Operand first) { first_ = first; }
    void second(Operand second) { second_ = second; }
    void result(Operand result) { result_ = result; }
    void offset(int offset) { offset_ = offset; }

private:
    Opcode opcode_;
    Operand first_;
    Operand second_;
    Operand result_;
    int offset_;
};

/* Class for basic block nodes */
class BasicBlock : public Object {
public:
    BasicBlock() : branch_(0), next_(0) {}
    BasicBlock* branch() const { return branch_; }
    BasicBlock* next() const { return next_; }
    String* label() const { return label_; }
    const Instruction& instr(size_t index) const { 
        return instrs_[index];
    }
    Instruction& instr(size_t index) {
        return instrs_[index];
    }
    int instrs() const { return instrs_.size(); }
    bool is_terminated() const {
        if (instrs_.empty()) {
            return false;
        }
        Opcode op = instrs_.back().opcode();
        if (op == RET || op == JUMP || (op >= BNE && op <= BLE)) {
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


