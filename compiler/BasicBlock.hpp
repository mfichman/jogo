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
    Operand(String* label) : label_(label), temp_(0), addr_(0) {}
    Operand(Expression* literal) : literal_(literal), temp_(0), addr_(0) {}
    Operand() : temp_(0), addr_(0) {}
    Operand(int temp) : temp_(temp), addr_(0) {}
    static Operand addr(int addr) { 
        Operand out;
        out.addr_ = addr;
        return out;
    }

    const Operand& operator++() { temp_++; return *this; }
    Expression* literal() const { return literal_; }
    String* label() const { return label_; }
    int temp() const { return temp_; }
    int addr() const { return addr_; }

private:
    Expression::Ptr literal_;
    String::Ptr label_; // FixMe: Should not be necessary...needed for CALL
    int temp_;
    int addr_;
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
    if (op.addr()) {
        return out << "mem[" << op.addr() << "]";
    }
    if (op.temp() > 0) {
        return out << "t" << op.temp();
    }
    return out << "r" << -op.temp();
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
    void first(Operand first) { first_ = first; }
    void second(Operand second) { second_ = second; }
    void result(Operand result) { result_ = result; }

private:
    Opcode opcode_;
    Operand first_;
    Operand second_;
    Operand result_;
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
    void swap(BasicBlock* other) { instrs_.swap(other->instrs_); }
    void branch(BasicBlock* branch) { branch_ = branch; }
    void next(BasicBlock* branch) { next_ = branch; }
    void label(String* label) { label_ = label; }
    void instr(const Instruction& inst) { 
        instrs_.push_back(inst); 
    }
    void instr(Opcode op, Operand result, Operand first, Operand second) {
        instrs_.push_back(Instruction(op, result, first, second));
    }
    typedef Pointer<BasicBlock> Ptr; 

private:
    std::vector<Instruction> instrs_;
    BasicBlock::Ptr branch_;
    BasicBlock::Ptr next_;
    String::Ptr label_;
};


