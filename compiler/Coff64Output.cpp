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

Coff64Output::Coff64Output() :
    text_(new Section),
    data_(new Section),
    string_(new Section) {
    
    string_->uint8(0); // ?
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
            //sym->Type = IMAGE_SYM_DTYPE_FUNCTION << 8;
            sym->Type = IMAGE_SYM_DTYPE_NULL << 8; 
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
            //sym->Type = IMAGE_SYM_DTYPE_FUNCTION << 8;
            sym->Type = IMAGE_SYM_DTYPE_NULL << 8;
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

void Coff64Output::out(Stream* out) {
    // Writes out the full text of the object file format to the given stream.

    IMAGE_FILE_HEADER header;
    IMAGE_SECTION_HEADER text;
    IMAGE_SECTION_HEADER data;
    
    memset(&header, 0, sizeof(header));
    memset(&text, 0, sizeof(text));
    memset(&data, 0, sizeof(data));

    size_t const metadata_bytes = sizeof(header)+sizeof(text)+sizeof(data);
    size_t offset = metadata_bytes;

    // Header
    header.Machine = IMAGE_FILE_MACHINE_AMD64;
    header.NumberOfSections = 2;
    header.TimeDateStamp = time(0);
    header.NumberOfSymbols = sym_.size();
    header.SizeOfOptionalHeader = 0;
    header.Characteristics = 0;

    memcpy(text.Name, ".text", sizeof(text.Name));
    text.Misc.VirtualSize = 0; // Not valid for object files
    text.VirtualAddress = 0;
    text.PointerToRawData = offset;
    text.SizeOfRawData = text_->bytes();
    offset += text.SizeOfRawData;
    text.PointerToRelocations = offset;
    text.NumberOfRelocations = text_reloc_.size();
    offset += text.NumberOfRelocations * sizeof(text_reloc_.front());
    text.PointerToLinenumbers = 0;
    text.NumberOfLinenumbers = 0;
    text.Characteristics = 0;
    text.Characteristics |= IMAGE_SCN_MEM_EXECUTE;
    text.Characteristics |= IMAGE_SCN_MEM_READ;
    text.Characteristics |= IMAGE_SCN_CNT_CODE;
    text.Characteristics |= IMAGE_SCN_ALIGN_16BYTES;

    memcpy(data.Name, ".data", sizeof(data.Name));
    data.Misc.VirtualSize = 0; // Not valid
    data.VirtualAddress = 0;
    data.PointerToRawData = offset;
    data.SizeOfRawData = data_->bytes();
    offset += data.SizeOfRawData;
    data.PointerToRelocations = offset;
    data.NumberOfRelocations = data_reloc_.size();
    offset += data.NumberOfRelocations * sizeof(data_reloc_.front());
    data.PointerToLinenumbers = 0;
    data.NumberOfLinenumbers = 0;
    data.Characteristics = 0;
    data.Characteristics |= IMAGE_SCN_MEM_READ;
    data.Characteristics |= IMAGE_SCN_MEM_WRITE;
    data.Characteristics |= IMAGE_SCN_CNT_INITIALIZED_DATA;

    header.PointerToSymbolTable = offset;

    // Write everything out
    out->write((char*)&header, sizeof(header));
    out->write((char*)&text, sizeof(text));
    out->write((char*)&data, sizeof(data));
    out->write((char*)text_->text(), text_->bytes());
    if (text_reloc_.size()) {
        out->write((char*)&text_reloc_.front(), text_reloc_.size()*sizeof(text_reloc_.front()));
    }
    out->write((char*)data_->text(), data_->bytes());
    if (data_reloc_.size()) {
        out->write((char*)&data_reloc_.front(), data_reloc_.size()*sizeof(data_reloc_.front()));
    }
    if (sym_.size()) {
        out->write((char*)&sym_.front(), sym_.size()*sizeof(sym_.front()));
    }
    uint32_t const strsize = string_->bytes() + sizeof(strsize);
    out->write((char*)&strsize, sizeof(strsize));
    out->write((char*)string_->text(), string_->bytes());
    out->flush();
}

#endif

