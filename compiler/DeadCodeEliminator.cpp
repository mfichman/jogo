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

#include "DeadCodeEliminator.hpp"


DeadCodeEliminator::DeadCodeEliminator(Environment* env, Machine* mach) :
    liveness_(new LivenessAnalyzer(mach)) {

    if (env->errors()) {
        return;
    } 
    for (Feature::Ptr m = env->modules(); m; m = m->next()) {
        m(this);
    }
}

void DeadCodeEliminator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void DeadCodeEliminator::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void DeadCodeEliminator::operator()(Function* feature) {
    liveness_->operator()(feature);
    for (int i = 0; i < feature->basic_blocks(); i++) {
        operator()(feature->basic_block(i));
    } 
}

void DeadCodeEliminator::operator()(BasicBlock* block) {
    BasicBlock::Ptr repl = new BasicBlock(); 
    
    // Loop through each instruction.  If the result of the instruction is
    // dead after the instruction, then the instruction is dead code.
    // For example, if the liveness is x := ..., out={} then the statement is 
    // trivially dead, because x is not live following the assignment.
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        int result = instr.result().temp();
        const std::set<int>& out = instr.liveness()->out();
        
        if (!result || out.find(result) != out.end()) {
            repl->instr(instr); 
        } 
    }
    block->swap(repl);
}

