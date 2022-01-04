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

#include "Nasm64Generator.hpp"
#include <sstream>
#include <stdint.h>
#include <cstdlib>

#ifdef WINDOWS
#define strtoull _strtoui64
#endif

Machine::Ptr const MACHINE = Machine::intel64();
RegisterId const RAX(MACHINE->reg("rax")->id());
RegisterId const XMM0(MACHINE->reg("xmm0")->id());
static const int INTEL64_MAX_IMM = 4096;
static const int INTEL64_MIN_STACK = 4096;

Nasm64Generator::Nasm64Generator(Environment* env) :
    env_(env),
    machine_(Machine::intel64()) {

}

void Nasm64Generator::operator()(File* file) {
    // Output a translation unit, which can include constants, classes,
    // literals, forward declarations, and function definitions.
    if (env_->errors()) { return; }

    //out_ << "default rel\n";
    // Output string literals, integer literals, and constants (enums, floats)
    out_ << "section .data\n";
    align();
    for (String::Itr s = env_->strings(); s; ++s) {
        string(s);
    }
    for (String::Itr s = env_->integers(); s; ++s) {
        out_ << "lit" << (void*)s.pointer() << ":\n";
        out_ << "    dq ";
        if (s->string()[0] == '.') {
            out_ << '0';
        }
        out_ << s.pointer() << "\n";
        align();
    }
    for (Constant::Itr cons = file->constants(); cons; ++cons) {
        if (cons->type()->is_value() && !cons->type()->is_primitive()) {
            assert(!"Not supported");
        }
        out_ << "global "; label(Operand(cons->label())); out_ << "\n";
        Expression* init = cons->initializer();
        if (Construct* constr = dynamic_cast<Construct*>(init)) {
            init = constr->arguments();
            // Support for primitive constructors, e.g., Char(0xf).  FIXME:
            // Should use constant folding instead, to allow all constant
            // expressions.
        }
        label(Operand(cons->label()));
        if(IntegerLiteral::Ptr lit = dynamic_cast<IntegerLiteral*>(init)) {
            out_ << " dq " << lit->value() << "\n";
        } else if(FloatLiteral::Ptr lit = dynamic_cast<FloatLiteral*>(init)) {
            out_ << " dq ";
            if (lit->value()->string()[0] == '.') {
                out_ << '0';
            }
            out_ << lit->value() << "\n";
        } else if (StringLiteral::Ptr lit = dynamic_cast<StringLiteral*>(init)) {
            out_ << " dq 0\n"; // Output the value of the literal label?
        } else {
            out_ << " dq 0\n";
        }
        assert(cons->label()->string()!="");
        definition_.insert(cons->label()->string());
    }

    // Output the generated code in nasm-format
    out_ << "section .text\n";
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }

    // Generate extern defs for each symbol that is referenced in this file,
    // but not defined in this file.
    for (std::set<std::string>::iterator i = symbol_.begin();
        i != symbol_.end(); i++) {

        std::set<std::string>::iterator q = definition_.find(*i);
        if (q == definition_.end() && (*i)[0] != '.') {
#if defined(DARWIN)
            out_ << "extern _" << *i << "\n";
#else
            out_ << "extern " << *i << "\n";
#endif
        }
    }

    out_->flush();
}

void Nasm64Generator::operator()(Class* feature) {
    // Emit the functions and vtable for the class specified by 'feature'
    class_ = feature;

    if (feature->destructor()->parent() == feature) {
        dispatch_table(feature);
    }

    // Now output the function defs
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }

    class_ = 0;
}

void Nasm64Generator::operator()(Function* feature) {
    // Emit the nasm assembly code for the given function.
    if (feature->is_virtual()) { return; }
    if (!feature->ir_blocks()) { return; }
    if (feature->is_native()) { return; }

    out_ << "section .text\n";
    out_ << "global "; label(Operand(feature->label())); out_ << "\n";
    label(Operand(feature->label())); out_ << ":\n";
    definition_.insert(feature->label()->string());
    out_ << "    push rbp\n";
    out_ << "    mov rbp, rsp\n";

    if (feature->stack_slots()) {
        // Allocate space on the stack; ensure that the stack is aligned to
        // a 16-byte boundary.
        int stack = feature->stack_slots() * machine_->word_size();
        if (stack % 16 != 0) {
            stack += 16 - (stack % 16);
        }
        out_ << "    sub rsp, " << stack << "\n";
    }

    for (int i = 0; i < feature->ir_blocks(); i++) {
        operator()(feature->ir_block(i));
    }
    // Make sure that the text section is gets re-aligned at the end of the
    // function, because the instruction stream can cause it be become
    // unaligned.
    align();
}

void Nasm64Generator::operator()(IrBlock* block) {
    // Translate a basic block in three-address code into x86.  For most
    // operations, this requires a "mov, op" sequence.

    IrBlock::Ptr branch = block->branch();
    IrBlock::Ptr next = block->next();
    if (block->label()) {
        out_ << block->label() << ":\n";
    }
    for (int i = 0; i < block->instrs(); i++) {
        Instruction const& inst = block->instr(i);
        Operand res = inst.result();
        Operand a1 = inst.first();
        Operand a2 = inst.second();
        opcode_ = inst.opcode();

        switch (inst.opcode()) {
        case BNE: bne(a1.reg(), a2.reg(), branch->label()); break;
        case BE: be(a1.reg(), a2.reg(), branch->label()); break;
        case BZ: bz(a1.reg(), branch->label()); break;
        case BNZ: bnz(a1.reg(), branch->label()); break;
        case BG: bg(a1.reg(), a2.reg(), branch->label()); break;
        case BGE: bge(a1.reg(), a2.reg(), branch->label()); break;
        case BL: bl(a1.reg(), a2.reg(), branch->label()); break;
        case BLE: ble(a1.reg(), a2.reg(), branch->label()); break;
        case CALL: instr("call", a1); break;
        case JUMP: instr("jmp", Operand(branch->label())); break;
        case MOV:
            if (res.is_float()) {
                instr("movsd", res, a1);
            } else {
                instr("mov", res, a1);
            }
            break;
        case ADD: arith(inst); break;
        case SUB: arith(inst); break;
        case MUL: arith(inst); break;
        case DIV: arith(inst); break;
        case NEG: neg(res, a1); break;
        case STORE: store_hack(a1, a2); break;
            // FIXME: Simplify the code path for labels, loads, stores,
            // literals, etc.
            // Convert literals to just labels with indirect flag set?
            // Disallow indirect operations on labels?
            // Investigate why leaq doesn't work
        case LOAD: load_hack(res, a1); break;
        case NOTB: instr("mov", res, a1); instr("not", res); break;
        case ANDB: instr("mov", res, a1); instr("and", res, a2); break;
        case ORB: instr("mov", res, a1); instr("or", res, a2); break;
        case NOP: instr("nop"); break;
        case RET: instr("leave"); instr("ret"); break;
        }
    }
}

void Nasm64Generator::dispatch_table(Class* feature) {
    // Output the class dispatch table for calling methods with dynamic
    // dispatch.  The format is as follows:
    //
    //     vtable[0] is the destructor
    //     vtable[1] is the hash function
    //     vtable[2] is the equals function
    //     vtable[3] is the number of 'slots' in the vtable (n)
    //     vtable[2..n+3] are the hash mixing values
    //     vtable[2n+2..2n+3] are the actual method pointers

    String* name = feature->label();
    Function* dtor = feature->destructor();
    std::string vtable = name->string()+"__vtable";

    // Output the vtable label and global directive
    out_ << "section .data\n";
    align();
    out_ << "global "; label(vtable); out_ << "\n";
    label(vtable); out_ << ":\n";
    definition_.insert(vtable);

    // Emit the destructor and vtable length
    out_ << "    dq "; label(Operand(dtor->label())); out_ << "\n";
    out_ << "    dq " << feature->jump1s() << "\n";

    // Emit the first jump table
    for (int i = 0; i < feature->jump1s(); i++) {
        out_ << "    dq " << feature->jump1(i) << "\n";
    }

    // Emit the second jump table
    for (int i = 0; i < feature->jump2s(); i++) {
        if (feature->jump2(i)) {
            out_ << "    dq ";
            label(Operand(feature->jump2(i)->label()));
            out_ << "\n";
        } else {
            out_ << "    dq 0\n";
        }
    }
    align();
}

void Nasm64Generator::neg(Operand res, Operand a1) {
    if (res.is_float()) {
        out_ << "    mov rax, 0\n";
        instr("cvtsi2sd", XMM0, RAX);
        instr("subsd", XMM0, a1);
        instr("movsd", res, XMM0);
    } else {
        instr("mov", res, a1);
        instr("neg", res);
    }
}

void Nasm64Generator::be(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
    } else {
        instr("cmp", a1, a2);
    }
    instr("je", Operand(label));
}

void Nasm64Generator::bne(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
    } else {
        instr("cmp", a1, a2);
    }
    instr("jne", Operand(label));
}

void Nasm64Generator::bz(RegisterId a1, String* label) {
    if (a1.is_float()) {
        instr("pxor", XMM0, XMM0);
        instr("comisd", a1, XMM0);
        // ptest
        instr("je", Operand(label));
    } else {
        instr("test", a1, a1);
        instr("jz", Operand(label));
    }
}

void Nasm64Generator::bnz(RegisterId a1, String* label) {
    if (a1.is_float()) {
        instr("pxor", XMM0, XMM0);
        instr("comisd", a1, XMM0);
        instr("jne", Operand(label));
    } else {
        instr("test", a1, a1);
        instr("jnz", Operand(label));
    }
}

void Nasm64Generator::bg(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
        instr("ja", Operand(label));
    } else {
        instr("cmp", a1, a2);
        instr("jg", Operand(label));
    }
}

void Nasm64Generator::bge(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
        instr("jae", Operand(label));
    } else {
        instr("cmp", a1, a2);
        instr("jge", Operand(label));
    }
}

void Nasm64Generator::bl(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
        instr("jb", Operand(label));
    } else {
        instr("cmp", a1, a2);
        instr("jl", Operand(label));
    }
}

void Nasm64Generator::ble(RegisterId a1, RegisterId a2, String* label) {
    if (a1.is_float()) {
        instr("comisd", a1, a2);
        instr("jbe", Operand(label));
    } else {
        instr("cmp", a1, a2);
        instr("jle", Operand(label));
    }
}

void Nasm64Generator::arith(Instruction const& inst) {
    // Emits an arithmetic instruction.  Depending on the opcode and the
    // operands, the expression may have to be manipulated because all x86
    // instructions take only 2 arithmetic operands.
    Operand res = inst.result();
    Operand r1 = inst.first();
    Operand r2 = inst.second();

    if (inst.opcode() == DIV && !res.is_float()) {
        out_ << "    push rdx\n";
        out_ << "    push rbx\n";
        out_ << "    mov rax, "; operand(r1); out_ << "\n";
        out_ << "    mov rbx, "; operand(r2); out_ << "\n";
        out_ << "    cqo\n";
        out_ << "    idiv rbx\n";
        out_ << "    pop rbx\n";
        out_ << "    pop rdx\n";
        out_ << "    mov "; operand(res); out_ << ", rax\n";
        return;
    }

    char const* name = 0;
    switch (inst.opcode()) {
    case ADD: name = (res.is_float() ? "addsd" : "add"); break;
    case SUB: name = (res.is_float() ? "subsd" : "sub"); break;
    case MUL: name = (res.is_float() ? "mulsd" : "imul"); break;
    case DIV: name = (res.is_float() ? "divsd" : "div"); break;
    default: assert(false);
    }

    if (res.reg() == r1.reg()) {
        // t1 <- t1 - t2
        instr(name, r1, r2);
    } else if (res.reg() != r2.reg()) {
        // t1 <- t2 - t3
        instr((res.is_float() ? "movsd" : "mov"), res, r1);
        instr(name, res, r2);
    } else if (inst.opcode() == ADD || inst.opcode() == MUL) {
        // t1 <- t2 + t1
        instr(name, r2, r1);
    } else if (inst.opcode() == SUB) {
        // t2 <- t1 - t2 goes to t2 <- -t2 + t1;
        if (res.is_float()) {
            instr("movsd", XMM0, r1);
            instr("subsd", XMM0, res);
            instr("movsd", res, XMM0);
        } else {
            instr("neg", r2);
            instr("add", r2, r1);
        }
    } else {
        // t1 <- t2 / t1
        assert(res.is_float());
        instr("movq", RAX, r1);
        instr(name, r1, res);
        instr("movsd", res, r1);
        instr("movq", r1, RAX);
    }
}

void Nasm64Generator::instr(const char* instr, Operand r1, Operand r2) {
    // Emits an instruction with two operands.
    out_ << "    " << instr << " ";
    operand(r1);
    out_ << ", ";
    operand(r2);
    out_ << "\n";
}

void Nasm64Generator::instr(const char* instr, Operand r1) {
    // Emits a single-operand instruction (either literal or register)
    out_ << "    " << instr << " ";
    operand(r1);
    out_ << "\n";
}

void Nasm64Generator::instr(const char* instr, Operand r1, const char* imm) {
    // Instruction that operates on a register r1 and an immediate value.
    out_ << "    " << instr << " ";
    reg(r1);
    out_ << ", " << imm << "\n";
}

void Nasm64Generator::instr(const char* instr) {
    // Emits a no-operand instruction.
    out_ << "    " << instr << "\n";
}

void Nasm64Generator::operand(Operand op) {
    // Emits any operand.  This function will automatically determine which
    // type of     operand to output.
    if (op.label()) {
        label(op);
    } else if (op.literal()) {
        literal(op);
    } else if (!!op.addr()) {
        addr(op);
    } else if (!!op.reg()) {
        reg(op);
    } else {
        assert(!"Nil operand");
    }
}

void Nasm64Generator::addr(Operand op) {
    // Emits the the address operand for a stack location as a an offset from
    // the base pointer (rbp).  No register need be specified in the operand,
    // as the register is understood to be rbp.  Likewise, the label and
    // literal fields should also be nil.
    assert(!op.literal());
    assert(!op.label());
    assert(!!op.addr());

    if (op.is_indirect()) {
        out_ << "[";
    }
    if (!!op.reg()) {
        out_ << machine_->reg(op.reg());
    } else {
        out_ << "rbp";
    }
    if (op.addr().value() > 0) {
        // Add +1 if loading from the base pointer, because the SP is stored
        // at location 0
        int addr = !!op.reg() ? op.addr().value() : op.addr().value()+1;
        out_ << "+" << addr * machine_->word_size();
    } else if (op.addr().value() < 0) {
        out_ << "-" << -op.addr().value() * machine_->word_size();
    }
    if (op.is_indirect()) {
        out_ << "]";
    }
}

void Nasm64Generator::reg(Operand op) {
    // Outputs a register, possibly with an address offset.  An operand passed
    // to this function should NOT have the literal or label fields set.
    assert(machine_->reg(op.reg()));
    assert(!op.literal());
    assert(!op.label());

    out_ << machine_->reg(op.reg());
}

void Nasm64Generator::literal(Operand literal) {
    // Outputs the correct representation of a literal.  If the literal is a
    // number, and the length is less than 32 bits, then output the literal
    // directly in the instruction.  Otherwise, load it from the correct memory
    // address for the literal.

    Expression* expr = literal.literal();
    assert(!literal.reg());
    assert(!literal.addr());
    assert(expr);

    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        out_ << "lit" << (void*)le->value();
    } else if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        out_ << le->value();
    } else if (NilLiteral* le = dynamic_cast<NilLiteral*>(expr)) {
        out_ << "0";
    } else if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
        uint64_t number = strtoull(le->value()->string().c_str(), 0, 0);
        if(number < INTEL64_MAX_IMM) {
            out_ << le->value();
        } else {
            out_ << "[lit" << (void*)le->value() << "]";
        }
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        out_ << "[lit" << (void*)le->value() << "]";
    }
}


void Nasm64Generator::label(Operand op) {
    // Emits a label, either as an operand or as an actual label at the
    // beginning of a code block.  No register/literal/addr fields should be
    // set on the operand.
    assert(!op.reg());
    assert(!op.literal());
    assert(!op.addr());
    assert(op.label());

    if (op.is_indirect()) {
        out_ << "[";
    }
    label(op.label()->string());
    if (op.is_indirect()) {
        out_ << "]";
    }
}

void Nasm64Generator::label(std::string const& label) {
    // Emits a label, either as an operand or as an actual label at the
    // beginning of a code block.
    std::string actual_label;
    if (label == env_->entry_point()) {
        actual_label = "main_";
    } else {
        actual_label = label;
    }

    if (actual_label[0] == '.') {
        out_ << actual_label;
    } else {
#if defined(DARWIN)
        out_ << "_" << actual_label;
#else
        out_ << actual_label;
#endif
    }
    symbol_.insert(label);
}

void Nasm64Generator::align() {
    out_ << "    align 8\n";
}

void Nasm64Generator::store_hack(Operand a1, Operand a2) {
    char const* mov = (a2.is_float() ? "movsd" : "mov qword");
    if (a1.label() && a1.is_indirect()) {
        out_ << "    " << mov << " rax, ";
        label(a1.label()->string());
        out_ << "\n";
        out_ << "    " << mov << " [rax], ";
        operand(a2);
        out_ << "\n";
    } else {
        instr(mov, a1, a2);
    }
}

void Nasm64Generator::load_hack(Operand res, Operand a1) {
    // FixMe: All these shenanigans are needed b/c NASM doesn't properly load
    // from a memory location to a 64-bit register.  For example, the following
    // works sometimes with "default rel" mode, but not always:
    //
    // mov xmm9, [addr]
    // mov rax, [addr]
    //
    // For now, this function works around this by inefficiently loading the
    // address directly into a register, and then using that register to
    // further load the literal:
    //
    // mov rax, addr
    // mov rax, [rax]

    char const* mov = (res.is_float() ? "movsd" : "mov qword");
    if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(a1.literal())) {
        uint64_t number = strtoull(le->value()->string().c_str(), 0, 0);
        if (number < INTEL64_MAX_IMM) {
            out_ << "    " << mov << " ";
            operand(res);
            out_ << ", " << le->value() << "\n";
        } else {
            out_ << "    " << mov << " ";
            operand(res);
            out_ << ", lit" << (void*)le->value() << "\n";
            out_ << "    " << mov << " ";
            operand(res);
            out_ << ", [";
            operand(res);
            out_ << "]\n";
        }
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(a1.literal())) {
        out_ << "    push rax\n";
        out_ << "    mov rax, lit" << (void*)le->value() << "\n";
        out_ << "    movsd ";
        operand(res);
        out_ << ", [rax]\n";
        out_ << "    pop rax\n";
    } else if (a1.label() && a1.is_indirect()) {
        out_ << "    mov rax, ";
        label(a1.label()->string()); // Load the label
        out_ << "\n";
        out_ << "    " << mov << " ";
        operand(res);
        out_ << ", [rax]\n"; // Now load the value of the label
    } else if (!!a1.addr() && !a1.is_indirect()) {
        // Load effective address.  An IR instruction like rax <- load rbx+3
        // gets translated to an LEAQ instruction
        out_ << "    lea "; operand(res); out_ << ", [";
        operand(a1); out_ << "]\n";
    } else {
        instr(mov, res, a1);
    }
}

void Nasm64Generator::string(String* string) {
    // Output a string literal, and correctly process escape sequences.
    std::string const& in = string->unescaped();
    std::string out = "`";
    for (int i = 0; i < in.length(); i++) {
        char buf[512];
        sprintf(buf, "\\x%02x", (uint8_t)in[i]);
        out += buf;
    }
    out += "\\x00`";
    out_ << "lit" << (void*)string << ": \n";
    out_ << "    dq ";
    label("String__vtable");
    out_ << "\n"; // vtable
    out_ << "    dq 0xf000000000000001\n";
    // reference count + readonly mask
    out_ << "    dq " << (int)in.length() << "\n";
    out_ << "    db " << out << "\n";
    align();
}
