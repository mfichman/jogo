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

CopyPropagator::CopyPropagator(Environment* env) :
    env_(env) {

}

void CopyPropagator::operator()(File* file) {
    if (env_->errors()) {
        return;
    } 
    file_ = file;
    for (Feature::Ptr f = env_->modules(); f; f = f->next()) {
        f(this);
    }
    file_ = 0;
}

void CopyPropagator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void CopyPropagator::operator()(Class* feature) {
    if (feature->location().file != file_) {
        return;
    }
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void CopyPropagator::operator()(Function* feature) {
    if (feature->location().file != file_) {
        return;
    }
    for (int i = 0; i < feature->basic_blocks(); i++) {
        operator()(feature->basic_block(i));
    } 
}

void CopyPropagator::operator()(BasicBlock* block) {
    std::map<int, Operand> eq;
    std::map<int, Operand> lit;

    for (int i = 0; i < block->instrs(); i++) {
        // Loop through all instructions in the block, looking for and recording
        // any aliases caused by move instructions.  Substitute aliases for the
        // originally copied value.  NOTE:  This assumes that the code is in
        // SSA form.
        Instruction& instr = block->instr(i);
        int first = instr.first().temp();
        int second = instr.second().temp();
        std::map<int, Operand>::iterator j;

        // Check to see if any of the operands have aliases.
        if (first) {
            if (MOV == instr.opcode() && (j = lit.find(first)) != lit.end()) {
                // If the RHS of a MOV can be replaced by a literal, then
                // morph it into a load for that literal
                Operand repl = j->second;
                repl.indirect(instr.first().indirect());
                instr.first(repl);
                instr.opcode(LOAD);
            }
            if ((j = eq.find(first)) != eq.end()) {
                // If the RHS has an alias to a non-precolored register, then
                // substitute that alias.  Make sure only to replace the
                // register name, NOT the whole operand!
                Operand repl = instr.first();
                repl.temp(j->second.temp());
                instr.first(repl);
            }
        }
        if (second) {
            if ((j = eq.find(second)) != eq.end()) {
                // If the RHS (second arg) has an alias to a non-precolored
                // register, then substitute that alias.  Make sure to only
                // replace the register name, NOT the whole operand!
                Operand repl = instr.second();
                repl.temp(j->second.temp());
                instr.second(repl);
            }
        }

        // If the instruction is a MOV, mark the LHS as an alias of the RHS
        if (MOV == instr.opcode()) {
            int result = instr.result().temp();
            Operand rhs = instr.first();
            if (rhs.temp() && (j = eq.find(rhs.temp())) != eq.end()) {
                // If the RHS has an alias, use that instead, but only if it's
                // not a machine reg
                if (j->second.temp() > 0) {
                    rhs = j->second;
                }
            }
            if (rhs.temp() > 0 && !rhs.indirect()) {
                eq[result] = rhs;
            }
        } 
        if (LOAD == instr.opcode() && !instr.first().indirect()) {
            int result = instr.result().temp();
            lit[result] = instr.first();
        }
    }
}


