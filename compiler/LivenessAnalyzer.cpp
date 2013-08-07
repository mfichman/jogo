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
#include "IrBlockPrinter.hpp"

using namespace std;

void LivenessAnalyzer::operator()(Function* feature) {
    if (!feature->ir_blocks()) { return; }
    finished_ = false;
    reset_ = true;
    function_ = feature;

    IrBlockPrinter::Ptr bprint(new IrBlockPrinter(env_, machine_));
    bprint->out(Stream::stout());

    // Iterate the liveness computation until the all the liveness rules are
    // fully statisfied.
    int iterations = 0;
    while (!finished_) {
        bprint->operator()(feature);
        finished_ = true;
        round_ = feature->ir_block(0)->round();
        operator()(feature->ir_block(0));
        reset_ = false;
        ++iterations;
        bprint->operator()(feature);
    }
    Stream::stout() << iterations << "iterations\n\n";
}

IrBlock* LivenessAnalyzer::branch(IrBlock* block) {
    // Get the next non-empty branch block
    IrBlock* next = block->branch();
    while (next && !next->instrs()) {
        assert(!next->branch() && "Descendant block has a branch");
        next = next->next();
    }
    return next;
}

IrBlock* LivenessAnalyzer::next(IrBlock* block) {
    // Get the next non-empty block
    IrBlock* next = block->next();
    while (next && !next->instrs()) {
        assert(!next->branch() && "Descendant block has a branch");
        next = next->next();
    }
    return next;
}

void LivenessAnalyzer::operator()(IrBlock* block) {
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

    if (!block->is_ret()) {
        if (block->next()) { operator()(block->next()); }
        if (block->branch()) { operator()(block->branch()); }
    }

    for (int i = block->instrs()-1; i >= 0; i--) {
        // Note: We iterate through the list of instructions backwards, since
        // liveness analysis is a reverse type of analysis - that is, we 
        // propagate liveness information from the use of a temporary back to
        // its last write.
        Instruction const& instr = block->instr(i);
        Operand const& first = instr.first();
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
                if (IrBlock* branch = this->branch(block)) {
                    outw |= branch->instr(0).liveness()->in();
                } 
                if (IrBlock* next = this->next(block)) {
                    outw |= next->instr(0).liveness()->in();
                } 
            } else { 
                // Get the 'in' set from the next instruction in this block;
                // that will become the 'out' set for the current instruction
                outw |= block->instr(i+1).liveness()->in();
            }
        }

        // in[n] := use[n] U (out[n] - def[n]).  Since the in/out sets can 
        // never decrease in size, don't worry about resetting either of the
        // two sets.
        bool result_in_use = false;
        // The only instruction in def[n] is the result reg.  If that reg is
        // not in use[n], then we must subtract it from the 'in' set.
        if (!!instr.first().reg()) {
            inw.set(instr.first().reg());
            if (instr.first().reg() == instr.result().reg()) {
                result_in_use = true;
            }
        }
        if (!!instr.second().reg()) {
            inw.set(instr.second().reg());
            if (instr.second().reg() == instr.result().reg()) {
                result_in_use = true;
            }
        }
        if (PHI == instr.opcode()) {
            for (PhiArg* arg = first.phi_arg(); arg; arg = arg->next()) {
                inw.set(arg->operand().reg()); 
            }
        }

        // If this is the first instruction of the function, then we need to
        // add all the registers belonging to the caller to the out[n] set.
        // Note: Since in[n] := use[n] U (out[n] - def[n]), and def[n] is
        // guaranteed to not include the caller registers, we simply add the
        // caller regs directly to the in[n] set
        if (block == function_->ir_block(0) && i == 0) {
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
        inw |= outw; 
        if (!!instr.result().reg() && !result_in_use) {
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
    }
}


