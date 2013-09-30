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

#pragma once

#include "Jogo.hpp"
#include "DebugFormat.hpp"
#include "IrBlock.hpp"
#include "Section.hpp"
#include "String.hpp"

/* Emits debugger information */
class JdbFormat : public DebugFormat {
public:
    JdbFormat();
    void function(String* name);
    void line(int addr, int line);
    void reg(int addr, String* name, RegisterId id);
    void stack(int addr, String* name, int offset);
    typedef Pointer<JdbFormat> Ptr;

    static const uint8_t FUNCTION = 0x00;
    static const uint8_t ADDR = 0x01;
    static const uint8_t REGISTER = 0x02;
    static const uint8_t STACK = 0x03;
    static const uint8_t LINE = 0x04;
    static const uint8_t SYMBOL = 0x05; 

private:
    void addr(int addr);
    uint64_t symbol(String* name);

    Section::Ptr section_;
    int line_;
    int addr_;
    int next_symbol_;
    std::map<String::Ptr,uint64_t> symbol_;
};

// The debugger info file has a simple binary format interpreted as follows:
//
// records   ::= \x00 id                Function
//             | \x01 addr              Address (relative to function start) 
//             | \x02 int64 id          Variable location (register)
//             | \x03 int64 id          Variable location (stack)
//             | \x04 int64             Line number
//             | \x05 int64 byte*       Symbol name
// 
// id        ::= int64                  Function/variable symbol id
//
// All symbols should come first in the file, so that they can be resolved
// properly by the subsequent record types (e.g., function records).  An addr
// record is followed by the line number and updated variable locations (if
// they changed from the previous addr).  
//
// Addresses for addr records are always relative to the function start;
// addresses for stack variable records are relative to the base pointer (and
// may be negative).  
//
// The register record has an int64 which contains the RegisterId for a
// register in the given Machine architecture (see Machine.hpp for register
// codes)
//
// Debug info can be found as a section in the final executable, or as a
// separate file that goes along with the executable/library.
