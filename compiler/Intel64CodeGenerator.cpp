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

static const int MAXIMM = 4096;
static const Register::Ptr ESP(new Register("rsp", 0, false));
static const Register::Ptr EBP(new Register("rbp", 0, false));

Intel64CodeGenerator::Intel64CodeGenerator(Environment* env, Stream* out) :
    env_(env),
    machine_(Machine::intel64()),
    out_(out) {

    if (env_->errors()) {
        return;
    }
    
    out_ << "default rel\n";
    out_ << "section .data\n";
    for (String::Ptr s = env_->strings(); s; s = s->next()) {
        string(s);
    }
    for (String::Ptr s = env_->integers(); s; s = s->next()) {
        out_ << "    lit" << (void*)s.pointer() << " dq " << s << "\n";
    }

    out_ << "section .text\n";
    out_ << "extern _calloc\n";
    out_ << "extern _Object__dispatch\n";
    out_ << "extern _Object__refcount_dec\n";
    out_ << "extern _Object__refcount_inc\n";
    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
    }

    out_ << "global _main\n";
    out_ << "_main:\n";
    out_ << "    push rbp\n";
    out_ << "    mov rbp, rsp\n";
    out_ << "    call main\n";
    out_ << "    leave\n";
    out_ << "    ret\n";
    out_->flush();
}


void Intel64CodeGenerator::operator()(Class* feature) {
    // Output the class dispatch table for calling methods with dynamic
    // dispatch.  The format is as follows: 
    //
    //     vtable[0] is the destructor
    //     vtable[1] is the number of 'slots' in the vtable (n)
    //     vtable[2..n+1] are the hash mixing values
    //     vtable[2n+2..2n+1] are the actual method pointers
    //
    // The table is output in the .text section because it should be 
    // immutable.

    if (feature->is_object()) {
        String::Ptr name = feature->name();
        Function::Ptr dtor = feature->function(env_->name("@destroy"));
        out_ << "section .text\n";
        out_ << "align 8\n";
        out_ << "global _" << name << "__vtable\n";
        out_ << "_" << name << "__vtable:\n";
        out_ << "    dq " << dtor->label() << "\n"; 
        out_ << "    dq " << feature->jump1s() << "\n";
        for (int i = 0; i < feature->jump1s(); i++) {
            out_ << "    dq " << feature->jump1(i) << "\n";
        } 
        for (int i = 0; i < feature->jump2s(); i++) {
            if (feature->jump2(i)) {
                String::Ptr label = feature->jump2(i)->label();
                out_ << "    dq " << label << "\n";
            } else {
                out_ << "    dq 0\n";
            }
        }
    }

    // Now output the function defs
    out_ << "section .text\n";
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
        out_ << "extern " << feature->label() << "\n";
    } else if (feature->basic_blocks()) {
        out_ << "global " << feature->label() << "\n";
        out_ << feature->label() << ":\n";
        out_ << "    push rbp\n"; 
        out_ << "    mov rbp, rsp\n";
        if (feature->stack_vars()) {
            // Allocate space on the stack; ensure that the stack is aligned
            // to a 16-byte boundary.
            int stack = feature->stack_vars() * machine_->word_size();
            if (stack % 16 != 0) {
                stack += 16 - (stack % 16);
            }
            out_ << "    sub rsp, " << stack << "\n";
        }
        
        for (int i = 0; i < feature->basic_blocks(); i++) {
            operator()(feature->basic_block(i));
        }
    }
}

void Intel64CodeGenerator::operator()(BasicBlock* block) {
    // Translate a basic block in three-address code into x86.  For most
    // operations, this requires a "mov, op" sequence.  PUSH and POP
    // instructions are handled specially, since the first 6 parameters to a
    // function in Intel64 are passed using registers.  The rest are passed on
    // the stack.  
    // FixMe: Different registers are used on Windows vs. Unix
    // FixMe: Remove hardcoded Operand(x) and replace with symbolic reg names.
    // FixMe: Don't hardcode the # of register params.

    
    BasicBlock::Ptr branch = block->branch();
    BasicBlock::Ptr next = block->next();
    if (block->label()) {
        out_ << "." << block->label() << ":\n";
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        Operand res = instr.result();
        Operand a1 = instr.first();
        Operand a2 = instr.second();

        switch (instr.opcode()) {
        case CALL: emit("call", a1); break;
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
        case STORE: store(a1, a2); break;
        case LOAD: load(res, a1); break;
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
    
    if (res.temp() == r1.temp()) {
        // t1 <- t1 - t2
        emit(instr, r1, r2);
    } else if (res.temp() != r2.temp()) {
        // t1 <- t2 - t3
        emit("mov", res, r1); 
        emit(instr, res, r2);
    } else if (std::string("add") == instr || std::string("mul") == instr) {
        // t1 <- t2 + t1
        emit(instr, r2, r1);
    } else {
        
        // t1 <- t2 - t1
        emit("push", r2);
        emit(instr, r2, r1);
        emit("mov", r1, r2);
        emit("pop", r2);        

    }
    
}

void Intel64CodeGenerator::literal(Operand literal) {
    // Outputs the correct representation of a literal.  If the literal is a
    // number, and the length is less than 13 bits, then output the literal
    // directly in the instruction.  Otherwise, load it from the correct
    // memory address for the literal.

    assert(!literal.temp());
    Expression* expr = literal.literal();

    assert(!dynamic_cast<StringLiteral*>(expr));
    // String literals can't be loaded immediately due to the requirement for
    // RIP-relative addressing on 64-bit systems.  Instead, the address to the
    // string must be loaded using LEA first (i.e., LOAD in the IR language).

    if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        out_ << le->value();
    } else if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
        std::stringstream ss(le->value()->string());
        int number;
        ss >> number;
        if(number < MAXIMM) {
            out_ << le->value();
        } else {
            out_ << "[lit" << (void*)le->value() << "]";
        }
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        out_ << "[lit" << (void*)le->value() << "]";
        return;
    }
}

void Intel64CodeGenerator::store(Operand r1, Operand r2) {
    // Moves the literal or register in r2 into the memory address specified
    // by register r1. 
    assert(!r2.label());

    out_ << "    mov ";

    if (r1.temp()) {
        assert(-r1.temp() < machine_->regs());
        // Store to memory address specified by register plus offset.
        out_ << "[" << machine_->reg(-r1.temp()) << "+";
        out_ << r1.addr() * machine_->word_size() << "], ";
    } else {
        // Store to memory address specified by offset from base pointer.
        if (r1.addr() < 0) {
            out_ << "[rbp" << r1.addr() * machine_->word_size() << "], ";
        } else {
            out_ << "[rbp+" << r1.addr() * machine_->word_size() << "], ";
        }
    }

    if (r2.temp()) { // Register
        assert(-r2.temp() < machine_->regs());
        out_ << machine_->reg(-r2.temp());
    } else { // Literal
        literal(r2); 
    }
    out_ << "\n";
}

void Intel64CodeGenerator::load(Operand r1, Operand r2) {
    // Loads the data at memory address r2 into register r1.  If r2 is a 
    // literal, then an immediate load is done.
    assert(r1.temp() < 0);
    assert(-r1.temp() < machine_->regs());
    if (r2.label()) {
        out_ << "    lea " << machine_->reg(-r1.temp()) << ", ";
        out_ << "[" << r2.label()->string() << "]";
    }
    else if (r2.temp()) {
        out_ << "    mov " << machine_->reg(-r1.temp()) << ", ";
        assert(-r2.temp() < machine_->regs());
        // Load from memory address specified by register operand plus offset.
        out_ << "[" << machine_->reg(-r2.temp()) << "+";
        out_ << r2.addr() * machine_->word_size() << "]";
    } else if (r2.literal()) {
        // Outputs the correct representation of a literal.  If the literal is a
        // number, and the length is less than 13 bits, then output the literal
        // directly in the instruction.  Otherwise, load it from the correct
        // memory address for the literal.
        Expression* expr = r2.literal();
        if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
            out_ << "    lea " << machine_->reg(-r1.temp()) << ", ";
            out_ << "[lit" << (void*)le->value() << "]";
        } else if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
            out_ << "    mov " << machine_->reg(-r1.temp()) << ", "; 
            out_ << le->value();
        } else if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
            std::stringstream ss(le->value()->string());
            int number;
            ss >> number;
            if(number < MAXIMM) {
                out_ << "    mov " << machine_->reg(-r1.temp()) << ", ";
                out_ << le->value();
            } else {
                out_ << "    mov " << machine_->reg(-r1.temp()) << ", ";
                out_ << "[lit" << (void*)le->value() << "]";
            }
        } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
            out_ << "    mov " << machine_->reg(-r1.temp()) << ", ";
            out_ << "[lit" << (void*)le->value() << "]";
            return;
        }
    } else {
        // Load from memory address specified by offset from the base pointer.
        out_ << "    mov " << machine_->reg(-r1.temp()) << ", ";
        if (r2.addr() < 0) {
            out_ << "[rbp" << r2.addr() * machine_->word_size() << "]";
        } else {
            out_ << "[rbp+" << (r2.addr()+1) * machine_->word_size() << "]";
            // Need to add +1.  For Intel64, the stack-pushed parameters start
            // at rbp+16, and up.  The intermediate languages specifies stack-
            // pushed parameters using addresses 1..n.
        }
    }
    out_ << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Register* r2) {
    // Emits a simple 1-register instruction
    out_ << "    " << instr << " " << r2 << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Register* r1, Operand r2) {
     // Emits an instruction with a possible literal right hand side.
    out_ << "    " << instr << " " << r1 << ", ";
    if (r2.temp()) {
        assert(-r2.temp() < machine_->regs());
        out_ << machine_->reg(-r2.temp());
    } else {
        literal(r2);
    }
    out_  << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Operand r2) {
    // Emits  an instruction with a register left operand and a possible 
    // literal right operand.
    assert(r1.temp());
    assert(r1.temp() < machine_->regs());
    if (std::string("mov") == instr && r1.temp() == r2.temp()) {
        return; // mov a, a
    }
    out_ << "    " << instr << " " << machine_->reg(-r1.temp()) << ", ";
    if (r2.temp()) {
        assert(-r2.temp() < machine_->regs());
        out_ << machine_->reg(-r2.temp());
    } else {
        literal(r2);
    }
    out_ << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1) {
    // Emit a single-operand instruction (either literal or register)
    assert(r1.temp() < machine_->regs());
    out_ << "    " << instr << " ";
    if (r1.temp()) {
        out_ << machine_->reg(-r1.temp());
    } else if (std::string("call") == instr) {
        out_ << r1.label();
    } else {
        literal(r1);
    }
    out_ << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Register* o2) {
    // Operation on a register and another register specified directly.
    assert(r1.temp());
    assert(-r1.temp() < machine_->regs());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(-r1.temp()) << ", ";
    out_ << o2 << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, const char* imm) {
    // Instruction that operates on a register r1 and an immediate value. 
    assert(r1.temp());
    assert(-r1.temp() < machine_->regs());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(-r1.temp()) << ", ";
    out_ << imm << "\n";
}

void Intel64CodeGenerator::emit(const char* instr) {
    out_ << "    " << instr << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, String* label) {
    out_ << "    " << instr << " ." << label << "\n";
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
                case '"': out += "0x22"; break; // quote
                case '\'': out += "0x27"; break; // quote
                default: out += std::string("\"") + c + '"'; break;
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
    if (escaped) {
        if (!first) out += ", ";
        out += "0x0";
    } else {
        out += "\", 0x0";
    }

    out_ << "align 8\n";
    out_ << "lit" << (void*)string << ": \n";  
    out_ << "    dq _String__vtable\n"; // vtable
    out_ << "    dq 1\n"; // reference count
    out_ << "    dq " << length << "\n";
    out_ << "    db " << out << "\n";
}
