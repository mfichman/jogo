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
        emit_string(s);
    }
    for (String::Ptr s = env_->integers(); s; s = s->next()) {
        out_ << "    lit" << (void*)s.pointer() << " dq " << s << "\n";
    }

    out_ << "section .text\n";
    out_ << "extern "; emit_label("calloc"); out_ << "\n";
    out_ << "extern "; emit_label("free"); out_ << "\n";
    out_ << "extern "; emit_label("Object__dispatch"); out_ << "\n";
    out_ << "extern "; emit_label("Object__refcount_dec"); out_ << "\n";
    out_ << "extern "; emit_label("Object__refcount_inc"); out_ << "\n";
    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
    }

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
        out_ << "section .data\n";
        out_ << "global "; emit_label(name->string()+"__vtable"); out_ << "\n";
        emit_label(name->string()+"__vtable"); out_ << ":\n";
        out_ << "    dq "; emit_label(dtor->label()); out_ << "\n"; 
        out_ << "    dq " << feature->jump1s() << "\n";
        for (int i = 0; i < feature->jump1s(); i++) {
            out_ << "    dq " << feature->jump1(i) << "\n";
        } 
        for (int i = 0; i < feature->jump2s(); i++) {
            if (feature->jump2(i)) {
                out_ << "    dq ";
                emit_label(feature->jump2(i)->label());
                out_ << "\n";
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
    // Emit a function, or an extern declaration if the function is native or
    // belongs to a different output file.
    if (feature->is_native()) {
        out_ << "extern "; emit_label(feature->label()); out_ << "\n";
    } else if (feature->basic_blocks()) {
        out_ << "global "; emit_label(feature->label()); out_ << "\n";
        emit_label(feature->label()); out_ << ":\n";
        out_ << "    push rbp\n"; 
        out_ << "    mov rbp, rsp\n";
        if (feature->stack_vars()) {
            // Allocate space on the stack; ensure that the stack is aligned to
            // a 16-byte boundary.
            int stack = feature->stack_vars() * machine_->word_size();
            if (stack % 16 != 0) {
                stack += 16 - (stack % 16);
            }
            out_ << "    sub rsp, " << stack << "\n";
        }
        
        for (int i = 0; i < feature->basic_blocks(); i++) {
            operator()(feature->basic_block(i));
        }
        // Make sure that the text section is gets re-aligned at the end of the
        // function, because the instruction stream can cause it be become
        // unaligned.
        out_ << "    align 8\n";
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
        out_ << block->label() << ":\n";
    }
    for (int i = 0; i < block->instrs(); i++) {
        const Instruction& instr = block->instr(i);
        Operand res = instr.result();
        Operand a1 = instr.first();
        Operand a2 = instr.second();
        opcode_ = instr.opcode();

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
        case ADD: emit_arith(instr); break;
        case SUB: emit_arith(instr); break;
        case MUL: emit_arith(instr); break;
        case DIV: emit_arith(instr); break;
        case PUSH: emit("push", a1); break;
        case POP: emit("pop", res); break;
        case STORE: emit("mov qword", a1, a2); break;
        case LOAD: emit("mov qword", res, a1); break;
        case NOTB: emit("mov", res, a1); emit("not", res); break;
        case ANDB: emit("mov", res, a1); emit("and", res, a2); break;
        case ORB: emit("mov", res, a1); emit("or", res, a2); break; 
        case NOP: emit("nop"); break;
        case RET: emit("leave"); emit("ret"); break;
        }
    }
}

void Intel64CodeGenerator::emit_arith(const Instruction& inst) {
    // Emits an arithmetic instruction.  Depending on the opcode and the
    // operands, the expression may have to be manipulated because all x86
    // instructions take only 2 arithmetic operands.
    const char* instr;
    switch (inst.opcode()) {
    case ADD: instr = "add"; break;
    case SUB: instr = "sub"; break;
    case MUL: instr = "mul"; break;
    case DIV: instr = "div"; break;
    default: assert(false);
    }

    Operand res = inst.result();
    Operand r1 = inst.first();
    Operand r2 = inst.second();
    
    if (res.temp() == r1.temp()) {
        // t1 <- t1 - t2
        emit(instr, r1, r2);
    } else if (res.temp() != r2.temp()) {
        // t1 <- t2 - t3
        emit("mov", res, r1); 
        emit(instr, res, r2);
    } else if (inst.opcode() == ADD || inst.opcode() == MUL) {
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

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Operand r2) {
    // Emits an instruction with two operands.
    out_ << "    " << instr << " ";
    emit_operand(r1);
    out_ << ", ";
    emit_operand(r2);
    out_ << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1) {
    // Emits a single-operand instruction (either literal or register)
    out_ << "    " << instr << " ";
    emit_operand(r1);
    out_ << "\n";
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, const char* imm) {
    // Instruction that operates on a register r1 and an immediate value. 
    out_ << "    " << instr << " ";
    emit_register(r1);
    out_ << ", " << imm << "\n";
}

void Intel64CodeGenerator::emit(const char* instr) {
    // Emits a no-operand instruction.
    out_ << "    " << instr << "\n";
}

void Intel64CodeGenerator::emit_operand(Operand op) {
    // Emits any operand.  This function will automatically determine which
    // type of     operand to output.
    if (op.label()) {
        emit_label(op);
    } else if (op.literal()) {
        emit_literal(op);
    } else if (op.indirect()) {
        emit_addr(op);
    } else if (op.temp()) {
        emit_register(op);
    } else {
        assert(!"Nil operand");
    } 
}

void Intel64CodeGenerator::emit_addr(Operand op) {
    // Emits the the address operand for a stack location as a an offset from
    // the base pointer (rbp).  No register should be specified in the operand,
    // as the register is understood to be rbp.  Likewise, the label and
    // literal fields should also be nil.
    assert(!op.literal());
    assert(!op.label());
    assert(op.addr() || op.temp());

    out_ << "[";    
    if (op.temp()) {
        out_ << machine_->reg(-op.temp());
    } else {
        out_ << "rbp";
    }
    if (op.addr() > 0) {
        // Add +1 if loading from the base pointer, because the SP is stored 
        // at location 0
        int addr = op.temp() ? op.addr() : op.addr()+1;
        out_ << "+" << addr * machine_->word_size(); 
    } else if (op.addr() < 0) {
        out_ << "-" << -op.addr() * machine_->word_size();
    }
    out_ << "]";
}

void Intel64CodeGenerator::emit_register(Operand op) {
    // Outputs a register, possibly with an address offset.  An operand passed
    // to this function should NOT have the literal or label fields set.
    assert(op.temp() < 0 && -op.temp() < machine_->regs());
    assert(!op.literal());
    assert(!op.label());
    //assert(!op.indirect());
    //assert(!op.addr());

    out_ << machine_->reg(-op.temp());
}

void Intel64CodeGenerator::emit_literal(Operand literal) {
    // Outputs the correct representation of a literal.  If the literal is a
    // number, and the length is less than 13 bits, then output the literal
    // directly in the instruction.  Otherwise, load it from the correct memory
    // address for the literal.

    Expression* expr = literal.literal();
    assert(!literal.temp());
    assert(expr);

    //assert(!dynamic_cast<StringLiteral*>(expr));
    // String literals can't be loaded immediately due to the requirement for
    // RIP-relative addressing on 64-bit systems.  Instead, the address to the
    // string must be loaded using LEA first (i.e., LOAD in the IR language).

    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        out_ << "lit" << (void*)le->value();
    }
    if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        out_ << le->value();
    } else if (NilLiteral* le = dynamic_cast<NilLiteral*>(expr)) {
        out_ << "0";
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
    }
}


void Intel64CodeGenerator::emit_label(Operand op) {
    // Emits a label, either as an operand or as an actual label at the
    // beginning of a code block.  No register/literal/addr fields should be
    // set on the operand.
    assert(!op.temp());
    assert(!op.literal());
    assert(!op.addr()); 
    assert(op.label());

    emit_label(op.label()->string());
}

void Intel64CodeGenerator::emit_label(const std::string& label) {
    // Emits a label, either as an operand or as an actual label at the
    // beginning of a code block.

    if (label[0] == '.') {
        out_ << label;
    } else {
#ifdef DARWIN
        out_ << "_" << label;
#else
        out_ << label;
#endif   
    }
}

void Intel64CodeGenerator::emit_string(String* string) {
    // Output a string literal, and correctly process escape sequences.

    std::string in = string->string();
    std::string out;
    int length = 0;
    bool escaped = true;
    bool first = true;
    for (int i = 0; i < in.length(); i++) {
        char c = in[i];
        if (c == '\\') {
            // Output escape sequences.  For NASM, the actual hex codes must
            // be output for non-visible characters; there is no escape
            // character.
            if (!escaped) out += "\"";
            if (!first) out += ", ";

            c = in[++i];
            if (isdigit(c)) { // Octal code
                char c2 = in[++i];
                char c3 = in[++i]; 
                out += std::string("0o") + c + c2 + c3;
            } else if (c == 'x') { // Hexadecimal code
                char c1 = in[++i];
                char c2 = in[++i];
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

    out_ << "lit" << (void*)string << ": \n";  
    out_ << "    dq ";
    emit_label("String__vtable");
    out_ << "\n"; // vtable
    out_ << "    dq 1\n"; // reference count
    out_ << "    dq " << length << "\n";
    out_ << "    db " << out << "\n";
    out_ << "    align 8\n";
}
