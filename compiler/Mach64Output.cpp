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

#ifdef DARWIN

#include "Mach64Output.hpp"
#include <mach/vm_prot.h>
#include <cassert>
#include <iostream>

Mach64Output::Mach64Output() : 
    text_(new Section), 
    data_(new Section), 
    string_(new Section) {

    string_->uint8(0);
    // The string table must have a 0-byte at the beginning, b/c the
    // nlist_64 struct uses an offset of 0 into the string table to
    // represent a 0-length string.
}

void Mach64Output::ref(String* label, RelocType rtype) { 
    // Refer to a symbol by name at the current text output location.  This
    // function adds an entry to the relocation table.  The size can be either
    // 4 (bytes) or 8 (bytes).  A 4-byte entry here indicates an instruction 
    // with a 4-byte immediate operand that uses RIP-relative addressing. 
    // A 8-byte entry indicates an absolute address.
    std::map<String::Ptr,int>::iterator i = symbol_.find(label);  
    int symnum = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        nlist_64* const sym = &sym_.back();
        sym->n_un.n_strx = string_->bytes(); // Offset to name in string table
        sym->n_type = N_UNDF|N_EXT; // Symbol is undefined.
        sym->n_sect = NO_SECT; // Not known yet.
        sym->n_value = 0; // Address of the symbol unknown.
        if (label->string()[0] != '.') {
            string_->uint8('_');
        }
        string_->buffer(label->string().c_str(), label->string().size()+1);  
        symnum = sym_.size()-1;
        // FixMe: Encode a string's name properly if it is a literal
    } else {
        symnum = i->second;
    }

    // FIXME: This is unfortunately very x86-specific.  It deals with the
    // difference between 64-bit immediate addresses and RIP-based addresses.
    // Also, the Mach-O file format distinguishes between branch RIP-based
    // addresses and non-branch RIP-based addresses, even though theoretically
    // they have the same addr length and addressing semantics.

    relocation_info reloc;
    reloc.r_symbolnum = symnum;

    if (REF_TEXT == rtype) {
        reloc.r_address = text_->bytes();
        reloc.r_pcrel = 0;
        reloc.r_length = 3; // 3=absolute 
        reloc.r_extern = 1;
        reloc.r_type = X86_64_RELOC_UNSIGNED;
        text_reloc_.push_back(reloc);
    } else if (REF_DATA == rtype || REF_VTABLE == rtype) {
        reloc.r_address = data_->bytes();
        reloc.r_pcrel = 0;
        reloc.r_length = 3; // 3=absolute 
        reloc.r_extern = 1;
        reloc.r_type = X86_64_RELOC_UNSIGNED;
        data_reloc_.push_back(reloc);
    } else if (REF_BRANCH == rtype || REF_CALL == rtype) {
        reloc.r_address = text_->bytes();
        reloc.r_pcrel = 1; 
        reloc.r_length = 2; // Length of address (0=1, 1=2, 2=4)
        reloc.r_extern = 1; // Look in the symbol table
        reloc.r_type = X86_64_RELOC_BRANCH;
        text_reloc_.push_back(reloc);
    } else if (REF_SIGNED == rtype) {
        reloc.r_address = text_->bytes();
        reloc.r_pcrel = 1;
        reloc.r_length = 2; // Length of address (0=1, 1=2, 2=4)
        reloc.r_extern = 1; 
        reloc.r_type = X86_64_RELOC_SIGNED;
        text_reloc_.push_back(reloc);
    } else {
        assert(!"Invalid relocation type");
    }
}

void Mach64Output::sym(String* label, SymType type) {
    // Finds the symbol table entry for 'label' and updates it to the current
    // offset of the text segment.
    std::map<String::Ptr,int>::iterator i = symbol_.find(label);  
    nlist_64* sym = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        sym = &sym_.back();
        sym->n_un.n_strx = string_->bytes(); // Offset to name in string table
        if (label->string()[0] != '.') {
            string_->uint8('_');
        }
        string_->buffer(label->string().c_str(), label->string().size()+1);  
    } else {
        sym = &sym_[i->second];
        assert("Duplicate label"&&(sym->n_type==(N_UNDF|N_EXT)));
    }
    sym->n_type = N_SECT;
    if (!(type & SYM_LOCAL)) {
        sym->n_type |= N_EXT;
    }
    if (type & SYM_TEXT) {
        sym->n_sect = OUT_SECT_TEXT; 
        sym->n_value = text_->bytes();
    } else if (type & SYM_DATA) {
        sym->n_sect = OUT_SECT_DATA;
        sym->n_value = data_->bytes(); 
    } else {
        assert(!"Unknown section type");
    }
}

void Mach64Output::out(Stream* out) {
    // Writes out the full text of the object file format to the path given by
    // the 'path' arg

    // Clean up symbol table entries for the data segment.  Add the size of the
    // text segment to each symbol address in that section.
    for (size_t i = 0; i < sym_.size(); ++i) {
        if (sym_[i].n_sect == OUT_SECT_DATA) {
            sym_[i].n_value += text_->bytes();
        }
    }

    segment_command_64 seg;
    symtab_command sym;  
    mach_header_64 header;
    section_64 text;
    section_64 data;

    memset(&seg, 0, sizeof(seg));
    memset(&sym, 0, sizeof(sym));
    memset(&header, 0, sizeof(header));
    memset(&text, 0, sizeof(text));
    memset(&data, 0, sizeof(data));

    size_t const seg_bytes = sizeof(seg)+sizeof(text)+sizeof(data);
    size_t const cmd_bytes = sizeof(sym)+seg_bytes;
    size_t const metadata_bytes = sizeof(header)+cmd_bytes;


    // Header
    header.magic = MH_MAGIC_64;
    header.cputype = CPU_TYPE_X86_64;
    header.cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    header.filetype = MH_OBJECT;
    header.ncmds = 2; // 1 segment, one symtab
    header.sizeofcmds = cmd_bytes;
    header.flags = 0;
    header.reserved = 0;

    // Text segment command
    seg.cmd = LC_SEGMENT_64;
    seg.cmdsize = seg_bytes;
    memset(seg.segname, 0, sizeof(seg.segname));
    seg.vmaddr = 0;
    seg.vmsize = text_->bytes()+data_->bytes();
    seg.fileoff = metadata_bytes;
    seg.maxprot = VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE;
    seg.initprot = VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE;
    seg.nsects = 2;
    seg.filesize = text_->bytes()+data_->bytes();
    seg.flags = 0;

    size_t offset = seg.fileoff;

    // Text section
    strncpy(text.sectname, "__text", sizeof(text.sectname)); 
    strncpy(text.segname, "__TEXT", sizeof(text.segname));
    text.addr = 0;
    text.offset = offset;
    text.size = text_->bytes();
    offset += text.size;
    text.align = 8;
    text.flags = 0;
    text.flags |= S_REGULAR;
    text.flags |= S_ATTR_EXT_RELOC;
    text.flags |= S_ATTR_LOC_RELOC;
    text.flags |= S_ATTR_PURE_INSTRUCTIONS;
    text.reserved1 = 0;
    text.reserved2 = 0;
    text.reserved3 = 0;

    // Data section
    strncpy(data.sectname, "__data", sizeof(data.sectname)); 
    strncpy(data.segname, "__DATA", sizeof(data.segname));
    data.addr = text.size;
    data.offset = offset;
    data.size = data_->bytes();
    offset += data.size;
    data.align = 8;
    data.flags = 0;
    data.flags |= S_REGULAR;
    data.flags |= S_ATTR_EXT_RELOC;
    data.flags |= S_ATTR_LOC_RELOC;
    data.reserved1 = 0;
    data.reserved2 = 0;
    data.reserved3 = 0;

    // Relocation tables
    text.reloff = offset;
    text.nreloc = text_reloc_.size();
    offset += text.nreloc * sizeof(text_reloc_.front());
    data.reloff = offset;
    data.nreloc = data_reloc_.size();
    offset += data.nreloc * sizeof(data_reloc_.front());

    // Symbol table command
    sym.cmd = LC_SYMTAB;
    sym.cmdsize = sizeof(sym);
    sym.symoff = offset;
    sym.nsyms = sym_.size();
    offset += sym.nsyms * sizeof(sym_.front());
    sym.stroff = offset;
    sym.strsize = string_->bytes();
    offset += string_->bytes();

    // Write everything to the output stream.
    out->write((char*)&header, sizeof(header));
    out->write((char*)&seg, sizeof(seg));
    out->write((char*)&text, sizeof(text));
    out->write((char*)&data, sizeof(data));
    out->write((char*)&sym, sizeof(sym));
    out->write((char*)text_->text(), text_->bytes());
    out->write((char*)data_->text(), data_->bytes());
    out->write((char*)&text_reloc_.front(), text_reloc_.size()*sizeof(text_reloc_.front()));
    out->write((char*)&data_reloc_.front(), data_reloc_.size()*sizeof(data_reloc_.front()));
    out->write((char*)&sym_.front(), sym_.size() * sizeof(sym_.front()));
    out->write((char*)string_->text(), string_->bytes());
    out->flush();
    // sizeof(relocation_info) == 8
}

#endif
