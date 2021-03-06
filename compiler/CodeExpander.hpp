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
#include "Object.hpp"
#include "TreeNode.hpp"
#include "SemanticAnalyzer.hpp"

/* Expands code prior to generation */
class CodeExpander : public TreeNode::Functor {
public:
    CodeExpander(Environment* env, SemanticAnalyzer* semant);
    typedef Pointer<CodeExpander> Ptr;

private:
    void operator()(Class* feature);
    void operator()(Module* feature);
    void operator()(Function* feature);
    void functor(Class* funct);
    void component(Attribute* attr);
    void stub(Function* func, Attribute* attr);
    void mutator(Attribute* feature);
    void accessor(Attribute* feature);
    void constructor();
    void destructor();
    void copier();

    Environment::Ptr env_;
    SemanticAnalyzer::Ptr semant_;
    Class::Ptr class_;
};
