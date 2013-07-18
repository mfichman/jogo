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
#include "LivenessAnalyzer.hpp"
#include "IrBlock.hpp"
#include "Machine.hpp"
#include <set>


/* Structure for a register interference graph */
class RegisterVertex {
public:
    void reg(RegisterId reg) { reg_ = reg; }
    void temp(RegisterId temp) { temp_ = temp; }
    RegisterIdSet& neighbors() { return neighbors_; }
    RegisterIdSet const& neighbors() const { return neighbors_; }
    RegisterId temp() const { return temp_; }
    RegisterId reg() const { return reg_; }

private:
    RegisterId temp_;
    RegisterId reg_;
    RegisterIdSet neighbors_;
};

/* Register allocator; reduces instructions to sequences */
class RegisterAllocator : public TreeNode::Functor {
public:
    RegisterAllocator(Environment* env, Machine* machine);

    typedef Pointer<RegisterAllocator> Ptr;
    void operator()(File* file);
    void operator()(Class* feature);
    void operator()(Function* feature);

private:
    RegisterId color_reg(RegisterVertex const& v);
    bool color_ok(RegisterVertex const& v, RegisterId reg); 
    void build_graph(IrBlock* block);
    void build_stack();
    void color_graph();
    void rewrite_temporaries(IrBlock* block);
    void spill_register(Function* func);
    void dump_graph();

    std::vector<RegisterVertex> graph_;
    std::vector<RegisterId> stack_;
    Environment::Ptr env_;
    LivenessAnalyzer::Ptr liveness_;
    Machine::Ptr machine_;
    bool spill_;
    bool spill_float_;
    RegisterIdSet spilled_;
};












