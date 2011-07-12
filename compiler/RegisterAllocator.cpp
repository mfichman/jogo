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

#include "RegisterAllocator.hpp"

using namespace std;

void RegisterVertex::edge_new(int temp) {
    if (find(out_.begin(), out_.end(), temp) == out_.end()) {
        out_.push_back(temp);
    }
}

void RegisterVertex::edge_del(int temp) {
    vector<int>::iterator i = find(out_.begin(), out_.end(), temp);
    if (i != out_.end()) {
        *i = out_.back();
        out_.pop_back();
    }
}

void RegisterAllocator::operator()(Function* func) {
    // Allocate registers for temporaries in the function.  This allocator
    // uses an optimistic, greedy graph coloring algorithm.
    spilled_ = 0;

    liveness_->operator()(func);
    build_graph(func->code());
    build_stack();
    color_graph();     

    if (spilled_) return;

    rewrite_temporaries(func->code()); 
}

void RegisterAllocator::build_graph(BasicBlock* block) {
    // Build the register interference graph.  Each vertex in the graph is a
    // temporary name.  Each edge represents a conflict between two
    // temporaries; that is, the temporaries are live at at least one code
    // point at the same time.
    graph_.clear();

    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        const set<int>& live = liveness_->live(instr);
        for (set<int>::iterator m = live.begin(); m != live.end(); m++) {
            set<int>::iterator n = m;
            n++;
            for (; n != live.end(); n++) {
                // Make sure the pair is always ordered the same
                if (*n < graph_.size()) {
                    graph_.resize(*n + 1);
                }
                if (*m < graph_.size()) {
                    graph_.resize(*m + 1);
                }
                graph_[*n].temporary(*n);
                graph_[*m].temporary(*m);
                graph_[*n].edge_new(*m);
                graph_[*m].edge_new(*n);
            }
        }
    }

    if (block->next()) {
        build_graph(block->next());
    }
    if (block->branch()) {
        build_graph(block->next());
    }
}


void RegisterAllocator::build_stack() {
    // Make a copy of the graph for the vertex removal.  We need the original
    // graph for the final coloring stage.
    vector<RegisterVertex> graph = graph_;
    vector<RegisterVertex*> work;
    stack_.clear();

    for (int i = 0; i < graph.size(); i++) {
        work.push_back(&graph[i]);    
    }

    while (!work.empty()) {
        // Loop through the whole graph, and find the first vertex with degree
        // less than the number of target registers.
        RegisterVertex* choice = 0;
        int index = 0;

        for (int i = 0; i < work.size(); i++) {
            if (work[i]->temporary() && work[i]->edges() < registers_) {  
                choice = work[i];
                index = i;
                break;
            }
        }

        // If a vertex was not found, then we have to optimistically choose one
        // vertex anyway.
        if (!choice) {
            choice = work[0];
        }
    
        // Remove the vertex and add the temporary to the coloring stack.
        stack_.push_back(choice->temporary()); 
        for (int i = 0; i < choice->edges(); i++) {
            graph[choice->edge(i)].edge_del(choice->temporary()); 
        } 
        work[index] = work.back();
        work.pop_back();
    }
}

void RegisterAllocator::color_graph() {
    // Pop vertices off the stack until empty.  For each vertex, loop through
    // each possible color.  If the color doesn't conflict with an existing
    // color assignment for an adjacent node, then choose that color. 

    while (!stack_.empty()) { // Now color the graph using the stack.
        RegisterVertex* v = &graph_[stack_.back()]; 
        int choice = 0;
        for (int color = 1; color < registers_; color++) {
            bool ok = true;
            for (int i = 0; i < v->edges(); i++) {
                if (graph_[v->edge(i)].color() == color) {
                    ok = false;
                }
            } 
            if (ok) {
                choice = color;
                break;
            }
        }
        if (choice) {
            v->color(choice);
        } else {
            spilled_ = v->temporary(); // Fail, need to spill a register here
            return;
        }
        stack_.pop_back();
    }
}

void RegisterAllocator::rewrite_temporaries(BasicBlock* block) {
    // Loop through all instructions and replace temporaries with real 
    // allocated registers.

    for (int i = 0; i < block->instrs(); i++) {
        Instruction& instr = block->instr(i);
        if (instr.first().temporary()) {
            instr.first(graph_[instr.first().temporary()].color());
        }
        if (instr.second().temporary()) {
            instr.second(graph_[instr.second().temporary()].color());
        }
        if (instr.result().temporary()) {
            instr.result(graph_[instr.result().temporary()].color());
        }
    }

    if (block->next()) {
        build_graph(block->next());
    }
    if (block->branch()) {
        build_graph(block->next());
    }
}
