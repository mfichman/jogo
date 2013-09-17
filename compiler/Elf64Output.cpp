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

#ifdef LINUX

#include "Elf64Output.hpp"
#include <cassert>
#include <iostream>

Elf64Output::Elf64Output() :
    text_(new Section),
    data_(new Section),
    string_(new Section) {

    string_->uint8(0); // Reserved for ""
    Elf64_Sym sym;
    memset(&sym, 0, sizeof(sym));
    sym_.push_back(sym); // Reserved for STN_UNDEF
}

void Elf64Output::ref(String* label, RelocType rtype) {
    // Refer to a symbol by name at the current text/data output location.
    // This function adds an entry to the relocation table.  The size can be
    // either 4 or 8 bytes.  A 4-byte entry indicates the instruction uses
    // RIP-relative addressing for x64.
    std::map<String::Ptr,size_t>::iterator i = symbol_.find(label);
    size_t symnum = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        Elf64_Sym* const sym = &sym_.back();
        sym->st_name = string_->bytes();
        sym->st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
        sym->st_other = 0;
        sym->st_shndx = SHN_UNDEF;
        sym->st_value = 0;
        sym->st_size = 0; // Unknown size
        string_->buffer(label->string().c_str(), label->string().size()+1);
        symnum = sym_.size()-1;
    } else {
        symnum = i->second;
    }

    Elf64_Rela reloc;
    if (REF_TEXT == rtype) {
        reloc.r_offset = text_->bytes();
        reloc.r_info = ELF64_R_INFO(symnum, R_X86_64_64);
        reloc.r_addend = 0;
        text_reloc_.push_back(reloc);
    } else if (REF_DATA == rtype || REF_VTABLE == rtype) {
        reloc.r_offset = data_->bytes();
        reloc.r_info = ELF64_R_INFO(symnum, R_X86_64_64);
        reloc.r_addend = 0;
        data_reloc_.push_back(reloc);
    } else if (REF_BRANCH == rtype || REF_CALL == rtype) {
        reloc.r_offset = text_->bytes();
        reloc.r_info = ELF64_R_INFO(symnum, R_X86_64_PC32);
        reloc.r_addend = -sizeof(uint32_t);
        text_reloc_.push_back(reloc);
    } else if (REF_SIGNED == rtype) {
        reloc.r_offset = text_->bytes();
        reloc.r_info = ELF64_R_INFO(symnum, R_X86_64_PC32);
        reloc.r_addend = -sizeof(uint32_t);
        text_reloc_.push_back(reloc);
    } 
}

void Elf64Output::sym(String* label, SymType type) {
    // Finds the symbol table entry for 'label' and updates it to the current
    // offset of the text or data segment.
    std::map<String::Ptr,size_t>::iterator i = symbol_.find(label);
    Elf64_Sym* sym = 0;
    if (i == symbol_.end()) {
        symbol_.insert(std::make_pair(label, sym_.size()));
        sym_.resize(sym_.size()+1);
        sym = &sym_.back();
        sym->st_name = string_->bytes();
        sym->st_other = 0;
        sym->st_size = 0; // FixMe: May need to set this explicitly
        // Offset to name in string table (+4 for strtab size header)
        string_->buffer(label->string().c_str(), label->string().size()+1);
    } else {
        sym = &sym_[i->second];
        assert("Duplicate label"&&(sym->st_shndx==0)); 
    }
    if (type & SYM_TEXT) {
        sym->st_value = text_->bytes();
        sym->st_shndx = OUT_SECT_TEXT;
    } else if (type & SYM_DATA) {
        sym->st_value = data_->bytes();
        sym->st_shndx = OUT_SECT_DATA;
    } else {
        assert(!"Unknown section type");
    }
    if (type & SYM_LOCAL) {
        sym->st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    } else {
        sym->st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    } 
}

void Elf64Output::sort_symtab() {
    // With ELF, the local symbols must come first in the symbol table.
    // Annoying, but true.  As a result, this function is necessary to 
    // reorder the symbols with the local ones first.

    std::vector<Elf64_Sym> symtmp;
    std::vector<size_t> redirect(sym_.size());
    // Mapping from old => new position of the symbol in the symbol table.
    for (int i = 0; i < sym_.size(); ++i) {
        if (ELF64_ST_BIND(sym_[i].st_info) == STB_LOCAL) {
            redirect[i] = symtmp.size(); 
            symtmp.push_back(sym_[i]);
        }
    } 
    local_syms_ = symtmp.size();
    for (int i = 0; i < sym_.size(); ++i) {
        if (ELF64_ST_BIND(sym_[i].st_info) == STB_GLOBAL) {
            redirect[i] = symtmp.size();
            symtmp.push_back(sym_[i]);
        }
    }
    for (int i = 0; i < text_reloc_.size(); ++i) {
        Elf64_Word type = ELF64_R_TYPE(text_reloc_[i].r_info);
        Elf64_Word sym = redirect[ELF64_R_SYM(text_reloc_[i].r_info)];
        text_reloc_[i].r_info = ELF64_R_INFO(sym, type);
    }
    for (int i = 0; i < data_reloc_.size(); ++i) {
        Elf64_Word type = ELF64_R_TYPE(data_reloc_[i].r_info);
        Elf64_Word sym = redirect[ELF64_R_SYM(data_reloc_[i].r_info)];
        assert(sym<sym_.size());
        data_reloc_[i].r_info = ELF64_R_INFO(sym, type);
    }
    sym_ = symtmp;
}

void Elf64Output::out(Stream* out) {
    // Writes out the full text of the object file format to the stream.
    Elf64_Ehdr header;
    Elf64_Shdr null;
    Elf64_Shdr text;
    Elf64_Shdr data;
    Elf64_Shdr reltext;
    Elf64_Shdr reldata;
    Elf64_Shdr symtab;
    Elf64_Shdr strtab;

    sort_symtab(); // Fix local/global symbol ordering

    memset(&null, 0, sizeof(null));
    memset(&header, 0, sizeof(header));
    memset(&text, 0, sizeof(text));
    memset(&data, 0, sizeof(data));
    memset(&reltext, 0, sizeof(reltext));
    memset(&reldata, 0, sizeof(reldata));
    memset(&symtab, 0, sizeof(symtab));
    memset(&strtab, 0, sizeof(strtab));

    size_t offset = sizeof(header);
    offset += sizeof(null);
    offset += sizeof(text);
    offset += sizeof(data);
    offset += sizeof(reltext);
    offset += sizeof(reldata);
    offset += sizeof(symtab);
    offset += sizeof(strtab);

    // Header setup
    header.e_ident[EI_MAG0] = '\x7f';
    header.e_ident[EI_MAG1] = 'E';
    header.e_ident[EI_MAG2] = 'L';
    header.e_ident[EI_MAG3] = 'F';
    header.e_ident[EI_CLASS] = ELFCLASS64;
    header.e_ident[EI_DATA] = ELFDATA2LSB;
    header.e_ident[EI_VERSION] = EV_CURRENT;
    header.e_ident[EI_OSABI] = ELFOSABI_SYSV;
    header.e_ident[EI_ABIVERSION] = 0;
    header.e_type = ET_REL;
    header.e_machine = EM_X86_64;
    header.e_version = EV_CURRENT;
    header.e_entry = 0;
    header.e_phoff = 0; // FIXME: Should this be set?
    header.e_shoff = sizeof(header);
    header.e_flags = 0;// processor dep
    header.e_ehsize = sizeof(header);
    header.e_phentsize = 0;
    header.e_phnum = 0; // FIXME: Should this be set?
    header.e_shentsize = sizeof(text);
    header.e_shnum = OUT_SECT_LAST+1;
    header.e_shstrndx = OUT_SECT_STRTAB; // Section header name string table

    // Null section header - required for SHN_UNDEF
    // Text section
    text.sh_name = string_->bytes();
    char const text_name[] = ".text";
    string_->buffer(text_name, sizeof(text_name));
    text.sh_type = SHT_PROGBITS;
    text.sh_flags = SHF_EXECINSTR|SHF_ALLOC;
    text.sh_addr = 0;
    text.sh_offset = offset;
    text.sh_size = text_->bytes();
    offset += text.sh_size;
    text.sh_link = SHN_UNDEF;
    text.sh_info = 0;
    text.sh_addralign = 16;
    text.sh_entsize = 0;
    
    // Data section 
    data.sh_name = string_->bytes();;
    char const data_name[] = ".data";
    string_->buffer(data_name, sizeof(data_name));
    data.sh_type = SHT_PROGBITS;
    data.sh_flags = SHF_WRITE|SHF_ALLOC;
    data.sh_addr = 0;
    data.sh_offset = offset;
    data.sh_size = data_->bytes();
    offset += data.sh_size;
    data.sh_link = SHN_UNDEF;
    data.sh_info = 0;
    data.sh_addralign = 16;
    data.sh_entsize = 0;
    
    // Text relocation
    reltext.sh_name = string_->bytes();
    char const reltext_name[] = ".rela.text";
    string_->buffer(reltext_name, sizeof(reltext_name));
    reltext.sh_type = SHT_RELA;
    reltext.sh_flags = 0;
    reltext.sh_addr = 0;
    reltext.sh_offset = offset;
    reltext.sh_size = sizeof(text_reloc_.front())*text_reloc_.size();
    offset += reltext.sh_size;
    reltext.sh_link = OUT_SECT_SYMTAB;
    reltext.sh_info = OUT_SECT_TEXT; // relocs apply to text section
    reltext.sh_addralign = 4;
    reltext.sh_entsize = sizeof(text_reloc_.front());
    
    // Data relocation
    reldata.sh_name = string_->bytes();
    char const reldata_name[] = ".rela.data";
    string_->buffer(reldata_name, sizeof(reldata_name));
    reldata.sh_type = SHT_RELA;
    reldata.sh_flags = 0;
    reldata.sh_addr = 0;
    reldata.sh_offset = offset;
    reldata.sh_size = sizeof(data_reloc_.front())*data_reloc_.size();
    offset += reldata.sh_size;
    reldata.sh_link = OUT_SECT_SYMTAB;
    reldata.sh_info = OUT_SECT_DATA; // relocs apply to data section
    reldata.sh_addralign = 4;
    reldata.sh_entsize = sizeof(data_reloc_.front());

    // Symbol table
    symtab.sh_name = string_->bytes();
    char const symtab_name[] = ".symtab";
    string_->buffer(symtab_name, sizeof(symtab_name));
    symtab.sh_type = SHT_SYMTAB;
    symtab.sh_flags = 0;
    symtab.sh_addr = 0;
    symtab.sh_offset = offset;
    symtab.sh_size = sizeof(sym_.front())*sym_.size();
    offset += symtab.sh_size;
    symtab.sh_link = OUT_SECT_STRTAB;
    symtab.sh_info = local_syms_;
    symtab.sh_addralign = 4;
    symtab.sh_entsize = sizeof(sym_.front());

    // String table
    strtab.sh_name = string_->bytes();
    char const strtab_name[] = ".strtab";
    string_->buffer(strtab_name, sizeof(strtab_name));
    strtab.sh_type = SHT_STRTAB;
    strtab.sh_flags = 0;
    strtab.sh_addr = 0;
    strtab.sh_offset = offset;
    strtab.sh_size = string_->bytes();
    offset += strtab.sh_size;
    strtab.sh_link = SHN_UNDEF;
    strtab.sh_info = 0;
    strtab.sh_addralign = 1;
    strtab.sh_entsize = 0;
      
    out->write((char*)&header, sizeof(header));
    out->write((char*)&null, sizeof(null));
    out->write((char*)&text, sizeof(text));
    out->write((char*)&data, sizeof(data));    
    out->write((char*)&reltext, sizeof(reltext));
    out->write((char*)&reldata, sizeof(reldata));
    out->write((char*)&symtab, sizeof(symtab));
    out->write((char*)&strtab, sizeof(strtab));
    out->write((char*)text_->text(), text_->bytes());
    out->write((char*)data_->text(), data_->bytes());
    out->write((char*)&text_reloc_.front(), text_reloc_.size()*sizeof(text_reloc_.front()));
    out->write((char*)&data_reloc_.front(), data_reloc_.size()*sizeof(data_reloc_.back()));
    out->write((char*)&sym_.front(), sym_.size()*sizeof(sym_.front()));
    out->write((char*)string_->text(), string_->bytes());
    out->flush();
 
}

#endif




















