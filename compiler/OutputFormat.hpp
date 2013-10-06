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
#include "Object.hpp"
#include "String.hpp"
#include "Stream.hpp"
#include "Section.hpp"
#include "IrBlock.hpp"

/* Interface for output file formats (e.g., COFF, Mach-O) */
class OutputFormat : public Object {
public:
    typedef Pointer<OutputFormat> Ptr;
    typedef int RelocType;
    typedef int SymType;
    virtual ~OutputFormat() {}

    virtual void ref(String* name, RelocType rtype)=0;
    // Records a references to a symbol name at the current byte offset in the
    // text section.  Ref info is used to generate relocation entries.

    virtual void sym(String* name, SymType type)=0;
    // Emits a label at the given location, adding an entry to the symbol
    // table.  This function asserts if a symbol is defined twice.

    virtual void out(Stream* out)=0;
    // Flushes the output format to the given file.

    virtual void line(int line)=0;
    // Writes line # debug info to the debug format

    virtual void reg(String* name, RegisterId id)=0;
    // Outputs a variable location (register)

    virtual void stack(String* name, int offset)=0;
    // Outputs a stack variable location (offset from BP)

    virtual void file(File* file)=0;
    // Full path to the file

    virtual void function(String* name)=0;
    // Start of a function

    virtual void ret()=0;
    // End of a function

    virtual Section* text() const=0;
    virtual Section* data() const=0;
    // Returns the text/data sections (for code emit)

    static int const REF_BRANCH = 0x1;
    static int const REF_SIGNED = 0x2;
    static int const REF_DATA = 0x3;
    static int const REF_TEXT = 0x4;
    static int const REF_CALL = 0x5;
    static int const REF_VTABLE = 0x6;

    static int const SYM_LOCAL = 0x1;
    static int const SYM_DATA = 0x2;
    static int const SYM_LDATA = 0x3;
    static int const SYM_TEXT = 0x4;
    static int const SYM_LTEXT = 0x5;
};

static inline uint32_t align(uint32_t in, uint32_t alignment) {
    uint32_t mod = in % alignment;
    if (mod) {
        return alignment-mod+in; 
    } else {
        return in;
    }
}
