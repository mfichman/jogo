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

/* Interface for output file formats (e.g., COFF, Mach-O) */
class OutputFormat : public Object {
public:
    typedef Pointer<OutputFormat> Ptr;
    typedef int RelocType;
    virtual ~OutputFormat() {}

    virtual void ref(String* name, RelocType type)=0;
    // Records a references to a symbol name at the current byte offset in the
    // text section.  Ref info is used to generate relocation entries.

    virtual void label(String* label)=0;
    virtual void local(String* label)=0;
    // Emits a label at the given location, adding an entry to the symbol
    // table.  This function asserts if a symbol is defined twice.

    virtual void out(Stream* out)=0;
    // Flushes the output format to the given file.

    virtual Section* text() const=0;
    // Returns the tet section (for code emit)

    static int const RELOC_ABSOLUTE = 0x1;
    static int const RELOC_BRANCH = 0x2;
    static int const RELOC_SIGNED = 0x3;
};
