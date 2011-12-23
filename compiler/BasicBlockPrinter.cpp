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

#include "BasicBlockPrinter.hpp"
#include <iostream>

using namespace std;

BasicBlockPrinter::BasicBlockPrinter(Environment* env, Machine* mach) :
    env_(env),
    liveness_(new LivenessAnalyzer(mach)) {
}

void BasicBlockPrinter::operator()(File* file) {
    if (env_->errors()) {
        return;
    }
    file_ = file;
    for (Feature::Ptr f = env_->modules(); f; f = f->next()) {
        f(this);
    }
    file_ = 0;
    out_->flush();
}

void BasicBlockPrinter::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        const Location& loc = f->location();
        f(this);
    } 
    module_ = 0;
}

void BasicBlockPrinter::operator()(Class* feature) {
    if (feature->location().file != file_) {
        return;
    }
    class_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    class_ = 0;
}

void BasicBlockPrinter::operator()(Function* feature) {
    if (feature->location().file != file_) {
        return;
    } 
    liveness_->operator()(feature);

    if (feature->basic_blocks()) {
        out_ << feature->label() << ":\n";
        for (int i = 0; i < feature->basic_blocks(); i++) {
            operator()(feature->basic_block(i));
        } 
    }
}

void BasicBlockPrinter::operator()(BasicBlock* block) {
    BasicBlock::Ptr branch = block->branch();
    BasicBlock::Ptr next = block->next();
    if (block->label()) {
        out_ << block->label() << ":\n";
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
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
        case PUSH: out_ << "push " << first; break; 
        case POP: out_ << res << " <- " << "pop"; break;
        case POPN: out_ << "popn " << first; break;
        case STORE: out_ << "store " << first << ", " << second; break;
        case LOAD: out_ << res << " <- " << "load " << first; break;
        case MOV: out_ << res << " <- " << first; break;
        case CALL: out_ << "call ";
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
        }
    
        if (env_->dump_liveness()) {
           out_ << " {";
           const set<int>& live = instr.liveness()->in();
           for (set<int>::const_iterator i = live.begin(); i != live.end();) {
               if (*i == 0) {
                   ++i;
               } else {
                   if (*i > 0) {
                       out_ << 't' << *i;
                   } else {
                       out_ << 'r' << -*i;
                   }
                   if (++i != live.end()) {
                       out_ << ", ";
                   }
               }
           }
           out_ << "}";
        }
        out_ << "\n";
    }
}

