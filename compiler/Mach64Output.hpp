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
#include "OutputFormat.hpp"
#include "Section.hpp"
#include "Stream.hpp"
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/reloc.h>
#include <mach-o/x86_64/reloc.h>
#include <fstream>
#include <vector>

/* Helps output basic blocks in the Mach-O format */
class Mach64Output : public OutputFormat {
public:
    Mach64Output() : text_(new Section), string_(new Section) {
        string_->uint8(0);
        // The string table must have a 0-byte at the beginning, b/c the
        // nlist_64 struct uses an offset of 0 into the string table to
        // represent a 0-length string.
    }
    Section* text() const { return text_; }
    void ref(String* name, RelocType type); // Add to reloc table
    void label(String* label); // Add a label
    void local(String* label); // Local label
    void out(Stream* out);

public:
    static int const TEXT_SECT = 1;
    //static int const CSTRING_SECT = 2;
    //Section cstring_;
    Section::Ptr text_;
    Section::Ptr string_;

    std::ofstream out_;
    std::vector<nlist_64> symtab_; // Symbol table
    std::vector<relocation_info> reloc_; // Relocation table 
    std::map<String::Ptr,int> symbol_;
};
