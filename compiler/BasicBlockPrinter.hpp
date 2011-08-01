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
#include "TreeNode.hpp"
#include "RegisterAllocator.hpp"
#include "LivenessAnalyzer.hpp"
#include "BasicBlock.hpp"
#include <set>

class BasicBlockPrinter : public TreeNode::Functor {
public:
    BasicBlockPrinter(Environment* env, Machine* mach);
    typedef Pointer<BasicBlockPrinter> Ptr;

    void operator()(Function* feature);
private:
    void operator()(Module* feature);
    void operator()(Class* feature);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Dispatch* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(Empty* empty);
    void operator()(Let* statement);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Variable* statement);
    void operator()(Return* statement);
    void operator()(When* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);
    void operator()(Attribute* feature);
    void operator()(Import* feature);
    void operator()(Type* type);
    void operator()(BasicBlock* block);

    Environment::Ptr environment_;
    LivenessAnalyzer::Ptr liveness_;
    Module::Ptr module_;
    Class::Ptr class_;
};
