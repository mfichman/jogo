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
    env_(env),
    liveness_(new LivenessAnalyzer(machine)),
    machine_(machine) {

}

void RegisterAllocator::operator()(File* file) {
    if (env_->errors()) { return; }
    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this);
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
    int spills = 0;
    spilled_.clear();

    while (true) {
        spill_ = false;
        graph_.clear();
        liveness_->operator()(func);
        for (int i = 0; i < func->basic_blocks(); i++) {
            build_graph(func->basic_block(i));
        }
        build_stack();
        color_graph();     

        if (spill_) {
            spill_register(func);
        } else {
            break;
        }
        spills++;
    }

    //std::cout << func->name()->string() <<" spills: " << spills << std::endl;

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
        Instruction const& instr = block->instr(i);
        set<RegisterId> const& live = instr.liveness()->in();
        RegisterId first = instr.first().reg();
        RegisterId second = instr.second().reg();
        RegisterId result = instr.result().reg();

        // This is just some record-keeping to find the largest temporary, a
        // to make sure that every temporary is assigned a register.
        if (first.id() >= graph_.size() && !first.is_colored()) { 
            graph_.resize(first.id() + 1);
        }
        if (second.id() >= graph_.size() && !second.is_colored()) {
            graph_.resize(second.id() + 1);
        }
        if (result.id() >= graph_.size() && !result.is_colored()) {
            graph_.resize(result.id() + 1);
        }

        // Find all interfering pairs of temporaries, and add them to the graph.
        for (set<RegisterId>::iterator m = live.begin(); m != live.end(); m++) {
            set<RegisterId>::iterator n = m;
            n++;
            if (!m->is_colored() && m->id() >= graph_.size()) {
               graph_.resize(m->id() + 1);
            }

            for (; n != live.end(); n++) {
                if (*n == *m) { continue; }

                if (!m->is_colored()) {
                    graph_[m->id()].neighbor_new(*n);
                }
                if (!n->is_colored()) {
                    // Add neighbors between temporary 'n' and temporary 'm' so 
                    // that they won't be allocated to the same register.
                    if (n->id() >= graph_.size()) {
                        graph_.resize(n->id() + 1);
                    }
                    graph_[n->id()].neighbor_new(*m);
                }
            }
            
            // Add an edge between the register being written and all of the
            // registers in the live set.  FIXME: This may not be necessary.
            // This was added to fix the case where a value was assigned, but
            // not read (i.e., use-def chain without the use).  Eventually, 
            // dead code elimination should take care of this.
            if (!result.is_colored()) {
                if (!m->is_colored()) {
                    graph_[m->id()].neighbor_new(result);
                }
                if (result.id() >= graph_.size()) {
                    graph_.resize(result.id() + 1);
                }
                graph_[result.id()].neighbor_new(*m); 
            }

            // If the instruction is a CALL or part of the call prologue, then
            // add an edge between live vars and the callee registers
            // (caller-saved)
            if (instr.opcode() == CALL && !m->is_colored()) {
                for (int j = 0; j < machine_->callee_regs(); j++) {
                    RegisterId reg = machine_->callee_reg(j)->id();
                    graph_[m->id()].neighbor_new(reg); 
                }            
            }
        }
    }
}


void RegisterAllocator::build_stack() {
    // Make a copy of the graph for the vertex removal.  We need the original
    // graph for the final coloring stage.
    vector<RegisterVertex*> work;
    for (int i = 1; i < graph_.size(); i++) {
        graph_[i].temp(RegisterId(i, 0)); // Set the graph temporary values
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
            int regs = machine_->reg_count();
            if (!!work[i]->temp() && work[i]->neighbors() < regs) {  
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
        stack_.push_back(choice->temp()); 
        for (int i = 0; i < choice->neighbors(); i++) {
            // 'Other' is precolored.
            RegisterId other = choice->neighbor(i);
            if (!other.is_colored()) {
                graph[other.id()].neighbor_del(choice->temp()); 
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
        RegisterVertex* v = &graph_[stack_.back().id()]; 
        RegisterId choice;

        for (Register::Ptr r = machine_->regs(); r; r = r->next()) {
            RegisterId reg = r->id();
            bool ok = true;
            
            // Check to make sure that the candidate color 'color' does not
            // interfere with any of the outgoing edges, including precolored
            // edges (indicated by a negative id)
            for (int i = 0; i < v->neighbors(); i++) {
                RegisterId other = v->neighbor(i);

                // Conflict one: candidate color is the same as the color 
                // already assigned to another temporary.
                if (!other.is_colored() && graph_[other.id()].reg() == reg) {
                    ok = false;
                    break;
                }

                // Conflict two: candiate color is the same as the color of
                // a precolored register that matches.
                if (other.is_colored() && other == reg) {
                    ok = false;
                    break;
                }
            } 
            if (ok) {
                choice = reg;
                break;
            }
        }
        if (!!choice) {
            v->reg(choice);
        } else {
            spill_ = true; // Fail, need to spill a register here
            return;
        }
        stack_.pop_back();
    }
}

void RegisterAllocator::rewrite_temporaries(BasicBlock* block) {
    // Loop through all instructions and replace temporaries with real
    // allocated registers.

    BasicBlock repl;
    for (int i = 0; i < block->instrs(); i++) {
        Instruction instr = block->instr(i);
        Operand first = instr.first();
        Operand second = instr.second();
        Operand result = instr.result();
        
        // Note: Negative numbers are used for the colored registers to make 
        // it clear that coloring has actually been done.
        if (!first.is_colored()) {
            first.reg(graph_[first.reg().id()].reg());
            instr.first(first);
        }
        if (!second.is_colored()) {
            second.reg(graph_[second.reg().id()].reg());
            instr.second(second);
        }
        if (!result.is_colored()) {
            instr.result(graph_[result.reg().id()].reg());
        }
        if (MOV != instr.opcode() || instr.result() != instr.first()) {
            repl.instr(instr);  
        }
    }
    block->swap(&repl);
}

void RegisterAllocator::spill_register(Function* func) {
    // Select and spill the register or temporary with the greatest number of
    // edges in the graph.
    RegisterId spilled;
    int max_neighbors = 0;
    bool is_caller_reg = false;

    // First attempt to spill caller-registers, because those registers have
    // the most conflicts, usually.
    for (int i = 0; i < machine_->caller_regs(); i++) {
        RegisterId reg = machine_->caller_reg(i)->id();
        set<RegisterId>::iterator m = spilled_.find(reg);
        if (m == spilled_.end()) {
            spilled = reg;
            is_caller_reg = true;
            break;
        } 
    }

    if (!spilled) {
        for (int i = 1; i < graph_.size(); i++) {
            RegisterVertex const& v = graph_[i];
            set<RegisterId>::iterator m = spilled_.find(v.temp());
            if (v.neighbors() > max_neighbors && m == spilled_.end()) {
                max_neighbors = v.neighbors();
                spilled = v.temp();
            }
        }
    }

    spilled_.insert(spilled);
    assert(!!spilled);

    func->stack_vars_inc();
    Address addr = Address(-func->stack_vars());

    // Iterate through all blocks, and insert loads/stores before reads/writes
    // of the spilled register.
    for (int i = 0; i < func->basic_blocks(); i++) {
        BasicBlock::Ptr block = func->basic_block(i);
        BasicBlock repl;
        
        // If this is the first block, and we're spilling a caller register,
        // then add a store
        if (i == 0 && is_caller_reg) {
            repl.instr(STORE, Operand(), Operand(addr), spilled);
        }

        for (int j = 0; j < block->instrs(); j++) {
            const Instruction& instr = block->instr(j);
            // Insert load if necessary for spilled register
            if (instr.first().reg() == spilled) {
                repl.instr(LOAD, instr.first().reg(), Operand(addr), Operand()); 
            }
            if (instr.second().reg() == spilled) {
                repl.instr(LOAD, instr.second().reg(), Operand(addr), Operand());
            } 
            
            // Insert a load before returns for caller regs
            if (instr.opcode() == RET && is_caller_reg) {
                repl.instr(LOAD, spilled, Operand(addr), Operand());
            }
            
            repl.instr(instr);
            // Insert store if necessary for spilled register
            if (instr.result().reg() == spilled) {
                RegisterId result = instr.result().reg();
                repl.instr(STORE, Operand(), Operand(addr), result);
            } 

        }
        block->swap(&repl);  
    } 
}

void RegisterVertex::neighbor_new(RegisterId temp) {
    if (find(out_.begin(), out_.end(), temp) == out_.end()) {
        out_.push_back(temp);
    }
}

void RegisterVertex::neighbor_del(RegisterId temp) {
    vector<RegisterId>::iterator i = find(out_.begin(), out_.end(), temp);
    if (i != out_.end()) {
        *i = out_.back();
        out_.pop_back();
    }
}
