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

CopyPropagator::CopyPropagator(Environment* env, Machine* machine) :
    env_(env),
    machine_(machine) {

}

void CopyPropagator::operator()(File* file) {
    if (env_->errors()) { return; }
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }
}

void CopyPropagator::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
       f(this);
    }
}

void CopyPropagator::operator()(Function* feature) {
    for (int i = 0; i < feature->ir_blocks(); i++) {
        operator()(feature->ir_block(i));
    } 
}

void CopyPropagator::operator()(IrBlock* block) {
    // Removes unnecessary forward copy instructions.  Consider the following
    // instruction sequence:
    // mov r1, r2
    // op r1, ...
    // op r1, ...
    // This is optimized to the following:
    // mov r1, r2
    // op r2, ...
    // op r2, ...
    // During dead-code elimination, the extra mov instruction is removed.  The
    // optimization does not apply to sequences where the RHS of an op is a
    // precolored regiester.
    std::map<RegisterId, Operand> eq;
    std::map<RegisterId, Operand> lit;

    for (int i = 0; i < block->instrs(); i++) {
        // Loop through all instructions in the block, looking for and recording
        // any aliases caused by move instructions.  Substitute aliases for the
        // originally copied value.  NOTE:  This assumes that the code is in
        // SSA form.
        Instruction& instr = block->instr(i);
        RegisterId const result = instr.result().reg();
        RegisterId const first = instr.first().reg();
        RegisterId const second = instr.second().reg();
        std::map<RegisterId, Operand>::iterator j;

        // Check to see if any of the operands have aliases.
        if (!!first) {
            if (MOV == instr.opcode() && (j = lit.find(first)) != lit.end()) {
                // If the RHS of a MOV can be replaced by a literal, then
                // morph it into a load for that literal
                Operand repl = j->second;//FixMe Operand(j->second.literal());
                instr.first(repl);
                instr.opcode(LOAD);
                assert(!!repl&&"Nil operand");
            } else if ((j = eq.find(first)) != eq.end()) {
                // If the RHS has an alias to a non-precolored register, then
                // substitute that alias.  Make sure only to replace the
                // register name, NOT the whole operand!
                Operand repl = instr.first();
                repl.reg(j->second.reg());
                instr.first(repl);
                assert(!!repl&&"Nil operand");
            }
        }
        if (!!second && ((j = eq.find(second)) != eq.end())) {
            // If the RHS (second arg) has an alias to a register, then
            // substitute that alias.  Make sure to only replace the register
            // name, NOT the whole operand!
            Operand repl = instr.second();
            repl.reg(j->second.reg());
            instr.second(repl);
            assert(!!repl&&"Nil operand");
        }

        // If the instruction is a MOV, mark the LHS as an alias of the RHS
        if (MOV == instr.opcode()) {
            Operand rhs = instr.first();
            if (!!rhs.reg() && (j = eq.find(rhs.reg())) != eq.end()) {
                // If the RHS has an alias, use that instead, but only if it's
                // not a machine reg
                if (!machine_->reg(j->second.reg())) {
                    rhs = j->second;
                }
            }
            if (!machine_->reg(rhs.reg()) && !rhs.is_indirect() && !!rhs.reg()) {
                assert(!!rhs&&"Nil operand");
                eq[result] = rhs;
            }
        } 
        if (LOAD == instr.opcode() && !instr.first().is_indirect()) {
            lit[result] = instr.first();
        }
        
        // Remove return registers from the alias set, b/c their value will be
        // different after the return from the function call.
        if (CALL == instr.opcode()) {
            for (int i = 0; i < machine_->int_return_regs(); ++i) {
                RegisterId id = machine_->int_return_reg(i)->id();
                eq.erase(id);
                lit.erase(id);
            }
            for (int i = 0; i < machine_->float_return_regs(); ++i) {
                RegisterId id = machine_->float_return_reg(i)->id();
                eq.erase(id);
                lit.erase(id);
            }
        }
    }
}
