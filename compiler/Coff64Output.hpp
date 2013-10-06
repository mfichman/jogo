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
#include "File.hpp"
#include "OutputFormat.hpp"
#include "Environment.hpp"
#include "Section.hpp"
#include <stdint.h>
#include <winnt.h>
#include <fstream>
#include <vector>

// COFF Header - IMAGE_FILE_HEADER
// Section Headers - IMAGE_SECTION_HEADER
// Code 
// Relocations - IMAGE_RELOCATION
// Data
// Relocations - IMAGE_RELOCATION
// Symbol Table - IMAGE_SYMBOL
// String Table (immediately after Symbol Table)

#pragma pack(1)
struct CvSectionHeader {
    uint32_t type;
    uint32_t size;
};

#pragma pack(1)
struct CvSourceFileInfo {
    uint32_t offset; // Offset in string table
    uint16_t checksum_mode; // Checksum present?
    uint16_t unused; // Padding
};

#pragma pack(1)
struct CvLineNumbers {
    uint32_t offset; // Start offset in section
    uint16_t section; // Section index
    uint16_t unused1; // Padding
    uint32_t section_length; // No idea what this is
    uint32_t source_file; // Offset in source file info table
    uint32_t linenos_count; // Number of lineno pairs 
    uint32_t linenos_size; // Number of bytes of lineno pairs + 12
};

#pragma pack(1)
struct CvLineNumber {
    uint32_t offset; // Offset in section
    uint32_t line; // Line number (set high bit for breakable)
};

#pragma pack(1)
struct CvSymbolRecordHeader {
    uint16_t size; // Size field itself is _not_ included in the size 
    uint16_t type;
};

#pragma pack(1)
struct CvObjectFileName {
    CvSymbolRecordHeader header;
    uint32_t signature; // Always 0
    // 0-terminated string containing object file name
};

#pragma pack(1)
struct CvFunction {
    CvSymbolRecordHeader header;
    uint32_t parent;
    uint32_t pend;
    uint32_t pnext;
    uint32_t size; // Length of procedure
    uint32_t prolog_offset; // Offset from beginning of procedure to frame setup
    uint32_t epilog_offset; // Offset from beginning of procedure to frame cleanup
    uint32_t type; // Type index of procedure
    uint32_t offset; // Offset in section
    uint16_t section; // Section of symbol
    uint8_t flags; 
    // 0-terminated string containing procedure name
};

#pragma pack(1)
struct CvEndBlock {
    CvSymbolRecordHeader header;
};

class Coff64Section : public Section {
public:
    Coff64Section(std::string const& name, size_t id) : name_(name), id_(id) {}
    std::string const& name() const { return name_; }
    size_t id() const { return id_; }
    size_t relocs() const { return reloc_.size(); }
    char* reloc() const { return (char*)&reloc_.front(); }
    void reloc(IMAGE_RELOCATION const& reloc) { reloc_.push_back(reloc); }

    typedef Pointer<Coff64Section> Ptr;
private:
    std::string name_;
    size_t id_;
    std::vector<IMAGE_RELOCATION> reloc_; // Relocation table 
};

/* Helps output basic blocks in the COFF (with CodeView8 debug info) format */
class Coff64Output : public OutputFormat {
public:
    Coff64Output(Environment* env);
    Section* text() const { return text_; }
    Section* data() const { return data_; }
    void ref(String* name, RelocType rtype);
    void sym(String* name, SymType type);
    void out(Stream* out);
    void line(int line);
    void reg(String* name, RegisterId id) {}
    void stack(String* name, int offset) {}
    void file(File* file);
    void function(String* name);
    void ret();

public:
    Coff64Section* section(std::string const& name);
    void write_debug_line_numbers();
    IMAGE_RELOCATION reloc(uint32_t addr, String* name, uint16_t type);

    static int const CV_SOURCE_FILE_INFO = 0xF4;
    static int const CV_SOURCE_FILE_STRINGS = 0xF3;
    static int const CV_LINE_NUMBERS = 0xF2;
    static int const CV_SYMBOL_INFO = 0xF1;

    static int const CV_FUNCTION = 0x1147;
    static int const CV_END_BLOCK = 0x114F;

    Environment::Ptr env_;
    std::vector<Coff64Section::Ptr> section_;
    Coff64Section::Ptr text_;
    Coff64Section::Ptr data_;
    Coff64Section::Ptr debug_;
    Coff64Section::Ptr types_;
    Coff64Section::Ptr string_;
    int line_;

    std::ofstream out_;
    std::vector<IMAGE_SYMBOL> sym_; // Symbol table
    std::map<String::Ptr,size_t> symbol_;
    std::vector<CvLineNumber> lineno_;
    CvFunction function_;
    String::Ptr function_name_;
};

#endif
