/*
 * Copyright (c) 2013 Matt Fichman
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

#include "IrFinalizer.hpp"


IrFinalizer::IrFinalizer(Environment* env) :
    env_(env) {

}

void IrFinalizer::operator()(File* file) {
    if (env_->errors()) { return; } 
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }
}

void IrFinalizer::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void IrFinalizer::operator()(Function* feature) {
    for (int i = 0; i < feature->ir_blocks(); i++) {
        operator()(feature->ir_block(i));
    } 
}

void IrFinalizer::operator()(IrBlock* block) {
    // Loop through each instruction.  If the result of the instruction is
    // dead after the instruction, then the instruction is dead code.
    // For example, if the liveness is x := ..., out={} then the statement is 
    // trivially dead, because x is not live following the assignment.
    for (int i = 0; i < block->instrs(); i++) {
        Instruction const& instr = block->instr(i);
        Operand const& ret = instr.result();
        if (instr.opcode() != PHI) {
            return;
        }
        block->instr(i, Instruction(NOP, Operand(), Operand(), Operand()));
        for (PhiArg* arg = instr.first().phi_arg(); arg; arg = arg->next()) {
            IrBlock* pred = arg->block();
            pred->instr(Instruction(MOV, ret, arg->operand(), Operand()));
        } 
    }
}

