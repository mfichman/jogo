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

#ifdef WINDOWS

#include "Coff64Output.hpp"
#include <cassert>
#include <ctime>
#include <cstring>

Coff64Output::Coff64Output(Environment* env) :
    env_(env),
    text_(new Section),
    data_(new Section),
    string_(new Section),
    debug_(new Section),
    line_(-1) {
    
    string_->uint8(0); // First string is always the empty string

    uint32_t const cv_version = 4;
    debug_->uint32(cv_version);
}

void Coff64Output::ref(String* label, RelocType rtype) {
    // Refer to a symbol by name at the current text/data output location.
    // This function adds an entry to the relocation table.  The size can be
    // either 4 or 8 bytes.  A 4-byte entry indicates the instruction uses
    // RIP-relative addressing for x64.
    std::map<String::Ptr,size_t>::iterator i = symbol_.find(label);
    size_t symnum = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        IMAGE_SYMBOL* const sym = &sym_.back();
        sym->N.Name.Short = 0;
        sym->N.Name.Long = string_->bytes()+sizeof(uint32_t); 
        // Offset to name in string table (+4 for strtab size header)
        sym->Value = 0; // Address 
        sym->SectionNumber = IMAGE_SYM_UNDEFINED; // Unknown section
        if (REF_CALL == rtype || REF_VTABLE == rtype) {
            sym->Type = IMAGE_SYM_DTYPE_FUNCTION << 8;
        } else {
            sym->Type = IMAGE_SYM_DTYPE_NULL << 8; 
        }
        sym->StorageClass = IMAGE_SYM_CLASS_EXTERNAL;
        sym->NumberOfAuxSymbols = 0;
        // FixMe: Do function symbols need an auxiliary symbol entry following
        // the primary symbol?
        string_->buffer(label->string().c_str(), label->string().size()+1);
        symnum = sym_.size()-1;
    } else {
        symnum = i->second;
    }

    IMAGE_RELOCATION reloc;
    reloc.SymbolTableIndex = symnum;
    if (REF_TEXT == rtype) {
        reloc.VirtualAddress = text_->bytes();
        reloc.Type = IMAGE_REL_AMD64_ADDR64;
        text_reloc_.push_back(reloc);
    } else if (REF_DATA == rtype || REF_VTABLE == rtype) {
        reloc.VirtualAddress = data_->bytes();
        reloc.Type = IMAGE_REL_AMD64_ADDR64;
        data_reloc_.push_back(reloc);
    } else if (REF_BRANCH == rtype || REF_CALL == rtype) {
        reloc.VirtualAddress = text_->bytes();
        reloc.Type = IMAGE_REL_AMD64_REL32;
        // FixMe: This may be incorrect; relative to the RIP
        text_reloc_.push_back(reloc);
    } else if (REF_SIGNED == rtype) {
        reloc.VirtualAddress = text_->bytes();
        reloc.Type = IMAGE_REL_AMD64_REL32;
        text_reloc_.push_back(reloc);
    } else {
        assert(!"Unknown ref type");
    }
}

void Coff64Output::sym(String* label, SymType type) {
    // Finds the symbol table entry for 'label' and updates it to the current
    // offset of the text segment.
    std::map<String::Ptr,size_t>::iterator i = symbol_.find(label);
    IMAGE_SYMBOL* sym = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        sym = &sym_.back();
        sym->N.Name.Short = 0;
        sym->N.Name.Long = string_->bytes()+sizeof(uint32_t);
        sym->NumberOfAuxSymbols = 0;
        // Offset to name in string table (+4 for strtab size header)
        string_->buffer(label->string().c_str(), label->string().size()+1);
    } else {
        sym = &sym_[i->second];
        assert("Duplicate label"&&(sym->SectionNumber==0)); 
    }
    if (type & SYM_TEXT) {
        sym->Value = text_->bytes();
        sym->SectionNumber = OUT_SECT_TEXT;
        if (type & SYM_LOCAL) {
            sym->Type = IMAGE_SYM_DTYPE_NULL << 8;
        } else {
            sym->Type = IMAGE_SYM_DTYPE_FUNCTION << 8;
        }
    } else if (type & SYM_DATA) {
        sym->Value = data_->bytes();
        sym->SectionNumber = OUT_SECT_DATA;
        sym->Type = IMAGE_SYM_DTYPE_NULL << 8;
    } else {
        assert(!"Unknown section type");
    }
    if (type & SYM_LOCAL) {
        sym->StorageClass = IMAGE_SYM_CLASS_STATIC;
    } else {
        sym->StorageClass = IMAGE_SYM_CLASS_EXTERNAL;
    } 
}

void Coff64Output::line(int line) {
    // Output a line number at the current text section offset.
    if (!env_->debug()) { return; } 
    if (line <= line_ || !line) { return; }
    uint32_t const CV_BREAK_MASK = 0x80000000;
    line_ = line;
    CvLineNumber lineno;
    memset(&lineno, 0, sizeof(lineno));
    lineno.offset = text_->bytes()-function_.offset;
    lineno.line = line | CV_BREAK_MASK;
    lineno_.push_back(lineno);
}

void Coff64Output::file(File* file) {
    // Write the file into the object file in CV8 format.
    if (!env_->debug()) { return; } 
    CvSourceFileInfo file_info;
    memset(&file_info, 0, sizeof(file_info));
    file_info.offset = 1; // Offset into file string table (emitted below)
    file_info.checksum_mode = 0; // No checksum

    CvSectionHeader header;
    memset(&header, 0, sizeof(header));
    header.type = CV_SOURCE_FILE_INFO;
    header.size += sizeof(file_info);
    header.size = align(header.size, sizeof(uint32_t));

    debug_->buffer((char*)&header, sizeof(header));
    debug_->buffer((char*)&file_info, sizeof(file_info));
    debug_->align(sizeof(uint32_t));

    // Now write out the file name string table
    std::string path = file->full_path()->string();
    memset(&header, 0, sizeof(header)); 
    header.type = CV_SOURCE_FILE_STRINGS;
    header.size += 1; // For the null string
    header.size += path.length()+1;
    header.size = align(header.size, sizeof(uint32_t));
    
    debug_->buffer((char*)&header, sizeof(header));
    debug_->uint8(0);  // NUL string
    debug_->buffer(path.c_str(), path.length()+1); 
    debug_->align(sizeof(uint32_t));
}

void Coff64Output::function(String* name) {
    // Record the beginning of a new function for the debug section
    if (!env_->debug()) { return; } 
    memset(&function_, 0, sizeof(function_));
    function_.header.type = CV_FUNCTION;
    function_.header.size += name->string().length()+1;
    function_.header.size += sizeof(function_);
    function_.header.size -= sizeof(function_.header.size);
    function_.parent = 0;
    function_.pend = 0;
    function_.pnext = 0;
    function_.prolog_offset = 0;
    function_.epilog_offset = 0;
    function_.type = 0; // FixMe
    function_.offset = text_->bytes(); 
    function_.section = 0; // Filled in by relocation
    function_.flags = 0;

    function_name_ = name;
}

void Coff64Output::ret() {
    // Output debugging information for the current function
    if (!env_->debug()) { return; } 
    std::string const& name = function_name_->string();
    function_.size = text_->bytes()-function_.offset;
    function_.offset = 0; // Filled in by relocation
    // Note: this is a bit of a hack, b/c we use the offset field to
    // temporarily store the pointer to the beginning of the function so that
    // we can compute the size.

    CvEndBlock end;
    memset(&end, 0, sizeof(end));
    end.header.type = CV_END_BLOCK;
    end.header.size = sizeof(end);
    end.header.size -= sizeof(end.header.size);

    CvSectionHeader header;
    memset(&header, 0, sizeof(header));
    header.type = CV_SYMBOL_INFO;
    header.size += sizeof(function_);
    header.size += name.size()+1; // For terminating NUL-byte
    header.size += sizeof(end);
    header.size = align(header.size, sizeof(uint32_t));

    debug_->buffer((char*)&header, sizeof(header));

    uint32_t offset_addr = debug_->bytes()+(char*)&function_.offset-(char*)&function_;
    uint32_t section_addr = debug_->bytes()+(char*)&function_.section-(char*)&function_;

    debug_reloc_.push_back(reloc(offset_addr, function_name_, IMAGE_REL_AMD64_SECREL));
    debug_reloc_.push_back(reloc(section_addr, function_name_, IMAGE_REL_AMD64_SECTION));

    debug_->buffer((char*)&function_, sizeof(function_));
    debug_->buffer(name.c_str(), name.size()+1);
    debug_->buffer((char*)&end, sizeof(end));
    debug_->align(sizeof(uint32_t));

    write_debug_line_numbers();
}

void Coff64Output::write_debug_line_numbers() {
    // Output line numbers for the current function.
    if (!env_->debug()) { return; } 
    CvLineNumbers lineno;
    memset(&lineno, 0, sizeof(lineno));
    lineno.offset = 0; // Fixed by relocations
    lineno.section = 0; // Fixed by relocations
    lineno.section_length = function_.size;//1; //text_->bytes(); // ??
    lineno.source_file = 0; // Only one entry in table, so use 1
    lineno.linenos_count = lineno_.size();
    lineno.linenos_size = lineno_.size()*sizeof(lineno_.front())+12;
    // N.B.: Not sure why the +12 is necessary here...
    
    CvSectionHeader header;
    memset(&header, 0, sizeof(header));
    header.type = CV_LINE_NUMBERS;
    header.size += sizeof(lineno);
    header.size += lineno_.size()*sizeof(lineno_.front());
    header.size = align(header.size, sizeof(uint32_t));

    debug_->buffer((char*)&header, sizeof(header)); 

    uint32_t offset_addr = debug_->bytes()+(char*)&lineno.offset-(char*)&lineno;
    uint32_t section_addr = debug_->bytes()+(char*)&lineno.section-(char*)&lineno;

    debug_reloc_.push_back(reloc(offset_addr, function_name_, IMAGE_REL_AMD64_SECREL));
    debug_reloc_.push_back(reloc(section_addr, function_name_, IMAGE_REL_AMD64_SECTION));

    debug_->buffer((char*)&lineno, sizeof(lineno)); 
    debug_->buffer((char*)&lineno_.front(), lineno_.size()*sizeof(lineno_.front()));
    debug_->align(sizeof(uint32_t));

    lineno_.clear();
}

void Coff64Output::out(Stream* out) {
    // Writes out the full text of the object file format to the given stream.
    IMAGE_FILE_HEADER header;
    IMAGE_SECTION_HEADER text;
    IMAGE_SECTION_HEADER data;
    IMAGE_SECTION_HEADER debugS;
    
    memset(&header, 0, sizeof(header));
    memset(&text, 0, sizeof(text));
    memset(&data, 0, sizeof(data));
    memset(&debugS, 0, sizeof(debugS));

    size_t metadata_bytes = sizeof(header)+sizeof(text)+sizeof(data);
    if (env_->debug()) {
        metadata_bytes += sizeof(debugS);
    }
    size_t offset = metadata_bytes;

    // Header
    header.Machine = IMAGE_FILE_MACHINE_AMD64;
    header.NumberOfSections = 2;
    if (env_->debug()) {
        header.NumberOfSections++;
    }
    header.TimeDateStamp = time(0);
    header.NumberOfSymbols = sym_.size();
    header.SizeOfOptionalHeader = 0;
    header.Characteristics = 0;

    memcpy(text.Name, ".text", sizeof(text.Name));
    text.PointerToRawData = offset;
    text.SizeOfRawData = text_->bytes();
    offset += text.SizeOfRawData;
    text.PointerToRelocations = offset;
    text.NumberOfRelocations = text_reloc_.size();
    offset += text.NumberOfRelocations * sizeof(text_reloc_.front());
    text.Characteristics = 0;
    text.Characteristics |= IMAGE_SCN_MEM_EXECUTE;
    text.Characteristics |= IMAGE_SCN_MEM_READ;
    text.Characteristics |= IMAGE_SCN_CNT_CODE;
    text.Characteristics |= IMAGE_SCN_ALIGN_16BYTES;

    memcpy(data.Name, ".data", sizeof(data.Name));
    data.PointerToRawData = offset;
    data.SizeOfRawData = data_->bytes();
    offset += data.SizeOfRawData;
    data.PointerToRelocations = offset;
    data.NumberOfRelocations = data_reloc_.size();
    offset += data.NumberOfRelocations * sizeof(data_reloc_.front());
    data.Characteristics = 0;
    data.Characteristics |= IMAGE_SCN_MEM_READ;
    data.Characteristics |= IMAGE_SCN_MEM_WRITE;
    data.Characteristics |= IMAGE_SCN_CNT_INITIALIZED_DATA;
    data.Characteristics |= IMAGE_SCN_ALIGN_1BYTES;

    if (env_->debug()) {
        memcpy(debugS.Name, ".debug$S", sizeof(debugS.Name));
        debugS.PointerToRawData = offset;
        debugS.SizeOfRawData = debug_->bytes();
        offset += debugS.SizeOfRawData;
        debugS.PointerToRelocations = offset;
        debugS.NumberOfRelocations = debug_reloc_.size();
        offset += debugS.NumberOfRelocations * sizeof(debug_reloc_.front());
        debugS.Characteristics = 0;
        debugS.Characteristics |= IMAGE_SCN_MEM_READ;
        debugS.Characteristics |= IMAGE_SCN_MEM_DISCARDABLE;
        debugS.Characteristics |= IMAGE_SCN_CNT_INITIALIZED_DATA;
        debugS.Characteristics |= IMAGE_SCN_ALIGN_1BYTES;
    }

    header.PointerToSymbolTable = offset;

    // Write everything out
    out->write((char*)&header, sizeof(header));
    out->write((char*)&text, sizeof(text));
    out->write((char*)&data, sizeof(data));
    if (env_->debug()) {
        out->write((char*)&debugS, sizeof(debugS));
    }
    out->write((char*)text_->text(), text_->bytes());
    if (text_reloc_.size()) {
        out->write((char*)&text_reloc_.front(), text_reloc_.size()*sizeof(text_reloc_.front()));
    }
    out->write((char*)data_->text(), data_->bytes());
    if (data_reloc_.size()) {
        out->write((char*)&data_reloc_.front(), data_reloc_.size()*sizeof(data_reloc_.front()));
    }
    if (env_->debug()) {
        out->write((char*)debug_->text(), debug_->bytes());
        if (debug_reloc_.size()) {
            out->write((char*)&debug_reloc_.front(), debug_reloc_.size()*sizeof(debug_reloc_.front()));
        }
    }
    if (sym_.size()) {
        out->write((char*)&sym_.front(), sym_.size()*sizeof(sym_.front()));
    }
    uint32_t const strsize = string_->bytes() + sizeof(strsize);
    out->write((char*)&strsize, sizeof(strsize));
    out->write((char*)string_->text(), string_->bytes());
    out->flush();
}

IMAGE_RELOCATION Coff64Output::reloc(uint32_t addr, String* name, uint16_t type) {
    IMAGE_RELOCATION reloc; // Relocate the function offset
    reloc.VirtualAddress = addr;
    reloc.SymbolTableIndex = symbol_.find(name)->second;
    reloc.Type = type;
    return reloc;
}

#endif

