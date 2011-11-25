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

#include "Apollo.hpp"
#include "Environment.hpp"
#include "BasicBlock.hpp"
#include "LivenessAnalyzer.hpp"
#include "Object.hpp"

/* Propagates copies (MOV) throughout a basic block */
class CopyPropagator : public TreeNode::Functor {
public:
    CopyPropagator(Environment* env, Machine* machine);

    typedef Pointer<CopyPropagator> Ptr;
    void operator()(File* file);
    void operator()(Module* feature);
    void operator()(Class* feature);
    void operator()(Function* feature);
    void operator()(BasicBlock* block);

private:
    Environment::Ptr env_;
    File::Ptr file_;
    Machine::Ptr machine_;
};
