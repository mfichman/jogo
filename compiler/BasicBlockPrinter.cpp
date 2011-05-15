/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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
    environment_(environment) {

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
        f(this);
    } 
}

void BasicBlockPrinter::operator()(Class* feature) {
    class_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockPrinter::operator()(Function* feature) {
    BasicBlock::Ptr block = feature->code();
    visited_.clear();
    operator()(block);     
    cout << endl; 
}

void BasicBlockPrinter::operator()(BasicBlock* block) {
    if (visited_.find(block) != visited_.end()) {
        return;
    }
    visited_.insert(block);

    BasicBlock::Ptr branch = block->branch();
    BasicBlock::Ptr next = block->next();
    if (block->label()) {
        cout << block->label() << ":" << endl;
    }
    for (int i = 0; i < block->length(); i++) {
        Instruction instr = block->instr(i);
        Operand result = instr.result();
        Operand first = instr.first();
        Operand second = instr.second();
        cout << "    ";
        switch (instr.opcode()) {
        case ADD:
            cout << result << " <- " << first << " + " << second << endl;
            break;
        case SUB:
            cout << result << " <- " << first << " - " << second << endl;
            break;
        case MUL:
            cout << result << " <- " << first << " * " << second << endl;
            break; 
        case DIV:
            cout << result << " <- " << first << " / " << second << endl;
            break; 
        case ANDL:
            cout << result << " <- " << first << " and " << second << endl;
            break; 
        case ORL:
            cout << result << " <- " << first << " or " << second << endl;
            break; 
        case ANDB:
            cout << result << " <- " << first << " & " << second << endl;
            break; 
        case ORB:
            cout << result << " <- " << first << " & " << second << endl;
            break; 
        case PUSH:
            cout << "push " << first << endl;
            break; 
        case POP:
            cout << result << " <- " << "pop " << endl;
            break;
        case STORE:
            cout << "store " << first << ", " << second << endl;
            break;
        case LOAD: 
            cout << result << " <- " << "load " << first << endl;
            break;
        case LI:
            cout << result << " <- " << "li " << first << endl;
            break;
        case NOTL:
            cout << result << " <- not " << first << endl;
            break;
        case CALL:
            cout << "call " << result << endl;
            break;
        case JUMP:
            cout << "jump " << branch->label() << endl;
            break;
        case BNE:
            cout << "if " << first << " != " << second << " goto ";
            cout << branch->label() << endl; 
            break;
        case BEQ:
            cout << "if " << first << " == " << second << " goto ";
            cout << branch->label() << endl; 
            break;
        case BEQZ:
            cout << "if " << first << " goto ";
            cout << branch->label() << endl;
            break;
        case BNEQZ:
            cout << "if not " << first << " goto ";
            cout << branch->label() << endl;
            break;
        case RET:
            cout << "ret" << endl;
            break;
        case HALT:
            cout << "halt" << endl;
            break;
        }
    }
    if (block->next()) {
        operator()(block->next());
    }
    if (block->branch()) {
        operator()(block->branch());
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

