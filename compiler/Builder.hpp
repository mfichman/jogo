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
#include "Environment.hpp"
#include "File.hpp"
#include "Machine.hpp"
#include "ProcessGroup.hpp"

/* Builds modules, executables, and dependencies in order. */
class Builder : public TreeNode::Functor {
public:
    Builder(Environment* env);
    typedef Pointer<Builder> Ptr;
    
    void operator()(Module* module);
    void operator()(File* file);
    int errors() const { return errors_; }

private:
    void process_path();
    void modular_build();
    void monolithic_build();
    void link(Module* module);
    void link(const std::string& in, const std::string& out);
    void archive(Module* module);
    void archive(const std::string& in, const std::string& out);
    void irgen(File* file); 
    void cgen(File* file);
    void nasm64gen(File* file);
    void intel64gen(File* file);
    void cc(const std::string& in, const std::string& out);
    void nasm(const std::string& in, const std::string& out);
    void execute(const std::string& exe);

    Environment::Ptr env_;
    ProcessGroup procs_;
    int errors_;
    std::string vcvarsall_;
};
