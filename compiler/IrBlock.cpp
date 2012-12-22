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

#include "IrBlock.hpp"
#include "Machine.hpp"

Stream::Ptr operator<<(Stream::Ptr out, const Address& addr) {
    return out << addr.value();
}

Stream::Ptr operator<<(Stream::Ptr out, const RegisterId& id) {
    if (out->machine()) {
        Register* reg = out->machine()->reg(id);
        if (reg) {
            return out << reg->name();
        }
        int const regs = out->machine()->regs();
        return out << (id.is_float() ? "f" : "i") << (id.id()-regs);
    }
    return out << (id.is_float() ? "f" : "i") << id.id();
}

Stream::Ptr operator<<(Stream::Ptr out, const Operand& op) {
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
    if (!!op.addr() && !!op.reg()) {
        if (op.is_indirect()) { out << "mem["; }
        out << op.reg() << "+" << op.addr();
        if (op.is_indirect()) { out << "]"; }
        return out;
    }
    if (!!op.addr()) {
        if (op.is_indirect()) { out << "mem["; }
        if (op.addr().value() < 0) {
            out << "rbp-" << -op.addr().value();
        } else {
            out << "rbp+" << op.addr();
        }
        if (op.is_indirect()) { out << "]"; }
        return out;
    }
    if (!!op.reg()) {
        return out << op.reg();
    }
    return out << "nil";
}

bool Operand::operator==(const Operand& other) const {
    return obj_ == other.obj_ && reg_ == other.reg_ && addr_ == other.addr_;
}

bool Operand::operator!=(const Operand& other) const {
    return !operator==(other);
}

bool IrBlock::is_terminated() const {
    if (instrs_.empty()) {
        return false;
    }
    Opcode op = instrs_.back().opcode();
    if (op == RET || op == JUMP || (op >= BNE && op <= BLE)) {
        return true;
    }
    return false;
}

bool IrBlock::is_ret() const {
    if (instrs_.empty()) {
        return false;
    }
    return instrs_.back().opcode() == RET;
}


Instruction const& IrBlock::instr(const Instruction& inst) { 
    instrs_.push_back(inst); 
    return instrs_.back();
}

Instruction const& IrBlock::instr(Opcode op, Operand res, Operand one, Operand two) {
    instrs_.push_back(Instruction(op, res, one, two));
    return instrs_.back();
}

RegisterIdSet::RegisterIdSet(RegisterIdSet const& other) : 
    high_(0),
    size_(other.size_) {
    
    if (other.high_) {
        high_ = new unsigned[size_](); 
        memcpy(high_, other.high_, sizeof(*high_)*size_);
    }
    memcpy(low_, other.low_, sizeof(low_));
}

RegisterIdSet::RegisterIdSet(int size) : 
    high_(0),
    size_(std::max(0, 1+((size-1)/BUCKET_BITS)-BUCKET_LOW)) {
    
    assert(size >= 0);
    if (size_ > 0) {
        high_ = new unsigned[size_]();
    }
    memset(low_, 0, sizeof(low_));
}

void RegisterIdSet::set(RegisterId id) {
    // Adds the register to the register bitset.
    int bucket = id.id() / BUCKET_BITS;
    int bit = 0x1 << (id.id() % BUCKET_BITS);

    if (bucket < BUCKET_LOW) {
        low_[bucket] |= bit;
    } else {   
        assert(bucket < size_+BUCKET_LOW); 
        high_[bucket-BUCKET_LOW] |= bit;
    }
}

void RegisterIdSet::del(RegisterId id) {
    // Deletes the register from the register bitset.
    int bucket = id.id() / BUCKET_BITS;
    int bit = 0x1 << (id.id() % BUCKET_BITS);
    
    if (bucket < BUCKET_LOW) { 
        low_[bucket] &= ~bit;
    } else {
        assert(bucket < size_+BUCKET_LOW);
        high_[bucket-BUCKET_LOW] &= ~bit;
    } 
}

bool RegisterIdSet::bit(int id) const {
    // Returns true if 'id' is in the register bitset
    int bucket = id / BUCKET_BITS;
    int bit = 0x1 << (id % BUCKET_BITS);
    
    if (bucket < BUCKET_LOW) { 
        return low_[bucket] & bit;
    } else {
        assert(bucket < size_+BUCKET_LOW);
        return high_[bucket-BUCKET_LOW] & bit;
    } 
}

int RegisterIdSet::next(int id) const {
    // Returns the next Register in the set, after 'id'; returns -1 if no more
    // bits can be found.
    int bucket = (id+1) / BUCKET_BITS;
    int bit = (id+1) % BUCKET_BITS;
    if (bucket >= size_+BUCKET_LOW) {
        return -1;
    }
    int next = next_bit(bucket, bit); 
    if (!next) {
        bucket = next_bucket(bucket);
        if (bucket > 0) {
            return bucket*BUCKET_BITS+next_bit(bucket, 0)-1;
        }
    } else {
        return bucket*BUCKET_BITS+next-1;
    }
    return -1;
}

int RegisterIdSet::next_bit(int bucket, int bit) const {
    // Returns the 1+ next bit set in 'bucket' following 'bit'.  Returns 0 if
    // the bit wasn't found in this bucket.
    assert(bucket < BUCKET_LOW+size_);
    unsigned val = (bucket < BUCKET_LOW) ? low_[bucket] : high_[bucket-BUCKET_LOW]; 
    unsigned mask = BUCKET_MASK << bit;
    return __builtin_ffs(val & mask);
}

int RegisterIdSet::next_bucket(int bucket) const {
    // Returns the next non-zero bucket after 'bucket'
    bucket += 1;
    while (bucket < size_+BUCKET_LOW) {
        if (bucket < BUCKET_LOW) {
            if (low_[bucket]) {
                return bucket;  
            } else {
                bucket++;
            }    
        } else {
            if (high_[bucket-BUCKET_LOW]) {
                return bucket;
            } else {
                bucket++;
            }
        }
    }
    return -1;
}

RegisterIdSet RegisterIdSet::operator&(RegisterIdSet const& other) const {
    // Performs an intersection between this set and another set.
    RegisterIdSet out(std::max(bits(), other.bits()));
    RegisterIdSet const& big = (size_ > other.size_) ? *this : other;
    RegisterIdSet const& small = (size_ <= other.size_) ? *this : other;
    for (int i = 0; i < BUCKET_LOW; ++i) {
        out.low_[i] = big.low_[i] & small.low_[i];
    } 
    for (int i = 0; i < small.size_; ++i) {
        out.high_[i] = big.high_[i] & small.high_[i];
    }
    return out;
}

RegisterIdSet RegisterIdSet::operator|(RegisterIdSet const &other) const {
    // Performs a union between this set and another set.
    RegisterIdSet out(std::max(bits(), other.bits()));
    RegisterIdSet const& big = (size_ > other.size_) ? *this : other;
    RegisterIdSet const& small = (size_ <= other.size_) ? *this : other;
    for (int i = 0; i < BUCKET_LOW; ++i) {
        out.low_[i] = big.low_[i] | small.low_[i];
    } 
    for (int i = 0; i < small.size_; ++i) {
        out.high_[i] = big.high_[i] | small.high_[i];
    }
    for (int i = small.size_; i < big.size_; ++i) {
        out.high_[i] = big.high_[i];
    }
    return out;
}

bool RegisterIdSet::operator==(RegisterIdSet const& other) const {
    if (size_ != other.size_) { return false; }
    for (int i = 0; i < BUCKET_LOW; ++i) {
        if (low_[i] != other.low_[i]) {
            return false;
        }
    }    
    for (int i = 0; i < size_; ++i) {
        if (high_[i] != other.high_[i]) {
            return false;
        }
    }
    return true;
}

bool RegisterIdSet::operator!=(RegisterIdSet const& other) const {
    if (size_ != other.size_) { return true; }
    for (int i = 0; i < BUCKET_LOW; ++i) {
        if (low_[i] != other.low_[i]) {
            return true; 
        }
    }
    for (int i = 0; i < size_; ++i) {
        if (high_[i] != other.high_[i]) {
            return true;
        }
    }
    return false; 
}

RegisterIdSet& RegisterIdSet::operator&=(RegisterIdSet const& other) {
    assert(size_ >= other.size_); 
    for (int i = 0; i < BUCKET_LOW; ++i) {
        low_[i] &= other.low_[i];
    } 
    for (int i = 0; i < other.size_; ++i) {
        high_[i] &= other.high_[i];
    }
    return *this;
}

RegisterIdSet& RegisterIdSet::operator|=(RegisterIdSet const& other) {
    assert(size_ >= other.size_); 
    for (int i = 0; i < BUCKET_LOW; ++i) {
        low_[i] |= other.low_[i];
    } 
    for (int i = 0; i < other.size_; ++i) {
        high_[i] |= other.high_[i];
    }
    return *this;
}

RegisterIdSet& RegisterIdSet::operator=(RegisterIdSet const& other) {
    if (this == &other) { return *this; }
    if (size_ != other.size_) {
        delete high_;
        high_ = 0;
    }
    if (other.size_ > 0) {
        if (size_ != other.size_) {
            high_ = new unsigned[other.size_]();
        }
        memcpy(high_, other.high_, sizeof(*high_)*other.size_);
    }
    size_ = other.size_;
    memcpy(low_, other.low_, sizeof(low_));
    return *this;
}

void RegisterIdSet::clear() {
    memset(low_, 0, sizeof(low_));
    if (high_) {
        memset(high_, 0, sizeof(*high_)*size_);
    }
}

int RegisterIdSet::count() const {
    int count = 0;
    for (int i = 0; i < BUCKET_LOW; ++i) {
        count += __builtin_popcount(low_[i]);        
    }
    for (int i = 0; i < size_; ++i) {
        count += __builtin_popcount(high_[i]);
    }
    return count;
}

Instruction::Instruction(Opcode op, Operand res, Operand first, Operand sec) :
    opcode_(op),
    first_(first),
    second_(sec),
    result_(res),
    liveness_(0) {

    if (first.is_float() || sec.is_float()) {
        int id = result_.reg().id();
        int flags = result_.reg().flags()|RegisterId::FLOAT; 
        result_.reg(RegisterId(id, flags));
    }
}

Liveness* Instruction::liveness() const {
    return liveness_ ? liveness_ : liveness_ = new Liveness; 
}

