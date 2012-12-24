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

#include "Section.hpp"
#include <cassert>

void Section::uint64(uint64_t val) { 
    size_t off = buffer_.size();
    buffer_.resize(off + sizeof(val));
    memcpy(&buffer_[0] + off, &val, sizeof(val));
}

void Section::uint32(uint32_t val) { 
    size_t off = buffer_.size();
    buffer_.resize(off + sizeof(val));
    memcpy(&buffer_[0] + off, &val, sizeof(val));
}

void Section::uint16(uint16_t val) { 
    size_t off = buffer_.size();
    buffer_.resize(off + sizeof(val));
    memcpy(&buffer_[0] + off, &val, sizeof(val));
}

void Section::uint8(uint8_t val) { 
    size_t off = buffer_.size();
    buffer_.resize(off + sizeof(val));
    memcpy(&buffer_[0] + off, &val, sizeof(val));
}

void Section::buffer(void const* buffer, int len) {
    size_t off = buffer_.size();
    buffer_.resize(off + len);
    memcpy(&buffer_[0] + off, buffer, len);
}

void Section::pad(int num) {
    // Pad the number of bytes to a multiple of the the given value
    int rem = buffer_.size() % num;
    if (rem) {
        buffer_.resize(buffer_.size() + num - (rem));
    }
}
