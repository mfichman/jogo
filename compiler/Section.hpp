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
#include <vector>
#include <stdint.h>

/* A sequence of machine code instructions/data corresponding to a section */
class Section : public Object {
public:
    Section() : flags_(0) {}
    typedef Pointer<Section> Ptr;
    int bytes() const { return buffer_.size(); }
    uint8_t const* text() const { return buffer_.size() ? &buffer_.front() : 0; }
    uint32_t flags() const { return flags_; }
    void uint64(uint64_t val);
    void uint32(uint32_t val);
    void uint16(uint16_t val);
    void uint8(uint8_t val);
    void buffer(void const* buf, int len);
    void align(int num, uint8_t fill=0);
    void flags(uint32_t flags) { flags_ = flags; }
private:
    uint32_t flags_;
    std::vector<uint8_t> buffer_;
};
