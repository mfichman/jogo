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
#include "LivenessAnalyzer.hpp"
#include "BasicBlock.hpp"
#include "Machine.hpp"
#include <set>


/* Structure for a register interference graph */
class RegisterVertex {
public:
    RegisterVertex() : temp_(0), reg_(0) {}
    
    void neighbor_new(int index);
    void neighbor_del(int index);
    void reg(int reg) { reg_ = reg; }
    void temp(int temp) { temp_ = temp; }
    int neighbor(int index) { return out_[index]; }
    int neighbors() const { return out_.size(); }
    int temp() const { return temp_; }
    int reg() const { return reg_; }

private:
    int temp_;
    int reg_;
    std::vector<int> out_;
};

/* Register allocator; reduces instructions to sequences */
class RegisterAllocator : public TreeNode::Functor {
public:
    RegisterAllocator(Machine* machine) :
        liveness_(new LivenessAnalyzer(machine)),
        machine_(machine) {
    }
    RegisterAllocator(Environment* env, Machine* machine);

    typedef Pointer<RegisterAllocator> Ptr;
    void operator()(Module* feature);
    void operator()(Class* feature);
    void operator()(Function* feature);

private:
    void build_graph(BasicBlock* block);
    void build_stack();
    void color_graph();
    void rewrite_temporaries(BasicBlock* block);
    void spill_register(Function* func);

    std::vector<RegisterVertex> graph_;
    std::vector<int> stack_;
    LivenessAnalyzer::Ptr liveness_;
    Machine::Ptr machine_;
    bool spill_;
    std::set<int> spilled_;
};












