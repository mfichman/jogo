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
#include "TreeNode.hpp"

#undef OPTIONAL

/* 
 * Class for imports.  Qualified imports only load the module corresponding to
 * the import; non-qualified imports also add any names in the loaded module to
 * the file's namespace.
 */
class Import : public TreeNode {
public:
    typedef int Flags;

    Import(Location loc, String* scope, Flags flags) :
        TreeNode(loc),
        file_name_(file_name(scope->string())),
        scope_(scope),
        flags_(flags) {
    }
        
    std::string const& file_name() const { return file_name_; }
    String* scope() const { return scope_; }
    bool is_qualified() const { return flags_ & QUALIFIED; }
    bool is_optional() const { return flags_ & OPTIONAL; }
    static std::string file_name(std::string const& scope);
    static std::string scope_name(std::string const& file);
    static std::string module_name(std::string const& file);
    static std::string parent_scope(std::string const& scope);
    Import* next() const { return next_; }
    Import* last() const { return last_; }
    void next(Import* next) { next_ = next; }
    void last(Import* last) { last_ = last; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Import> Ptr;

    static int const QUALIFIED = 0x1;
    static int const OPTIONAL = 0x2;

private:
    Import::Ptr next_;
    Import::Ptr last_;
    std::string file_name_;
    String::Ptr scope_;
    Flags flags_;
};

