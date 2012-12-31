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

void
Mach64Output::ref(String* label, RelocType type) { 
    // Refer to a symbol by name at the current text output location.  This
    // function adds an entry to the relocation table.  The size can be either
    // 4 (bytes) or 8 (bytes).  A 4-byte entry here indicates an instruction 
    // with a 4-byte immediate operand that uses RIP-relative addressing. 
    // A 8-byte entry indicates an absolute address.
    std::map<String::Ptr,int>::iterator i = symbol_.find(label);  
    int symnum = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, symtab_.size()));
        symtab_.resize(symtab_.size()+1);
        nlist_64* const sym = &symtab_.back();
        sym->n_un.n_strx = string_->bytes(); // Offset to name in string table
        sym->n_type = N_UNDF|N_EXT; // Symbol is undefined.
        sym->n_sect = NO_SECT; // Not known yet.
        sym->n_value = 0; // Address of the symbol unknown.
        string_->uint8('_');
        string_->buffer(label->string().c_str(), label->string().size()+1);  
        symnum = symtab_.size()-1;
        // FixMe: Encode a string's name properly if it is a literal
    } else {
        symnum = i->second;
    }
    relocation_info reloc;
    reloc.r_address = text_->bytes();
    reloc.r_symbolnum = symnum;

    // FIXME: This is unfortunately very x86-specific.  It deals with the
    // difference between 64-bit immediate addresses and RIP-based addresses.
    // Also, the Mach-O file format distinguishes between branch RIP-based
    // addresses and non-branch RIP-based addresses, even though theoretically
    // they have the same addr length and addressing semantics.
    if (RELOC_ABSOLUTE == type) {
        reloc.r_pcrel = 0;
        reloc.r_length = 3; // 3=absolute 
        reloc.r_extern = 1;
        reloc.r_type = X86_64_RELOC_UNSIGNED;
    } else if (RELOC_BRANCH == type) {
        reloc.r_pcrel = 1; 
        reloc.r_length = 2; // Length of address (0=1, 1=2, 2=4)
        reloc.r_extern = 1; // Look in the symbol table
        reloc.r_type = X86_64_RELOC_BRANCH;
    } else if (RELOC_SIGNED == type) {
        reloc.r_pcrel = 1;
        reloc.r_length = 2; // Length of address (0=1, 1=2, 2=4)
        reloc.r_extern = 1; 
        reloc.r_type = X86_64_RELOC_SIGNED;
    }
    // For an address 
    reloc_.push_back(reloc);
}

void
Mach64Output::label(String* label) {
    // Finds the symbol table entry for 'label' and updates it to the current
    // offset of the text segment.
    std::map<String::Ptr,int>::iterator i = symbol_.find(label);  
    nlist_64* sym = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, symtab_.size()));
        symtab_.resize(symtab_.size()+1);
        sym = &symtab_.back();
        sym->n_un.n_strx = string_->bytes(); // Offset to name in string table
        string_->uint8('_');
        string_->buffer(label->string().c_str(), label->string().size()+1);  
    } else {
        sym = &symtab_[i->second];
        assert("Duplicate label"&&(sym->n_type==(N_UNDF|N_EXT)));
    }

    sym->n_type = N_SECT|N_EXT;
    sym->n_sect = TEXT_SECT; 
    sym->n_value = text_->bytes();
}

void
Mach64Output::local(String* label) {
    // Finds the symbol table entry for 'label' and updates it to the current
    // offset of the text segment.
    std::map<String::Ptr,int>::iterator i = symbol_.find(label);  
    nlist_64* sym = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, symtab_.size()));
        symtab_.resize(symtab_.size()+1);
        sym = &symtab_.back();
        sym->n_un.n_strx = string_->bytes(); // Offset to name in string table
        string_->uint8('_');
        string_->buffer(label->string().c_str(), label->string().size()+1);  
    } else {
        sym = &symtab_[i->second];
        assert("Duplicate label"&&(sym->n_type==(N_UNDF|N_EXT)));
    }

    sym->n_type = N_SECT;
    sym->n_sect = TEXT_SECT; 
    sym->n_value = text_->bytes();
}

void
Mach64Output::out(Stream* out) {
    // Writes out the full text of the object file format to the path given by
    // the 'path' arg
    segment_command_64 seg;
    symtab_command sym;  
    mach_header_64 header;
    section_64 text;

    size_t const reloc_bytes = sizeof(reloc_.front())*reloc_.size();
    size_t const symtab_bytes = sizeof(symtab_.front())*symtab_.size();
    size_t const cmd_bytes = sizeof(seg)+sizeof(sym)+sizeof(text);
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
    seg.cmdsize = sizeof(seg)+sizeof(text); 
    memset(seg.segname, 0, sizeof(seg.segname));
    seg.vmaddr = 0;
    seg.vmsize = text_->bytes();
    seg.fileoff = metadata_bytes;
    seg.maxprot = VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE;
    seg.initprot = VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE;
    seg.nsects = 1;
    seg.filesize = text_->bytes();
    seg.flags = 0;

    // Text section
    strncpy(text.sectname, "__text", sizeof(text.sectname)); 
    strncpy(text.segname, "__TEXT", sizeof(text.segname));
    text.addr = 0;
    text.offset = seg.fileoff;
    text.size = text_->bytes();
    text.align = 0;
    text.reloff = seg.fileoff + text_->bytes();
    text.nreloc = reloc_.size();
    text.flags = 0;
    text.flags |= S_REGULAR;
    text.flags |= S_ATTR_EXT_RELOC;
    text.flags |= S_ATTR_LOC_RELOC;
    text.flags |= S_ATTR_SOME_INSTRUCTIONS;
    text.reserved1 = 0;
    text.reserved2 = 0;
    text.reserved3 = 0;

    // Symbol table command
    sym.cmd = LC_SYMTAB;
    sym.cmdsize = sizeof(sym);
    sym.symoff = text.reloff + reloc_bytes;
    sym.nsyms = symtab_.size();
    sym.stroff = sym.symoff + symtab_bytes;
    sym.strsize = string_->bytes();

    // Write everything to the output stream.
    out->write((char const*)&header, sizeof(header));
    out->write((char const*)&seg, sizeof(seg));
    out->write((char const*)&text, sizeof(text));
    out->write((char const*)&sym, sizeof(sym));
    out->write((char const*)text_->text(), text_->bytes());
    out->write((char const*)&reloc_.front(), reloc_bytes);
    out->write((char const*)&symtab_.front(), symtab_bytes);
    out->write((char const*)string_->text(), string_->bytes());
    out->flush();
}

#endif
