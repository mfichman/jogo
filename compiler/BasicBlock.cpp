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

#include "BasicBlock.hpp"

Stream::Ptr operator<<(Stream::Ptr out, const Address& addr) {
    return out << addr.value();
}

Stream::Ptr operator<<(Stream::Ptr out, const Operand& op) {
    if (op.indirect()) { out << "*"; }
    if (op.label()) {
        return out << op.label();
    }
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
    if (!!op.addr() && op.temp()) {
        out << "mem[";
        if (op.temp() > 0) {
            out << "t" << op.temp();
        } else {
            out << "r" << -op.temp();
        }   
        return out << "+" << op.addr() << "]";
    }
    if (!!op.addr()) {
        return out << "mem[" << op.addr() << "]";
    }
    if (op.temp() > 0) {
        return out << "t" << op.temp();
    }
    return out << "r" << -op.temp();
}

Operand Operand::addr(Address addr) {
    // Returns an operand that corresponds to the contents of the memory
    // address at 'addr.'
    Operand out;
    out.addr_ = addr;
    out.indirect_ = true;
    return out;
}

Operand Operand::addr(int temp, Address addr) {
    // Returns an operand that corresponds to the contents of the memory 
    // address at 'addr' plus the offset stored in the register given by
    // 'temp.'
    Operand out;
    out.addr_ = addr;
    out.temp_ = temp;
    out.indirect_ = true;
    return out;
}

bool Operand::operator==(const Operand& other) const {
    return literal_ == other.literal_ &&
        label_ == other.label_ &&
        type_ == other.type_ &&
        temp_ == other.temp_ && 
        addr_ == other.addr_ &&
        indirect_ == other.indirect_;
}

bool Operand::operator!=(const Operand& other) const {
    return !operator==(other);
}

bool BasicBlock::is_terminated() const {
    if (instrs_.empty()) {
        return false;
    }
    Opcode op = instrs_.back().opcode();
    if (op == RET || op == JUMP || (op >= BNE && op <= BLE)) {
        return true;
    }
    return false;
}

bool BasicBlock::is_ret() const {
    if (instrs_.empty()) {
        return false;
    }
    return instrs_.back().opcode() == RET;
}

void BasicBlock::instr(const Instruction& inst) { 
    instrs_.push_back(inst); 
}

void BasicBlock::instr(Opcode op, Operand res, Operand one, Operand two) {
    instrs_.push_back(Instruction(op, res, one, two));
}
