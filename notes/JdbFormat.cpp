/*
 * Copyright (c) 2013 Matt Fichman
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

#include "DebugFormat.hpp"

DebugFormat::DebugFormat() : 
    section_(new Section), 
    line_(0), 
    addr_(0), 
    next_symbol_(0) { 
}

void DebugFormat::function(String* name) {
    section_->uint64(name->string().length()); 
    section_->buffer(name->string().c_str(), name->string().length());
    addr_ = 0;
}

void DebugFormat::line(int addr, int line) {
    if (line != line_) {
        line_ = line;
        this->addr(addr);
        section_->uint8(LINE);
        section_->uint64(line);
    }
}

void DebugFormat::reg(int addr, String* name, RegisterId id) {
    this->addr(addr);
    section_->uint8(REGISTER);
    section_->uint64(symbol(name));
    section_->uint64(id.id());
}

void DebugFormat::stack(int addr, String* name, int offset) {
    this->addr(addr);
    section_->uint8(STACK);
    section_->uint64(symbol(name));
    section_->uint64(offset);
}

void DebugFormat::addr(int addr) {
    if (addr != addr_) {
        addr_ = addr;
        section_->uint8(ADDR);
        section_->uint64(addr);
    }
}

uint64_t DebugFormat::symbol(String* name) {
    std::map<String::Ptr,uint64_t>::iterator const i = symbol_.find(name);
    if (i == symbol_.end()) {
        int const id = next_symbol_;
        symbol_.insert(std::make_pair(name, id));
        next_symbol_++;
        return id;
    } else {
        return i->second;     
    }
}
