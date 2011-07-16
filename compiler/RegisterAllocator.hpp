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
#include <set>


/* Structure for a register interference graph */
class RegisterVertex {
public:
    RegisterVertex() : temporary_(0), color_(0) {}
    
    void edge_new(int index);
    void edge_del(int index);
    void color(int color) { color_ = color; }
    void temporary(int temp) { temporary_ = temp; }
    int edge(int index) { return out_[index]; }
    int edges() const { return out_.size(); }
    int temporary() const { return temporary_; }
    int color() const { return color_; }

private:
    int temporary_;
    int color_;
    std::vector<int> out_;
};

/* Register allocator; reduces instructions to sequences */
class RegisterAllocator : public Object {
public:
    RegisterAllocator(int registers) :
        liveness_(new LivenessAnalyzer),
        registers_(registers) {
    }
    typedef Pointer<RegisterAllocator> Ptr;
    void operator()(Function* feature);
    int spilled() const { return spilled_; }

private:
    void build_graph(BasicBlock* block);
    void build_stack();
    void color_graph();
    void rewrite_temporaries(BasicBlock* block);

    std::vector<RegisterVertex> graph_;
    std::vector<int> stack_;
    LivenessAnalyzer::Ptr liveness_;
    int registers_;
    int spilled_;
    int max_;
};












