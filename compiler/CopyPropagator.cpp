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

#include "CopyPropagator.hpp"

CopyPropagator::CopyPropagator(Environment* env) {
    if (env->errors()) {
        return;
    } 
    for (Feature::Ptr m = env->modules(); m; m = m->next()) {
        m(this);
    }
}

void CopyPropagator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void CopyPropagator::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void CopyPropagator::operator()(Function* feature) {
    for (int i = 0; i < feature->basic_blocks(); i++) {
        operator()(feature->basic_block(i));
    } 
}

void CopyPropagator::operator()(BasicBlock* block) {
    std::map<int, Operand> eq;

    for (int i = 0; i < block->instrs(); i++) {
        // Loop through all instructions in the block, looking for an recording
        // any aliases caused by move instructions.  Substitute aliases for the
        // originally copied value.  NOTE:  This assumes that the code is in
        // SSA form.
        Instruction& instr = block->instr(i);
        int first = instr.first().temp();
        int second = instr.second().temp();
        std::map<int, Operand>::iterator j;

        // Check to see if any of the operands have aliases.
        if (first && (j = eq.find(first)) != eq.end()) {
            instr.first(j->second);
        }
        if (second && (j = eq.find(second)) != eq.end()) {
            instr.second(j->second);
        }

        // If the instruction is a MOV, then insert the LHS as an alias for the
        // RHS.  Make sure to use the RHS's alias that if the RHS itself has an
        // alias.
        if (MOV == instr.opcode()) {
            int result = instr.result().temp();
            Operand rhs = instr.first();
            if (rhs.temp() && (j = eq.find(rhs.temp())) != eq.end()) {
                rhs = j->second;
            }
            eq[result] = rhs;
        } 
    }
}


