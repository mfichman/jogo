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

#include "Intel64CodeGenerator.hpp"
#include <sstream>

// RAX is reserved for special things
static const int MAXIMM = 4096;
static const Register::Ptr AL(new Register("al", 0));
static const Register::Ptr RAX(new Register("rax", 0));
static const Register::Ptr ESP(new Register("esp", 0));
static const Register::Ptr EBP(new Register("ebp", 0));

Intel64CodeGenerator::Intel64CodeGenerator(Environment* env) :
    environment_(env),
    machine_(Machine::intel64()),
    out_(env->output().c_str()) {

    if (environment_->errors()) {
        return;
    }

    for (String::Ptr s = environment_->strings(); s; s = s->next()) {
        string(s);
    }

    for (String::Ptr s = environment_->integers(); s; s = s->next()) {
        out_ << "    lit" << (void*)s.pointer() << " dq " << s << std::endl;
    }

    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }

    out_ << "global _main" << std::endl;
    out_ << "_main:" << std::endl;
    out_ << "    push rbp" << std::endl;
    out_ << "    mov rbp, rsp" << std::endl;
    out_ << "    call main" << std::endl;
    out_ << "    leave" << std::endl;
    out_ << "    ret" << std::endl;
}


void Intel64CodeGenerator::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void Intel64CodeGenerator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void Intel64CodeGenerator::operator()(Function* feature) {
    if (feature->is_native()) {
        out_ << "extern " << feature->label() << std::endl;
    } else if (feature->basic_blocks()) {
        out_ << "global " << feature->label() << std::endl;
        out_ << feature->label() << ":" << std::endl;
        out_ << "    push rbp" << std::endl; 
        out_ << "    mov rbp, rsp" << std::endl;
        
        for (int i = 0; i < feature->basic_blocks(); i++) {
            operator()(feature->basic_block(i));
        }
    }
}

void Intel64CodeGenerator::operator()(BasicBlock* block) {
    // Translate a basic block in three-address code into x86.  For most 
    // operations, this requires a "mov, op" sequence.  PUSH and POP 
    // instructions are handled specially, since the first 6 parameters to
    // a function in Intel64 are passed using registers.  The rest are passed
    // on the stack.
    // FixMe: Different registers are used on Windows vs. Unix
    // FixMe: Remove hardcoded Operand(x) and replace with symbolic reg names.
    // FixMe: Don't hardcode the # of register params.

    
    int pop_count = 0;
    int push_count = -1;

    BasicBlock::Ptr branch = block->branch();
    BasicBlock::Ptr next = block->next();
    if (block->label()) {
        out_ << block->label() << ":" << std::endl;
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        Operand res = instr.result();
        Operand a1 = instr.first();
        Operand a2 = instr.second();

        //case ADD: emit("mov", res, a1); emit("add", res, a2); break;
        switch (instr.opcode()) {
        case CALL: emit("call", res); push_count = 0; break;
        case JUMP: emit("jmp", branch->label()); break;
        case MOV: emit("mov", res, a1); break;
        case BNE: emit("cmp", a1, a2); emit("jne", branch->label()); break;
        case BE: emit("cmp", a1, a2); emit("je", branch->label()); break;
        case BZ: emit("cmp", a1, "0"); emit("je", branch->label()); break;
        case BNZ: emit("cmp", a1, "0"); emit("jne", branch->label()); break; 
        case BG: emit("cmp", a1, a2); emit("jg", branch->label()); break;
        case BGE: emit("cmp", a1, a2); emit("jge", branch->label()); break;
        case BL: emit("cmp", a1, a2); emit("jl", branch->label()); break;
        case BLE: emit("cmp", a1, a2); emit("jle", branch->label()); break;
        case ADD: arith("add", res, a1, a2); break;
        case SUB: arith("sub", res, a1, a2); break;
        case MUL: arith("mul", res, a1, a2); break;
        case DIV: arith("div", res, a1, a2); break;
        case PUSH: emit("push", a1); break;
        case POP: emit("pop", res); break;
        case STORE: store(res, a1, instr.offset()); break;
        case LOAD: load(res, a1, instr.offset()); break;
        case NOTB: emit("mov", res, a1); emit("not", res); break;
        case ANDB: emit("mov", res, a1); emit("and", res, a2); break;
        case ORB: emit("mov", res, a1); emit("or", res, a2); break; 
        case NOP: emit("nop"); break;
        case RET: emit("leave"); emit("ret"); break;
        }
    }
}

void Intel64CodeGenerator::arith(const char* instr, Operand res, Operand r1,
        Operand r2) {
    
    if (res.temporary() == r1.temporary()) {
        // t1 <- t1 - t2
        emit(instr, r1, r2);
    } else if (res.temporary() != r2.temporary()) {
        // t1 <- t2 - t3
        emit("mov", res, r1); 
        emit(instr, res, r2);
    } else if (std::string("add") == instr || std::string("mul") == instr) {
        // t1 <- t2 + t1
        emit(instr, r2, r1);
    } else {
        
        // t1 <- t2 - t1
        emit("mov", RAX, r2); 
        emit(instr, RAX, r1);
        emit("mov", res, RAX);
    }
    
}

void Intel64CodeGenerator::literal(Operand literal) {
    // Outputs the correct representation of a literal.  If the literal is a
    // number, and the length is less than 13 bits, then output the literal
    // directly in the instruction.  Otherwise, load it from the correct
    // memory address for the literal.
    assert(!literal.temporary());
    Expression* expr = literal.literal();
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        out_ << "lit" << (void*)le->value();
        return;
    }
    if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        out_ << le->value();
        return;
    }
    if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
        std::stringstream ss(le->value()->string());
        int number;
        ss >> number;
        if(number < MAXIMM) {
            out_ << le->value();
        } else {
            out_ << "[lit" << (void*)le->value() << "]";
        }
        return;
    }
    if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        out_ << "[lit" << (void*)le->value() << "]";
        return;
    }
}

void Intel64CodeGenerator::store(Operand r1, Operand r2, int offset) {
    // Moves the literal or register in r2 into the memory address specified
    // by register r1. 
    assert(r1.temporary());
    assert(-r1.temporary() < machine_->regs());
    out_ << "    mov ";
    if (r1.temporary()) {
        // Store to memory address specified by register plus offset.
        out_ << "[" << machine_->reg(-r1.temporary()) << "+";
        out_ << offset * machine_->word_size() << "], ";
    } else {
        // Store to memory address specified by offset from base pointer.
        out_ << "[ebp+" << offset * machine_->word_size() << "]";
    }


    if (r2.temporary()) { // Register
        assert(-r2.temporary() < machine_->regs());
        out_ << machine_->reg(-r2.temporary());
    } else { // Literal
        literal(r2); 
    }
    out_ << std::endl;
}

void Intel64CodeGenerator::load(Operand r1, Operand r2, int offset) {
    // Loads the data at memory address r2 into register r1.  If r2 is a 
    // literal, then an immediate load is done.
    assert(r1.temporary());
    assert(-r1.temporary() < machine_->regs());
    out_ << "    mov " << machine_->reg(-r1.temporary()) << ", ";
    if (r2.temporary()) {
        assert(-r2.temporary() < machine_->regs());
        // Load from memory address specified by register operand plus offset.
        out_ << "[" << machine_->reg(-r2.temporary()) << "+";
        out_ << offset * machine_->word_size() << "]";
    } else if (r2.literal()) {
        // Load a literal value.
        literal(r2);
    } else {
        // Load from memory address specified by offset from the base pointer.
        out_ << "[rbp+" << offset * machine_->word_size() << "]";
    }
    out_ << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Register* r2) {
    // Emits a simple 1-register instruction
    out_ << "    " << instr << " " << r2 << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Register* r1, Operand r2) {
     // Emits an instruction with a possible literal right hand side.
    out_ << "    " << instr << " " << r1 << ", ";
    if (r2.temporary()) {
        assert(-r2.temporary() < machine_->regs());
        out_ << machine_->reg(-r2.temporary());
    } else {
        literal(r2);
    }
    out_  << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Operand r2) {
    // Emits  an instruction with a register left operand and a possible 
    // literal right operand.
    assert(r1.temporary());
    assert(r1.temporary() < machine_->regs());
    if (std::string("mov") == instr && r1.temporary() == r2.temporary()) {
        return; // mov a, a
    }
    out_ << "    " << instr << " " << machine_->reg(-r1.temporary()) << ", ";
    if (r2.temporary()) {
        assert(-r2.temporary() < machine_->regs());
        out_ << machine_->reg(-r2.temporary());
    } else {
        literal(r2);
    }
    out_ << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1) {
    // Emit a single-operand instruction (either literal or register)
    assert(r1.temporary() < machine_->regs());
    out_ << "    " << instr << " ";
    if (r1.temporary()) {
        out_ << machine_->reg(r1.temporary());
    } else if (std::string("call") == instr) {
        out_ << r1.label();
    } else {
        literal(r1);
    }
    out_ << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Register* o2) {
    // Operation on a register and another register specified directly.
    assert(r1.temporary());
    assert(-r1.temporary() < machine_->regs());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(-r1.temporary()) << ", ";
    out_ << o2 << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, const char* imm) {
    // Instruction that operates on a register r1 and an immediate value. 
    assert(r1.temporary());
    assert(-r1.temporary() < machine_->regs());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(-r1.temporary()) << ", ";
    out_ << imm << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr) {
    out_ << "    " << instr << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, String* label) {
    out_ << "    " << instr << " " << label << std::endl;
}

void Intel64CodeGenerator::string(String* string) {
    // Output a string literal, and correctly process escape sequences.

    std::string out;
    int length = 0;
    bool escaped = true;
    bool first = true;
    for (int i = 0; i < string->string().length(); i++) {
        char c = string->string()[i];
        if (c == '\\') {
            // Output escape sequences.  For NASM, the actual hex codes must
            // be output for non-visible characters; there is no escape
            // character.
            if (!escaped) out += "\"";
            if (!first) out += ", ";

            c = string->string()[++i];
            if (isdigit(c)) { // Octal code
                char c2 = string->string()[++i];
                char c3 = string->string()[++i]; 
                out += std::string("0o") + c + c2 + c3;
            } else if (c == 'x') { // Hexadecimal code
                char c1 = string->string()[++i];
                char c2 = string->string()[++i];
                out += std::string("0x") + c1 + c2;
            } else {
                switch (c) {
                case 'a': out += "0x7"; break; // alarm
                case 'b': out += "0x8"; break; // backspace
                case 't': out += "0x9"; break; // horizontal tab
                case 'n': out += "0xa"; break; // newline
                case 'v': out += "0xb"; break; // vertical tab
                case 'f': out += "0xc"; break; // form feed
                case 'r': out += "0xd"; break; // carriage return
                case '"': out += "0x42"; break; // quote
                case '\'': out += "0x47"; break; // quote
                default: out += c; break;
                }
            }
            escaped = true;
        } else {
            if (escaped) {
                if (!first) out += ", ";
                out += "\"";
            }
            out += c;
            escaped = false;
        }
        length++;
        first = false;
    }
    if (!escaped) out += "\"";

    out_ << "lit" << (void*)string << ": " << std::endl;  
    out_ << "    dq 0" << std::endl; // vtable
    out_ << "    dq 1" << std::endl; // reference count
    out_ << "    dq " << length << std::endl;
    out_ << "    db " << out << std::endl;
}
