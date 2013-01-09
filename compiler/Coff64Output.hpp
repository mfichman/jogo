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

#ifdef WINDOWS

#include "Jogo.hpp"
#include "OutputFormat.hpp"
#include "Section.hpp"
#include <stdint.h>
#include <winnt.h>
#include <fstream>
#include <vector>

// IMAGE_FILE_HEADER 
struct Coff64Header {};

// IMAGE_SECTION_HEADER
struct Coff64Section {};

// IMAGE_RELOCATION
struct Coff64Reloc {};

// IMAGE_SYMBOL
struct Coff64Sym {}; // Followed by string table, which has 4 bytes for total size

// COFF Header - IMAGE_FILE_HEADER
// Section Headers - IMAGE_SECTION_HEADER
// Code 
// Relocations - IMAGE_RELOCATION
// Data
// Relocations - IMAGE_RELOCATION
// Symbol Table - IMAGE_SYMBOL
// String Table (immediately after Symbol Table)

/* Helps output basic blocks in the Mach-O format */
class Coff64Output : public OutputFormat {
public:
    Coff64Output();
    Section* text() const { return text_; }
    Section* data() const { return data_; }
    void ref(String* name, RelocType rtype);
    void sym(String* name, SymType type);
    void out(Stream* out);

public:
    static int const OUT_SECT_TEXT = 1;
    static int const OUT_SECT_DATA = 2;
    Section::Ptr text_;
    Section::Ptr data_;
    Section::Ptr string_;

    std::ofstream out_;
    std::vector<IMAGE_SYMBOL> sym_; // Symbol table
    std::vector<IMAGE_RELOCATION> text_reloc_; // Relocation table 
    std::vector<IMAGE_RELOCATION> data_reloc_; // Relocation table 
    std::map<String::Ptr,size_t> symbol_;
};

#endif
