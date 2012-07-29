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
#include <set>

/* A register ID identifies a register by type and number. */
class RegisterId {
public:
    // A colored register has the COLORED flag set.  This means the register is
    // an actual hardware register.  A floating-point register has the FLOAT
    // flag set.
    static int const FLOAT = 0x8000;
    static int const COLORED = 0x4000;

    RegisterId() : id_(0), flags_(0) {}
    RegisterId(int id, int flags) : id_(id), flags_(flags) {
        assert(id < std::numeric_limits<short>::max());
    }

    bool is_colored() const { return flags_ & COLORED; }
    bool is_int() const { return !is_float(); }
    bool is_float() const { return flags_ & FLOAT; }
    bool is_valid() const { return !!id_; }
    int id() const { return id_; }
    bool operator!() const { return !is_valid(); }
    bool operator==(const RegisterId& id) const; 
    bool operator!=(const RegisterId& id) const;
    bool operator<(const RegisterId& id) const;
    
private:
    short id_;
    short flags_;
};

/* Represents a 'logical' address, expressed in words (not bytes) */
class Address {
public:
    Address() : value_(0), valid_(false) {}
    explicit Address(int value) : value_(value), valid_(true) {
        assert(value < std::numeric_limits<short>::max());
    }

    int value() const { return value_; }
    bool operator!() const { return !valid_; }
    bool operator==(const Address& ad) const { return value_ == ad.value_; }
    bool operator!=(const Address& ad) const { return value_ != ad.value_; }

private:
    short value_;
    bool valid_;

};

/* Operands for three-address code SSA instructions */
class Operand {
public:
    Operand() {}
    Operand(String* label) : obj_(label) {}
    // Represents the label's address, e.g., LABEL.  Used for CALL instructions.
    Operand(String* label, Address addr) : obj_(label), addr_(addr) {}
    // Represents the contents of label (with offset), i.e., MEM[LABEL+ADDR].
    Operand(Expression* literal) : obj_(literal) {}
    // Represents the value of a literal, i.e., LITERAL.
    Operand(RegisterId reg) : reg_(reg) {}
    // Represents the contents of a register, e.g., REG.
    Operand(RegisterId reg, Address addr) : reg_(reg), addr_(addr) {}
    // Represents the contents of a memory location, e.g., MEM[ADDR+REG]
    Operand(Address addr) : addr_(addr) {}
    // Represents the contents of a memory address, e.g., MEM[ADDR]

    //Operand static addr(Address addr);
    //Operand static addr(int reg, Address addr);

    Expression* literal() const { return dynamic_cast<Expression*>(obj_.pointer()); }
    String* label() const { return dynamic_cast<String*>(obj_.pointer()); }
    RegisterId reg() const { return reg_; }
    Address addr() const { return addr_; }
    bool is_indirect() const { return !!addr_; }
    bool is_colored() const { return reg_.is_colored(); }
    bool operator==(const Operand& other) const;
    bool operator!=(const Operand& other) const;
    bool operator!() const { return !obj_ && !reg_ && !addr_; }
    void reg(RegisterId reg) { reg_ = reg; }

private:
    Pointer<Object> obj_; // Literal expression or string label
    RegisterId reg_; // Temporary variable ID (negative = machine register)
    Address addr_; // Address offset (in words)
};

Stream::Ptr operator<<(Stream::Ptr out, const Operand& op);

/* Enumeration of opcodes available to the TAC code */
enum Opcode { 
    MOV, ADD, SUB, MUL, DIV, NEG, ANDB, ORB, PUSH, POP, LOAD, STORE, NOTB,
    CALL, JUMP, BNE, BE, BNZ, BZ, BG, BL, BGE, BLE, RET, NOP, POPN 
    // Note: BNE through BLE must be contiguous
};

/* Class for liveness information related to the instruction */
class Liveness : public Object {
public:
    std::set<RegisterId> const& in() const { return in_; }
    std::set<RegisterId> const& out() const { return out_; }
    std::set<RegisterId>& in() { return in_; }
    std::set<RegisterId>& out() { return out_; }
    typedef Pointer<Liveness> Ptr;

private:
    std::set<RegisterId> in_; // Live variables on incoming edge
    std::set<RegisterId> out_; // Live variables on outgoing edge
};

/* Class for three-address code instructions */
class Instruction {
public:
    Instruction(Opcode op, Operand result, Operand first, Operand second) :
        opcode_(op),
        first_(first),
        second_(second),
        result_(result),
        liveness_(new Liveness) {
    }

    Opcode opcode() const { return opcode_; }
    Operand const& first() const { return first_; }
    Operand const& second() const { return second_; }
    Operand result() const { return result_; }
    Liveness* liveness() const { return liveness_; }
    void opcode(Opcode opcode) { opcode_ = opcode; }
    void first(Operand first) { first_ = first; }
    void second(Operand second) { second_ = second; }
    void result(Operand result) { result_ = result; }

private:
    Opcode opcode_;
    Operand first_;
    Operand second_;
    Operand result_;
    Liveness::Ptr liveness_;
};

/* Class for basic block nodes */
class BasicBlock : public Object {
public:
    BasicBlock() : branch_(0), next_(0), round_(0) {}
    BasicBlock* branch() const { return branch_; }
    BasicBlock* next() const { return next_; }
    String* label() const { return label_; }
    Instruction const& instr(size_t index) const { return instrs_[index]; }
    Instruction& instr(size_t index) { return instrs_[index]; }
    int round() const { return round_; }
    int instrs() const { return instrs_.size(); }
    bool is_terminated() const;
    bool is_ret() const;
    void swap(BasicBlock* other) { instrs_.swap(other->instrs_); }
    void branch(BasicBlock* branch) { branch_ = branch; }
    void next(BasicBlock* branch) { next_ = branch; }
    void label(String* label) { label_ = label; }
    void instr(const Instruction& inst);
    void instr(Opcode op, Operand res, Operand one, Operand two);
    void round_inc() { round_++; }
    typedef Pointer<BasicBlock> Ptr; 

private:
    std::vector<Instruction> instrs_;
    BasicBlock* branch_;
    BasicBlock* next_;
    String::Ptr label_;
    int round_;
};


