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

#include "IrBlockPrinter.hpp"
#include <iostream>

using namespace std;

IrBlockPrinter::IrBlockPrinter(Environment* env, Machine* mach) :
    env_(env),
    machine_(mach),
    liveness_(new LivenessAnalyzer(mach)) {
}

void IrBlockPrinter::operator()(File* file) {
    if (env_->errors()) { return; }
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }
    out_->flush();
}

void IrBlockPrinter::operator()(Class* feature) {
    class_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    class_ = 0;
}

void IrBlockPrinter::operator()(Function* feature) {
    liveness_->operator()(feature);

    if (feature->ir_blocks()) {
        out_ << feature->label() << ":\n";
        for (int i = 0; i < feature->ir_blocks(); i++) {
            operator()(feature->ir_block(i));
        } 
    }
}

void IrBlockPrinter::operator()(IrBlock* block) {
    IrBlock::Ptr branch = block->branch();
    IrBlock::Ptr next = block->next();
    if (block->label()) {
        out_ << block->label() << ":\n";
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        if (instr.opcode() == NOP) { continue; }
        Operand res = instr.result();
        Operand first = instr.first();
        Operand second = instr.second();

        out_ << "    ";
        switch (instr.opcode()) {
        case ADD: out_ << res << " <- " << first << " + " << second; break;
        case SUB: out_ << res << " <- " << first << " - " << second; break;
        case MUL: out_ << res << " <- " << first << " * " << second; break; 
        case DIV: out_ << res << " <- " << first << " / " << second; break; 
        case NEG: out_ << res << " <- -" << first; break;
        case ANDB: out_ << res << " <- " << first << " & " << second; break;
        case ORB: out_ << res << " <- " << first << " | " << second; break; 
        case NOTB: out_ << res << " <- not " << first; break;
        case STORE: out_ << "store " << first << ", " << second; break;
        case LOAD: out_ << res << " <- " << "load " << first; break;
        case MOV: out_ << res << " <- " << first; break;
        case CALL: 
            out_ << "call ";
            if (first.label()) {
                out_ << first.label();
            } else {
                out_ << first;
            }
            break;
        case JUMP: out_ << "jump " << branch->label(); break;
        case BNE:
            out_ << "if " << first << " != " << second << " goto ";
            out_ << branch->label(); 
            break;
        case BE:
            out_ << "if " << first << " == " << second << " goto ";
            out_ << branch->label(); 
            break;
        case BZ:
            out_ << "if not " << first << " goto ";
            out_ << branch->label();
            break;
        case BNZ:
            out_ << "if " << first << " goto ";
            out_ << branch->label();
            break;
        case BG:
            out_ << "if " << first << " > " << second << " goto ";
            out_ << branch->label();
            break;
        case BGE:
            out_ << "if " << first << " >= " << second << " goto ";
            out_ << branch->label();
            break;
        case BL:
            out_ << "if " << first << " < " << second << " goto ";
            out_ << branch->label();
            break;
        case BLE:
            out_ << "if " << first << " <= " << second << "goto ";
            out_ << branch->label();
            break;
        case RET: out_ << "ret"; break;
        case NOP: break;
        default: assert(!"Not supported");
        }
    
        if (env_->dump_liveness()) {
            out_ << " { ";
            RegisterIdSet const& live = instr.liveness()->in();
            for (int i = 0; i < live.bits(); ++i) {
                if (live.bit(i)) {
                    RegisterId id(i, 0);
                    if (machine_->reg(id)) {
                        out_ << RegisterId(i, 0);
                    } else {
                        out_ << "i" << (i-machine_->regs());
                    }
                    if (i != live.bits()-2) {
                        out_ << " "; 
                    }
                }
            }
            out_ << "}";
        }
        out_ << '\n';
    }
}

