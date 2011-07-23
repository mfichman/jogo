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

// RAX is reserved for special things
static const Register::Ptr AL(new Register("al", 0));
static const Register::Ptr RAX(new Register("rax", 0));

Intel64CodeGenerator::Intel64CodeGenerator(Environment* env) :
    environment_(env),
    machine_(Machine::intel64()),
    out_(env->output().c_str()) {

    if (environment_->errors()) {
        return;
    }

    for (String::Ptr s = environment_->strings(); s; s = s->next()) {
        out_ << "str" << (void*)s.pointer() << ": " << std::endl;  
        out_ << "    dq 0" << std::endl; // vtable
        out_ << "    dq 1" << std::endl; // reference count
        out_ << "    dq " << s->string().length()-1 << std::endl;
        out_ << "    db \"";

        bool needs_unquote = true;
        for (int i = 0; i < s->string().length(); i++) {
            if (s->string()[i] == '\\') { 
                i++;
                if (i >= s->string().length()) { return; }
                if (s->string()[i] == 'n') {
                    out_ << "\", 0xa";
                    if (i+1 < s->string().length()) { 
                        out_ << "\"";
                        needs_unquote = true;
                    } else {
                        needs_unquote = false;
                    }
                }
            } else {
                out_ << s->string()[i];
            }
        }
        if (needs_unquote) {
            out_ << "\"";
        } else {
            out_ << ", 0x0" << std::endl;
        }
    }

    for (String::Ptr s = environment_->integers(); s; s = s->next()) {
        out_ << "    int" << (void*)s.pointer() << " dq " << s << std::endl;
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
    BasicBlock::Ptr block = feature->code();
    if (feature->is_native()) {
        out_ << "extern " << feature->label() << std::endl;
    } else if (block->instrs()) {
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
        case BNE: emit("cmp", a1, a2); emit("jne", branch->label()); break;
        case BEQ: emit("cmp", a1, a2); emit("je", branch->label()); break;
        case MOV: emit("mov", res, a1); break;
        case BEQZ: emit("cmp", a1, "0"); emit("je", branch->label()); break;
        case BNEQZ: emit("cmp", a1, "0"); emit("jne", branch->label()); break; 
        case ADD: arith("add", res, a1, a2); break;
        case SUB: arith("sub", res, a1, a2); break;
        case MUL: arith("mul", res, a1, a2); break;
        case DIV: arith("div", res, a1, a2); break;
        case EQ: 
            emit("cmp", a1, a2); 
            emit("sete", AL); 
            emit("movzx", res, AL);
            break;
        case ANDL: assert(false); break;
        case ORL: emit("mov", res, a1); emit("or", res, a2); break;
        case PUSHARG: 
            if (push_count < 0) {
                push_count = 0;
                for (int j = i+1; j < block->instrs(); j++) {
                    if (block->instr(j).opcode() == CALL) {
                        break;
                    }
                    push_count++;
                }
            }
            if (push_count >= machine_->arg_regs()) {
                emit("push", a1);
            } else {
                emit("mov", machine_->arg_reg(push_count), a1);
            }
            push_count--;
            break;
        case PUSHRET: emit("mov", RAX, a1); break;
        case POPARG:
            if (pop_count >= machine_->arg_regs()) {
                emit("pop", res);
            } else {
                emit("mov", res, machine_->arg_reg(pop_count));
            }
            pop_count++;
            break;
        case POPRET: emit("mov", res, RAX); break;
        case STORE: store(res, a1); break;
        case LOAD: load(res, a1); break;
        case LI: li(res, a1); break;
        case STR: str(res, a1); break;
        case NOTL: emit("mov", res, a1); emit("not", res); break;
        case ANDB: emit("mov", res, a1); emit("and", res, a2); break;
        case ORB: emit("mov", res, a1); emit("or", res, a2); break; 
        case RET: emit("leave"); emit("ret"); break;
        case HALT: break;
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
        emit("mov", r1, RAX);
    }
    
}

void Intel64CodeGenerator::store(Operand r1, Operand r2) {
    assert(r1.temporary() && r2.temporary());
    assert(r1.temporary() < machine_->regs());
    assert(r2.temporary() < machine_->regs());
    out_ << "    mov [";
    out_ << machine_->reg(r1.temporary()) << "], ";
    out_ << machine_->reg(r2.temporary()) << std::endl;
}

void Intel64CodeGenerator::load(Operand r1, Operand r2) {
    assert(r1.temporary() && r2.temporary());
    assert(r1.temporary() < machine_->regs());
    assert(r2.temporary() < machine_->regs());
    out_ << "    mov ";
    out_ << machine_->reg(r1.temporary()) << ", [";
    out_ << machine_->reg(r2.temporary()) << "]" << std::endl;
}

void Intel64CodeGenerator::str(Operand r1, Operand r2) {
    assert(r1.temporary());
    assert(r1.temporary() < machine_->regs());
    out_ << "    mov ";
    out_ << machine_->reg(r1.temporary()) << ", ";
    out_ << "str" << (void*)r2.literal().pointer() << std::endl;
}

void Intel64CodeGenerator::li(Operand r1, Operand r2) {
    assert(r1.temporary());
    assert(r1.temporary() < machine_->regs());

    if (r2.literal()->string() == "false") {
        out_ << "    mov ";
        out_ << machine_->reg(r1.temporary()) << ", ";
        out_ << "0" << std::endl;  
    } else if (r2.literal()->string() == "true") {
        out_ << "    mov ";
        out_ << machine_->reg(r1.temporary()) << ", ";
        out_ << "1" << std::endl;
    } else {
        out_ << "    mov ";
        out_ << machine_->reg(r1.temporary()) << ", ";
        out_ << "int" << (void*)r2.literal().pointer() << std::endl;
        out_ << "    mov ";
        out_ << machine_->reg(r1.temporary()) << ", [";
        out_ << machine_->reg(r1.temporary()) << "]" << std::endl;
    }
}

void Intel64CodeGenerator::emit(const char* instr, Register* r2) {
    out_ << "    " << instr << " " << r2 << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Register* r1, Operand r2) {
    assert(r2.temporary());
    assert(r2.temporary() < machine_->regs());

    out_ << "    " << instr << " ";
    out_ << r1 << ", " << machine_->reg(r2.temporary()) << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Operand r2) {
    assert(r1.temporary() && r2.temporary());
    assert(r1.temporary() < machine_->regs());
    assert(r2.temporary() < machine_->regs());
    
    if (std::string("mov") == instr && r1.temporary() == r2.temporary()) {
        return; // mov a, a
    }

    out_ << "    " << instr << " ";
    out_ << machine_->reg(r1.temporary()) << ", ";
    if (r2.temporary()) {
        out_ << machine_->reg(r2.temporary()) << std::endl;
    } else {
        out_ << "str" << (void*)r2.literal().pointer() << std::endl;
    }
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1) {
    assert(r1.temporary() < machine_->regs());
    out_ << "    " << instr << " ";
    if (r1.temporary()) {
        out_ << machine_->reg(r1.temporary()) << std::endl;
    } else {
        out_ << r1.literal() << std::endl;
        // FixMe: Print constant label!
    }
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, Register* o2) {
    assert(r1.temporary() < machine_->regs());
    assert(r1.temporary());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(r1.temporary()) << ", ";
    out_ << o2 << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, Operand r1, const char* imm) {
    assert(r1.temporary() < machine_->regs());
    assert(r1.temporary());
    out_ << "    " << instr << " ";
    out_ << machine_->reg(r1.temporary()) << ", ";
    out_ << imm << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr) {
    out_ << "    " << instr << std::endl;
}

void Intel64CodeGenerator::emit(const char* instr, String* label) {
    out_ << "    " << instr << " " << label << std::endl;
}

