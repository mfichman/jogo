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
#include "BasicBlockPrinter.hpp"
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
    spilled_ = RegisterIdSet(func->temp_regs()+1);

    while (true) {
        spill_ = false;
        graph_.clear();
        graph_.resize(func->temp_regs()+1);
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
        RegisterIdSet const& live = instr.liveness()->in();
        RegisterId first = instr.first().reg();
        RegisterId second = instr.second().reg();
        RegisterId result = instr.result().reg();

        // Find all interfering pairs of temporaries, and add them to the graph.
        for (int i = 0; i < live.bits(); ++i) {
            if (!live.bit(i)) { continue; }
            RegisterId m(i, 0);

            for (int j = i+1; j < live.bits(); ++j) {
                if (!live.bit(j)) { continue; }
                assert(j != i);
                RegisterId n(j, 0);
                if (!machine_->reg(m)) {
                    graph_[m.id()].neighbor_new(n);
                    graph_[m.id()].temp(m);
                }
                if (!machine_->reg(n)) {
                    // Add neighbors between temporary 'n' and temporary 'm' so 
                    // that they won't be allocated to the same register.
                    graph_[n.id()].neighbor_new(m);
                    graph_[n.id()].temp(n);
                }
            }
            
            // Add an edge between the register being written and all of the
            // registers in the live set.  FIXME: This may not be necessary.
            // This was added to fix the case where a value was assigned, eut
            // not read (i.e., use-def chain without the use).  Eventually, 
            // dead code elimination should take care of this.
            if (!!result && !machine_->reg(result)) {
                if (!machine_->reg(m)) {
                    graph_[m.id()].neighbor_new(result);
                    graph_[m.id()].temp(m);
                }
                graph_[result.id()].neighbor_new(m); 
                graph_[result.id()].temp(result);
            }

            // If the instruction is a CALL or part of the call prologue, then
            // add an edge between live vars and the callee registers
            // (caller-saved)
            if (instr.opcode() == CALL && !machine_->reg(m)) {
                for (int j = 0; j < machine_->callee_regs(); j++) {
                    RegisterId reg = machine_->callee_reg(j)->id();
                    graph_[m.id()].neighbor_new(reg); 
                    graph_[m.id()].temp(m);
                }            
            }
        }
    }
}


void RegisterAllocator::build_stack() {
    // Make a copy of the graph for the vertex removal.  We need the original
    // graph for the final coloring stage.
    vector<RegisterVertex*> work;
    for (size_t i = 1; i < graph_.size(); ++i) {
        if (!graph_[i].temp()) { continue; }
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
            int const regs = machine_->regs();
            RegisterId temp = work[i]->temp();
            if (!machine_->reg(temp) && work[i]->neighbors() < regs) {  
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
            if (!machine_->reg(other)) {
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

        for (int j = 1; j < machine_->regs(); ++j) {
            RegisterId reg = machine_->reg(RegisterId(j, 0))->id();
            bool ok = true;
        
            if (reg.is_float() != v->temp().is_float()) {
                continue;
            }
            
            // Check to make sure that the candidate color 'color' does not
            // interfere with any of the outgoing edges, including precolored
            // edges (indicated by a negative id)
            for (int i = 0; i < v->neighbors(); i++) {
                RegisterId other = v->neighbor(i);

                // Conflict one: candidate color is the same as the color 
                // already assigned to another temporary.
                if (!machine_->reg(other) && graph_[other.id()].reg() == reg) {
                    ok = false;
                    break;
                }

                // Conflict two: candiate color is the same as the color of
                // a precolored register that matches.
                if (machine_->reg(other) && other == reg) {
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
            assert(machine_->reg(choice) && "Not a machine reg");
            v->reg(choice);
        } else {
            spill_ = true; // Fail, need to spill a register here
            spill_float_ = v->temp().is_float();
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
        if (!!first.reg() && !machine_->reg(first.reg())) {
            first.reg(graph_[first.reg().id()].reg());
            instr.first(first);
        }
        if (!!second.reg() && !machine_->reg(second.reg())) {
            second.reg(graph_[second.reg().id()].reg());
            instr.second(second);
        }
        if (!!result.reg() && !machine_->reg(result.reg())) {
            instr.result(graph_[result.reg().id()].reg());
        }
        repl.instr(instr);  
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
        if (reg.is_float() != spill_float_) {
            continue;
        }
        if (!spilled_.has(reg)) {
            spilled = reg;
            is_caller_reg = true;
            break;
        }
    }

    if (!spilled) {
        for (size_t i = 1; i < graph_.size(); ++i) {
            RegisterVertex const& v = graph_[i];
            if (!v.temp()) { continue; }
            if (v.neighbors() > max_neighbors && !spilled_.has(v.temp())) {
                max_neighbors = v.neighbors();
                spilled = v.temp();
            }
        }
    }

    spilled_.set(spilled);
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
            const RegisterId first = instr.first().reg();
            const RegisterId second = instr.second().reg();
            const RegisterId result = instr.result().reg();
            // Insert load if necessary for spilled register
            if (!!first && first == spilled) {
                repl.instr(LOAD, first, Operand(addr), Operand()); 
            }
            if (!!second && second == spilled) {
                repl.instr(LOAD, second, Operand(addr), Operand());
            } 
            
            // Insert a load before returns for caller regs
            if (instr.opcode() == RET && is_caller_reg) {
                repl.instr(LOAD, spilled, Operand(addr), Operand());
            }
            
            repl.instr(instr);
            // Insert store if necessary for spilled register
            if (!!result && result == spilled) {
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
