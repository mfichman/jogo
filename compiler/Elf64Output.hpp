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

#ifdef LINUX

#include <OutputFormat.hpp>
#include <Section.hpp>
#include <stdint.h>
#include <elf.h>
#include <fstream>

// ELF Header - Elf64_Ehdr
// Program header table (optional) - Elf64_Phdr
// Section 1
// Section 2
// ...
// section header table - Elf64_Shdr
// Elf64_Rel - gets addend from the original value of the reloc word
// Elf64_Rela - explicitly contains the addend

/* Outputs basic blocks in ELF-64 format */
class Elf64Output : public OutputFormat {
public:
    Elf64Output();
    Section* text() const { return text_; }
    Section* data() const { return data_; }
    void ref(String* name, RelocType rtype);
    void sym(String* name, SymType type);
    void out(Stream* out);
    void line(int line) {}
    void reg(String* name, RegisterId id) {}
    void stack(String* name, int offset) {}
    void file(File* file) {}
    void function(String* name) {}
    void ret() {}

public:
    void sort_symtab();

    static int const OUT_SECT_NULL = 0;
    static int const OUT_SECT_TEXT = 1;
    static int const OUT_SECT_DATA = 2;
    static int const OUT_SECT_RELTEXT = 3;
    static int const OUT_SECT_RELDATA = 4;
    static int const OUT_SECT_SYMTAB = 5;
    static int const OUT_SECT_STRTAB = 6;
    static int const OUT_SECT_LAST = 6;
    Section::Ptr text_;
    Section::Ptr data_;
    Section::Ptr string_;
    
    std::ofstream out_;
    std::vector<Elf64_Sym> sym_; // Symbol table
    std::vector<Elf64_Rela> text_reloc_; // Relocation table
    std::vector<Elf64_Rela> data_reloc_; // Relocation table
    std::map<String::Ptr,size_t> symbol_;
    size_t local_syms_; 
};

#endif
