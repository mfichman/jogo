/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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
#include "BasicBlock.hpp"
#include "Object.hpp"
#include <vector>
#include <map>

/* Code generator structure; creates basic block flow graphs */
class BasicBlockGenerator : public TreeNode::Functor {
public:
    BasicBlockGenerator(Environment* env);
    typedef Pointer<BasicBlockGenerator> Ptr; 

private:
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(StringLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Dispatch* expression);
    void operator()(Identifier* expression);
    void operator()(Block* statment);
    void operator()(Simple* statment);
    void operator()(While* statment);
    void operator()(For* statment);
    void operator()(Conditional* statment);
    void operator()(Variable* statment);
    void operator()(Return* statment);
    void operator()(When* statment);
    void operator()(Case* statment);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);

    int add(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::ADD, ++temp_, t2, t3));
        return temp_;
    }

    int sub(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::SUB, ++temp_, t2, t3));    
        return temp_;
    }

    int mul(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::MUL, ++temp_, t2, t3));    
        return temp_;
    }

    int div(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::DIV, ++temp_, t2, t3));    
        return temp_;
    }

    int andl(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::ANDL, ++temp_, t2, t3));    
        return temp_;
    }

    int orl(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::ORL, ++temp_, t2, t3));    
        return temp_;
    }

    int andb(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::ANDB, ++temp_, t2, t3));    
        return temp_;
    }

    int orb(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::ORB, ++temp_, t2, t3));    
        return temp_;
    }

    void push(BasicBlock* block, int t2) {
        block->instruction(Instruction(Instruction::PUSH, 0, t2, 0));    
    }

    void pop(BasicBlock* block, int t2) {
        block->instruction(Instruction(Instruction::POP, 0, t2, 0));    
    }

    int load(BasicBlock* block, int t2) {
        block->instruction(Instruction(Instruction::LOAD, ++temp_, t2, 0));    
        return temp_;
    }

    void store(BasicBlock* block, int t2, int t3) {
        block->instruction(Instruction(Instruction::STORE, 0, t2, t3));    
    }

    int li(BasicBlock* block, int immediate) {
        block->instruction(Instruction(Instruction::LI, ++temp_, immediate, 0));
        return temp_;
    }

    void beqz(BasicBlock* block, int t2) {
        block->instruction(Instruction(Instruction::BEQZ, 0, t2, 0));
    }

    int call(BasicBlock* block, Name* name) {
        block->instruction(Instruction(Instruction::CALL, 0, 0, 0));
        block->call_label(name);
        return ++temp_;
    }

    int variable(Name* name);
    void variable(Name* name, int temporary);
    void enter_scope();
    void exit_scope();

    Environment::Ptr environment_;
    Class::Ptr class_;
    Function::Ptr function_;
    BasicBlock::Ptr block_;
    
    // Mapping from var to temporary
    std::vector<std::map<Name::Ptr, int> > variable_;

    // Next temporary to use
    int temp_;
};

