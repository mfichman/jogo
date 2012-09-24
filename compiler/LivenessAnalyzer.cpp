/*
 * Copyright (c) 2011 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaisg a copy
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

#include "LivenessAnalyzer.hpp"

using namespace std;

void LivenessAnalyzer::operator()(Function* feature) {
    if (!feature->basic_blocks()) { return; }
    finished_ = false;
    reset_ = true;
    function_ = feature;

    // Iterate the liveness computation until the all the liveness rules are
    // fully statisfied.
    while (!finished_) {
        finished_ = true;
        round_ = feature->basic_block(0)->round();
        operator()(feature->basic_block(0));
        reset_ = false;
    }
    //std::cout << feature->name()->string() << ", " << round_ << std::endl;
}

void LivenessAnalyzer::operator()(BasicBlock* block) {
    // Compute liveness information for the basic block.  This algorithm is 
    // partly based on the notes found here, with slight optimizations: 
    // http://www.classes.cs.uchicago.edu/archive/2004/spring/22620-1/docs/liveness.pdf 
    if (block->round() > round_) { return; }
    block->round_inc();

    if (reset_) {
        for (int i = 0; i < block->instrs(); i++) {
            Instruction& instr = block->instr(i);
            RegisterIdSet empty(function_->temp_regs()+1);
            instr.liveness()->in(empty);
            instr.liveness()->out(empty);
        }
    }

//    std::cout << block->label()->string() << " -> ";
//    if (block->next()) { 
//        std::cout << block->next()->label()->string() << ", ";
//    }
//    if (block->branch()) { 
//        std::cout << block->branch()->label()->string();
//    }
 //   std::cout << std::endl;
    if (!block->is_ret()) {
        if (block->next()) { operator()(block->next()); }
        if (block->branch()) { operator()(block->branch()); }
    }

//    std::cout << block->label()->string()  << std::endl;
    

    for (int i = block->instrs()-1; i >= 0; i--) {
        // Note: We iterate through the list of instructions backwards, since
        // liveness analysis is a reverse type of analysis - that is, we 
        // propagate liveness information from the use of a temporary back to
        // its last write.
        Instruction const& instr = block->instr(i);
        RegisterIdSet const& in = instr.liveness()->in();
        RegisterIdSet const& out = instr.liveness()->out();
        RegisterIdSet inw = in;
        RegisterIdSet outw = out;

        // Recompute the 'out' set from the 'in' set of the next instruction(s).
        // Last instruction of the block
        if (RET != instr.opcode()) {
            if (i == block->instrs()-1) { 
                // Last instruction of the block, so get the next instruction 
                // the first instruction of the following block.
                if (block->branch() && block->branch()->instrs()) {
                    outw |= block->branch()->instr(0).liveness()->in();
                } 
                if (block->next() && block->next()->instrs()) {
                    outw |= block->next()->instr(0).liveness()->in();
                } 
            } else { 
                // Get the 'in' set from the next instruction in this block.
                outw |= block->instr(i+1).liveness()->in();
            }
        }

        // in[n] := use[n] U (out[n] - def[n]).  Since the in/out sets can 
        // never decrease in size, don't worry about resetting either of the
        // two sets.
        if (!!instr.first().reg()) {
            inw.set(instr.first().reg());
        }
        if (!!instr.second().reg()) {
            inw.set(instr.second().reg());
        }

        // If this is the first instruction of the function, then we need to
        // add all the registers belonging to the caller to the def[n] set.
        if (block == function_->basic_block(0) && i == 0) {
            inw |= machine_->caller_set();
        }

        // If this is a return instruction, then we need to add all registers
        // belonging to the caller to the use[n] set.
        if (instr.opcode() == RET) {
            inw |= machine_->caller_set();
            inw |= machine_->return_set();
        }

        // If the instruction is a call instruction, add all arg registers to the
        // use[n] set.
        if (instr.opcode() == CALL) {
            inw |= machine_->arg_set();
        }

        // In/out sets can never decrease in size, don't worry about removing
        // def[n] temporaries from the set
        // out[n] - def[n] added to in[n]
        bool const has_result = in.has(instr.result().reg());
        inw |= outw; 
        if (has_result) {
            inw.set(instr.result().reg());
        } else {
            inw.del(instr.result().reg());
        }

        if (inw != in) {
            finished_ = false; 
            instr.liveness()->in(inw);
        }
        if (outw != out) {
            finished_ = false; 
            instr.liveness()->out(outw);
        }
        assert(instr.liveness()->in() == inw);
        assert(instr.liveness()->out() == outw);

//        for (int i = 0; i < inw.bits(); ++i) {
//            if (inw.bit(i)) {
//                std::cout << (i-machine_->regs()) << ", ";
//            }
//        }
//        std::cout << std::endl;
    }
//    std::cout << std::endl;
}


