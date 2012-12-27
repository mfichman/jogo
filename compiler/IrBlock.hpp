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
    static int const SPECIAL = 0x4000; // Special reg that is not allocatable

    RegisterId() : id_(0), flags_(0) {}
    RegisterId(int id, int flags) : id_(id), flags_(flags) {
        assert(id < std::numeric_limits<short>::max());
    }

    bool is_int() const { return !is_float(); }
    bool is_float() const { return flags_ & FLOAT; }
    bool is_special() const { return flags_ & SPECIAL; }
    bool is_valid() const { return id_; }
    int flags() const { return flags_; }
    int id() const { return id_; }
    bool operator!() const { return !is_valid(); }
    bool operator==(RegisterId const& id) const { return id_ == id.id_; } 
    bool operator!=(RegisterId const& id) const { return id_ != id.id_; }
    bool operator<(RegisterId const& id) const { return id_ < id.id_; }
    
private:
    short id_;
    short flags_;
};

/* 
 * Bitset of register IDs.  This class is optimized for small sets of 128
 * registers or less.
 */
class RegisterIdSet {
public:
    RegisterIdSet(RegisterIdSet const& other);
    explicit RegisterIdSet(int size=0);
    ~RegisterIdSet() { delete high_; }
    void set(RegisterId id);
    void del(RegisterId id);
    bool has(RegisterId id) const { return bit(id.id()); }
    void clear();
    bool bit(int index) const;
    int bits() const { return BUCKET_BITS*(size_+BUCKET_LOW); }
    int count() const;
    int next(int id) const;
    bool operator==(RegisterIdSet const& other) const;
    bool operator!=(RegisterIdSet const& other) const;
    RegisterIdSet operator&(RegisterIdSet const& other) const;
    RegisterIdSet operator|(RegisterIdSet const& other) const;
    RegisterIdSet& operator&=(RegisterIdSet const& other);
    RegisterIdSet& operator|=(RegisterIdSet const& other);
    RegisterIdSet& operator=(RegisterIdSet const& other);
private:
    int next_bucket(int bucket) const;
    int next_bit(int bucket, int bit) const;

    static int const BUCKET_LOW = 4;
    static int const BUCKET_BITS = 8 * sizeof(unsigned);
    static unsigned const BUCKET_MASK = 0xFFFFFFFF;
    unsigned low_[BUCKET_LOW];
    unsigned* high_;
    int size_;
};

/* Represents a 'logical' address, expressed in words (not bytes) */
class Address {
public:
    static int const INDIRECT = 0x1;
    static int const VALID = 0x2;

    Address() : value_(0), flags_(0) {}
    explicit Address(int value) : value_(value), flags_(INDIRECT|VALID) {
        assert(value < std::numeric_limits<short>::max());
    }
    Address(int value, int flags) : value_(value), flags_(flags|VALID) {
        assert(value < std::numeric_limits<short>::max());
    }

    int value() const { return value_; }
    bool operator!() const { return !is_valid(); }
    bool operator==(Address const& ad) const { 
        return value_ == ad.value_ && flags_ == ad.flags_; 
    }
    bool operator!=(Address const& ad) const { 
        return value_ != ad.value_ || flags_ != ad.flags_; 
    }
    bool is_valid() const { return flags_ & VALID; }
    bool is_indirect() const { return flags_ & INDIRECT; }

private:
    short value_;
    short flags_;

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
    Operand(FloatLiteral* literal) : obj_(literal), 
        reg_(RegisterId(0, RegisterId::FLOAT)) {}
    // Represents the value of a literal, i.e., LITERAL.
    Operand(RegisterId reg) : reg_(reg) {}
    // Represents the contents of a register, e.g., REG.
    Operand(RegisterId reg, Address addr) : reg_(reg), addr_(addr) {}
    // Represents the contents of a memory location, e.g., MEM[ADDR+REG]
    Operand(Address addr) : addr_(addr) {}
    // Represents the contents of a memory address, e.g., MEM[ADDR]

    Expression* literal() const { return dynamic_cast<Expression*>(obj_.pointer()); }
    String* label() const { return dynamic_cast<String*>(obj_.pointer()); }
    RegisterId reg() const { return reg_; }
    Address addr() const { return addr_; }
    bool has_addr() const { return addr_.is_valid(); }
    bool is_indirect() const { return addr_.is_indirect(); }
    bool is_float() const { return reg_.is_float(); }
    bool operator==(Operand const& other) const;
    bool operator!=(Operand const& other) const;
    bool operator!() const { return !obj_ && !reg_ && !addr_; }
    void reg(RegisterId reg) { reg_ = reg; }

private:
    Pointer<Object> obj_; // Literal expression or string label
    RegisterId reg_; // Temporary variable ID (negative = machine register)
    Address addr_; // Address offset (in words)
};

Stream::Ptr operator<<(Stream::Ptr out, Operand const& op);

/* Enumeration of opcodes available to the TAC code */
enum Opcode { 
    MOV, ADD, SUB, MUL, DIV, NEG, ANDB, ORB, LOAD, STORE, NOTB, CALL, JUMP,
    BNE, BE, BNZ, BZ, BG, BL, BGE, BLE, RET, NOP
    // Note: BNE through BLE must be contiguous
};

/* Class for liveness information related to the instruction */
class Liveness : public Object {
public:
    RegisterIdSet const& in() const { return in_; }
    RegisterIdSet const& out() const { return out_; }
    void in(RegisterIdSet const& in) { in_ = in; }
    void out(RegisterIdSet const& out) { out_ = out; }
    typedef Pointer<Liveness> Ptr;

private:
    RegisterIdSet in_; // Live variables on incoming edge
    RegisterIdSet out_; // Live variables on outgoing edge
};

/* Class for three-address code instructions */
class Instruction {
public:
    Instruction(Opcode op, Operand res, Operand one, Operand two);
    Opcode opcode() const { return opcode_; }
    Operand const& first() const { return first_; }
    Operand const& second() const { return second_; }
    Operand result() const { return result_; }
    Liveness* liveness() const;
    void opcode(Opcode opcode) { opcode_ = opcode; }
    void first(Operand first) { first_ = first; }
    void second(Operand second) { second_ = second; }
    void result(Operand result) { result_ = result; }

private:
    Opcode opcode_;
    Operand first_;
    Operand second_;
    Operand result_;
    mutable Liveness::Ptr liveness_;
};

/* Sequence of intermediate-representation (IR) instructions */
class IrBlock : public Object {
public:
    IrBlock() : branch_(0), next_(0), round_(0) {}
    IrBlock* branch() const { return branch_; }
    IrBlock* next() const { return next_; }
    String* label() const { return label_; }
    Instruction const& instr(int index) const { return instrs_[index]; }
    Instruction& instr(int index) { return instrs_[index]; }
    int round() const { return round_; }
    int instrs() const { return instrs_.size(); }
    bool is_terminated() const;
    bool is_ret() const;
    void swap(IrBlock* other) { instrs_.swap(other->instrs_); }
    void branch(IrBlock* branch) { branch_ = branch; }
    void next(IrBlock* branch) { next_ = branch; }
    void label(String* label) { label_ = label; }
    Instruction const& instr(Instruction const& inst);
    Instruction const& instr(Opcode op, Operand res, Operand one, Operand two);
    void round_inc() { round_++; }
    typedef Pointer<IrBlock> Ptr; 

private:
    std::vector<Instruction> instrs_;
    IrBlock* branch_;
    IrBlock* next_;
    String::Ptr label_;
    int round_;
};

