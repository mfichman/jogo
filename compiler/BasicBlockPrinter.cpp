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

BasicBlockPrinter::BasicBlockPrinter(Environment* environment) :
    environment_(environment),
    liveness_(new LivenessAnalyzer) {

    if (environment_->errors()) {
        return;
    }
    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }
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

    BasicBlock::Ptr block = feature->code();
    if (block->instrs()) {
        cout << feature->label() << ":" << std::endl;
        for (int i = 0; i < feature->basic_blocks(); i++) {
            operator()(feature->basic_block(i));
        } 
    }
}

void BasicBlockPrinter::operator()(BasicBlock* block) {
    BasicBlock::Ptr branch = block->branch();
    BasicBlock::Ptr next = block->next();
    if (block->label()) {
        cout << block->label() << ":" << endl;
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        Operand res = instr.result();
        Operand first = instr.first();
        Operand second = instr.second();
        cout << "    ";
        switch (instr.opcode()) {
        case ADD: cout << res << " <- " << first << " + " << second; break;
        case SUB: cout << res << " <- " << first << " - " << second; break;
        case MUL: cout << res << " <- " << first << " * " << second; break; 
        case DIV: cout << res << " <- " << first << " / " << second; break; 
        case EQ: cout << res << " <- " << first << " == " << second; break;
        case ANDL: cout << res << " <- " << first << " and " << second; break;
        case ORL: cout << res << " <- " << first << " or " << second; break; 
        case ANDB: cout << res << " <- " << first << " & " << second; break; 
        case ORB: cout << res << " <- " << first << " & " << second; break; 
        case PUSH: cout << "push " << first; break; 
        case POP: cout << res << " <- " << "pop"; break;
        case STORE: cout << "store " << first << ", " << second; break;
        case LOAD: cout << res << " <- " << "load " << first; break;
        case NOTL: cout << res << " <- not " << first; break;
        case CALL: cout << "call " << res; break;
        case JUMP: cout << "jump " << branch->label(); break;
        case BNE:
            cout << "if " << first << " != " << second << " goto ";
            cout << branch->label(); 
            break;
        case BEQ:
            cout << "if " << first << " == " << second << " goto ";
            cout << branch->label(); 
            break;
        case MOV: cout << res << " <- " << first; break;
        case BEQZ:
            cout << "if not " << first << " goto ";
            cout << branch->label();
            break;
        case BNEQZ:
            cout << "if " << first << " goto ";
            cout << branch->label();
            break;
        case RET: cout << "ret"; break;
        }
    
        cout << " {";
        set<int>& live = liveness_->live_in(block->instr(i));
        for (set<int>::iterator i = live.begin(); i != live.end();) {
            if (*i == 0) {
                ++i;
            } else {
                if (*i > 0) {
                    cout << 't' << *i;
                } else {
                    cout << 'r' << -*i;
                }
                if (++i != live.end()) {
                    cout << ", ";
                }
            }
        }
        cout << "}" << endl;
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

