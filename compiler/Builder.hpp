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

#include "Apollo.hpp"
#include "Environment.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"
#include "BasicBlockGenerator.hpp"
#include "RegisterAllocator.hpp"
#include "Intel64CodeGenerator.hpp"
#include "CCodeGenerator.hpp"
#include "BasicBlockPrinter.hpp"
#include "DeadCodeEliminator.hpp"
#include "File.hpp"
#include "CopyPropagator.hpp"
#include "TreePrinter.hpp"
#include "Machine.hpp"
#include "InterfaceGenerator.hpp"

/* Builds modules, executables, and dependencies in order. */

class Builder : public TreeNode::Functor {
public:
    Builder(Environment* env);
    typedef Pointer<Builder> Ptr;
    
    void operator()(Module* module) {}
    void operator()(File* file);

private:
    Environment::Ptr env_;
    Parser::Ptr parser_;
    Machine::Ptr machine_;
    BasicBlockGenerator::Ptr bgen_;
    CopyPropagator::Ptr copy_;
    DeadCodeEliminator::Ptr elim_;
    RegisterAllocator::Ptr alloc_;
    Intel64CodeGenerator::Ptr intel64gen_;
    CCodeGenerator::Ptr cgen_;
};
