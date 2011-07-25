/*
 * Copyright (c) 2011 Matt Fichman
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
#include "Environment.hpp"
#include <algorithm>

using namespace std;


RegisterAllocator::RegisterAllocator(Environment* env, Machine* machine) :
    liveness_(new LivenessAnalyzer),
    machine_(machine) {

    if (env->errors()) {
        return;
    }
    for (Feature::Ptr m = env->modules(); m; m = m->next()) {
        m(this);
    }
}

void RegisterAllocator::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void RegisterAllocator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void RegisterAllocator::operator()(Function* func) {
    // Allocate registers for temporaries in the function.  This allocator
    // uses an optimistic, greedy graph coloring algorithm.
    spilled_ = 0;
    graph_.clear();

    liveness_->operator()(func);
    build_graph(func->code());
    build_stack();
    color_graph();     

    assert(!spilled_);
    if (spilled_) { return; }

    for (int i = 0; i < func->basic_blocks(); i++) {
        rewrite_temporaries(func->basic_block(i)); 
    }
}

void RegisterAllocator::build_graph(BasicBlock* block) {
    // Build the register interference graph.  Each vertex in the graph is a
    // temporary name.  Each edge represents a conflict between two
    // temporaries; that is, the temporaries are live at at least one code
    // point at the same time.

    // Now build the interference graph.
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        const set<int>& live = liveness_->live_in(instr);
        int first = instr.first().temporary();
        int second = instr.second().temporary();
        int result = instr.result().temporary();

        // This is just some record-keeping to find the largest temporary, a
        // to make sure that every temporary is assigned a register.
        if (first >= graph_.size() && first > 0) { 
            graph_.resize(first + 1);
        }
        if (second >= graph_.size() && second > 0) {
            graph_.resize(second + 1);
        }
        if (result >= graph_.size() && result > 0) {
            graph_.resize(result + 1);
        }

        // Find all interfering pairs of temporaries, and add them to the graph.
        for (set<int>::iterator m = live.begin(); m != live.end(); m++) {
            set<int>::iterator n = m;
            n++;
            if (*m < 0) { continue; } // Operand is precolored, skip.
            if (*m >= graph_.size()) {
               graph_.resize(*m + 1);
            }

            for (; n != live.end(); n++) {
                if (*n == *m) { continue; }
                if (*n < 0) { // Operand is a precolored register.
                    graph_[*m].edge_new(*n);
                } else {
                    // Add edges between temporary 'n' and temporary 'm' so 
                    // that they won't be allocated to the same register.
                    if (*n >= graph_.size()) {
                        graph_.resize(*n + 1);
                    }
                    graph_[*n].edge_new(*m);
                    graph_[*m].edge_new(*n);
                }
            }
            
            // Add an edge between the register being written and all of the
            // registers in the live set.  FIXME: This may not be necessary.
            // This was added to fix the case where a value was assigned, but
            // not read (i.e., use-def chain without the use).  Eventually, 
            // dead code elimination should take care of this.
            if (result > 0) {
                graph_[*m].edge_new(result);
                graph_[result].edge_new(*m); 
            }

            // If the instruction is a CALL or part of the call prologue, then
            // add an edge between live vars and the callee registers
            // (caller-saved)
            if (instr.opcode() == CALL) {
                if (*m >= graph_.size()) {
                    graph_.resize(*m + 1);
                }
                for (int j = 0; j < machine_->callee_regs(); j++) {
                    graph_[*m].edge_new(-machine_->callee_reg(j)->id()); 
                    //Negative numbers are pre-colored
                }            
            }
        }
    }

    if (block->next()) {
        build_graph(block->next());
    }
    if (block->branch()) {
        build_graph(block->branch());
    }
}


void RegisterAllocator::build_stack() {
    // Make a copy of the graph for the vertex removal.  We need the original
    // graph for the final coloring stage.
    vector<RegisterVertex*> work;
    for (int i = 1; i < graph_.size(); i++) {
        graph_[i].temporary(i); // Set the graph temporary values
        work.push_back(&graph_[i]); 
    }
    vector<RegisterVertex> graph = graph_;
    stack_.clear();

    while (!work.empty()) {
        // Loop through the whole graph, and find the first vertex with degree
        // less than the number of target registers.
        RegisterVertex* choice = 0;
        int index = 0;

        for (int i = 0; i < work.size(); i++) {
            if (work[i]->temporary() && work[i]->edges() < machine_->regs()) {  
                choice = work[i];
                index = i;
                break;
            }
        }

        // If a vertex was not found, then we have to optimistically choose one
        // vertex anyway.
        if (!choice) {
            choice = work[0];
            index = 0;
        }
    
        // Remove the vertex and add the temporary to the coloring stack.
        stack_.push_back(choice->temporary()); 
        for (int i = 0; i < choice->edges(); i++) {
            // Note: If 'other' is not positive, then it's a precolored node.
            int other = choice->edge(i);
            if (other > 0) {
                graph[other].edge_del(choice->temporary()); 
            }
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
        for (int color = 1; color < machine_->regs(); color++) {
            bool ok = true;
            
            // Check to make sure that the candidate color 'color' does not
            // interfere with any of the outgoing edges, including precolored
            // edges (indicated by a negative id)
            for (int i = 0; i < v->edges(); i++) {
                int other = v->edge(i);

                // Conflict one: candidate color is the same as the color 
                // already assigned to another temporary.
                if (other > 0 && graph_[other].color() == color) {
                    ok = false;
                    break;
                }

                // Conflict two: candiate color is the same as the color of
                // a precolored register.
                if (other < 0 && -other == color) {
                    ok = false;
                    break;
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
        int first = instr.first().temporary();
        int second = instr.second().temporary();
        int result = instr.result().temporary();
        
        // Note: Negative numbers are used for the colored registers to make 
        // it clear that coloring has actually been done.
        if (first > 0) {
            instr.first(-graph_[first].color());
        }
        if (second > 0) {
            instr.second(-graph_[second].color());
        }
        if (result > 0) {
            instr.result(-graph_[result].color());
        }
    }
}

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
