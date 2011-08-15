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
    environment_(env),
    out_(Stream::stdout()),
    liveness_(new LivenessAnalyzer(mach)) {

    if (environment_->errors()) {
        return;
    }
    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }

    out_->flush();
}

void BasicBlockPrinter::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        const Location& loc = f->location();
        if (loc.file_name->string().find("../runtime") != 0) {
            f(this);
        }
    } 
}

void BasicBlockPrinter::operator()(Class* feature) {
    class_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockPrinter::operator()(Function* feature) {
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
        case ANDB: out_ << res << " <- " << first << " & " << second; break;
        case ORB: out_ << res << " <- " << first << " | " << second; break; 
        case NOTB: out_ << res << " <- not " << first; break;
        case PUSH: out_ << "push " << first; break; 
        case POP: out_ << res << " <- " << "pop"; break;
        case STORE: out_ << "store " << res << ", " << first; break;
        case LOAD: out_ << res << " <- " << "load " << first; break;
        case MOV: out_ << res << " <- " << first; break;
        case CALL: out_ << "call " << res.label(); break;
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
    
        out_ << " {";
        set<int>& live = liveness_->live_in(block->instr(i));
        for (set<int>::iterator i = live.begin(); i != live.end();) {
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
        out_ << "}\n";
    }
}


void BasicBlockPrinter::operator()(Formal* formal) {
}

void BasicBlockPrinter::operator()(StringLiteral* expression) {
}

void BasicBlockPrinter::operator()(IntegerLiteral* expression) {
}

void BasicBlockPrinter::operator()(FloatLiteral* expression) {
}

void BasicBlockPrinter::operator()(BooleanLiteral* expression) {
}

void BasicBlockPrinter::operator()(Binary* expression) {
}

void BasicBlockPrinter::operator()(Unary* expression) {
}

void BasicBlockPrinter::operator()(Call* expression) {
}

void BasicBlockPrinter::operator()(Dispatch* expression) {
}

void BasicBlockPrinter::operator()(Construct* expression) {
}

void BasicBlockPrinter::operator()(Identifier* expression) {
}

void BasicBlockPrinter::operator()(Empty* empty) {
}

void BasicBlockPrinter::operator()(Let* statement) {
}

void BasicBlockPrinter::operator()(Block* statement) {
}

void BasicBlockPrinter::operator()(Simple* statement) {
}

void BasicBlockPrinter::operator()(While* statement) {
}

void BasicBlockPrinter::operator()(Conditional* statement) {
}

void BasicBlockPrinter::operator()(Variable* statement) {
}

void BasicBlockPrinter::operator()(Return* statement) {
}

void BasicBlockPrinter::operator()(When* statement) {
}

void BasicBlockPrinter::operator()(Case* statement) {
}

void BasicBlockPrinter::operator()(Fork* statement) {
}

void BasicBlockPrinter::operator()(Yield* statement) {
}

void BasicBlockPrinter::operator()(Attribute* feature) {
}

void BasicBlockPrinter::operator()(Import* feature) {
}

void BasicBlockPrinter::operator()(Type* type) {
}

