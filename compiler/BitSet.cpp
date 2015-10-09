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

#define NOMINMAX
#include "BitSet.hpp"
#include <algorithm>

bool BitSet::set(int value) {
    // Sets the bit at index 'value' to one; also handles negative values.
    int offset = (value < 0) ? value * -2 - 1 : value * 2;
    int index = offset / (sizeof(unsigned) * 8);
    int bit = offset % (sizeof(unsigned) * 8);

    if (index >= data_.size()) {
        data_.resize(index+1);
    }

    // Return true if the bit was previously NOT set.
    bool ret = !(data_[index] & (1 << bit));
    data_[index] |= 1 << bit;
    return ret;
}

bool BitSet::clear(int value) {
    // Sets the bit at index 'value' to zero; also handles negative values.
    int offset = (value < 0) ? value * -2 - 1 : value * 2;
    int index = offset / (sizeof(unsigned) * 8);
    int bit = offset % (sizeof(unsigned) * 8);

    if (index >= data_.size()) {
        data_.resize(index+1);
    }

    // Return true if the bit was previously set
    bool ret = data_[index] & (1 << bit);
    data_[index] &= ~(1 << bit);
    return ret;
}

bool BitSet::test(int value) const {
    // Sets the bit at index 'value' to zero; also handles negative values.
    int offset = (value < 0) ? value * -2 - 1 : value * 2;
    int index = offset / (sizeof(unsigned) * 8);
    int bit = offset % (sizeof(unsigned) * 8);

    if (index >= data_.size()) {
        return false;
    }
    return data_[index] & (1 << bit);
}

BitSet BitSet::operator&(const BitSet& other) const {
    // Perform a set intersection on the bits of this BitSet and 'other', and
    // then return a new BitSet.
    BitSet ret;
    ret.data_.resize(std::max(other.data_.size(), data_.size()));

    for (int i = 0; i < ret.data_.size(); i++) {
        if (i >= other.data_.size()) {
            ret.data_[i] = 0;
        } else if (i >= data_.size()) {
            ret.data_[i] = 0;
        } else {
            ret.data_[i] = other.data_[i] & data_[i]; 
        }
    }
    return ret;
}

BitSet BitSet::operator|(const BitSet& other) const {
    // Perform a set union on the bits of this BitSet and 'other', and
    // then return a new BitSet.
    BitSet ret;
    ret.data_.resize(std::max(other.data_.size(), data_.size()));
    
    for (int i = 0; i < ret.data_.size(); i++) {
        if (i >= other.data_.size()) {
            ret.data_[i] = data_[i];
        } else if (i >= data_.size()) {
            ret.data_[i] = other.data_[i];
        } else {
            ret.data_[i] = other.data_[i] | data_[i]; 
        }
    }
    return ret;
}

BitSet& BitSet::operator&=(const BitSet& other) {
    // Perform a set intersection with 'other' modifying the receiver BitSet.
    data_.resize(std::max(other.data_.size(), data_.size()));

    for (int i = 0; i < data_.size(); i++) {
        if (i >= other.data_.size()) {
            data_[i] = 0;
        } else {
            data_[i] &= other.data_[i];
        }
    }    
    return *this;
}

BitSet& BitSet::operator|=(const BitSet& other) {
    // Perform a set union with 'other', modyfing the receiver BitSet.
    data_.resize(std::max(other.data_.size(), data_.size()));

    for (int i = 0; i < data_.size(); i++) {
        if (i < other.data_.size()) {
            data_[i] |= other.data_[i];
        }
    }    
    return *this;
}

bool BitSet::operator==(const BitSet& other) {
    // Return true if 'other' is equal to the bitset.
    for (int i = 0; i < std::max(data_.size(), other.data_.size()); i++) {
        if (i >= other.data_.size() && data_[i] != 0) {
            return false;
        }
        if (i >= data_.size() && other.data_[i] != 0) {
            return false;
        }
        if (other.data_[i] != data_[i]) {
            return false;
        }
    }
    return true;
}

bool BitSet::operator!=(const BitSet& other) {
    return !operator==(other);
}

void BitSet::clear() {
    data_.clear();
}
