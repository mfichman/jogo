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

#include "Intel64Generator.hpp"

RegisterId const Intel64Generator::RSP(5, 0);
RegisterId const Intel64Generator::RBP(6, 0);

Intel64Generator::Intel64Generator(Environment* env) :
    env_(env),
    machine_(Machine::intel64()) {
}

void Intel64Generator::format(OutputFormat::Ptr format) { 
    format_ = format; 
    text_ = format->text();
}

void Intel64Generator::operator()(File* file) {
    // Output machine code for a single translation unit.
    if (env_->errors()) { return; }

    // FIXME: Output constants
    // FIXME: Output integer literals?
    // FIXME: Output strings
    //for (String::Ptr s = env_->strings(); s; s = s->next()) {
    //    string(s);
    //}

    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this);
    }

    text_->align(16);
    
    for (std::set<String*>::iterator i = string_.begin(); 
        i != string_.end(); ++i) {
        string(*i);  
    }
    string_.clear();

    format_->out(out_);
}

void Intel64Generator::operator()(Class* feature) {

    // Emit dispatch table

    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void Intel64Generator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void Intel64Generator::operator()(Function* feature) {
    // Emit the machine code for a function.
    if (feature->is_virtual()) { return; }
    if (!feature->ir_blocks()) { return; }
    if (feature->is_native()) { return; }
    function_ = feature;

    // stack_check
    text_->align(16);
    if (feature->label()->string() == env_->entry_point()) {
        format_->label(env_->name("main_"));
    } else if (feature->label()->string() == "Boot_main") {
        format_->label(env_->name("main"));
    } else {
        format_->label(feature->label()); 
    }

    push(RBP);
    mov(RBP, RSP);

    if (feature->stack_slots()) {
        // Allocate space on the stack; ensure that the stack is aligned to a
        // 16-byte boundary.
        int stack = feature->stack_slots() * machine_->word_size();
        if (stack % 16 != 0) {
            stack += 16 - (stack % 16);
        }
        sub(RSP, stack);
    }
    for (int i = 0; i < feature->ir_blocks(); i++) {
        operator()(feature->ir_block(i));
    } 
    function_ = 0;
}

void Intel64Generator::operator()(IrBlock* block) {
    // Translate a basic block in three-address code into x86.  For most
    // operations, this requires a "mov, op" sequence.  
    IrBlock::Ptr branch = block->branch();
    IrBlock::Ptr next = block->next();
    if (block->label()) {
        format_->local(block->label());
    }
    for (int i = 0; i < block->instrs(); i++) {
        Instruction const& inst = block->instr(i);
        Operand res = inst.result();
        Operand a1 = inst.first();
        Operand a2 = inst.second();

        switch (inst.opcode()) {
        case RET: leave(); ret(); break;
        case MOV: mov(res.reg(), a1.reg()); break;
        case LOAD: load(res.reg(), a1); break;
        case STORE: store(a1, a2.reg()); break;
//        case BNE: cmp(a1, a2); jne(branch->label(); break;
//        case BE: instr("cmp", a1, a2); instr("je", branch->label()); break;
//        case BZ: instr("cmp", a1, "0"); instr("je", branch->label()); break;
//        case BNZ: instr("cmp", a1, "0"); instr("jne", branch->label()); break; 
//        case BG: instr("cmp", a1, a2); instr("jg", branch->label()); break;
//        case BGE: instr("cmp", a1, a2); instr("jge", branch->label()); break;
//        case BL: instr("cmp", a1, a2); instr("jl", branch->label()); break;
//        case BLE: instr("cmp", a1, a2); instr("jle", branch->label()); break;
        case CALL: call(a1); break;
//        case JUMP: instr("jmp", branch->label()); break;
//        case MOV: 
//            if (!!a1.addr() && !a1.is_indirect()) {
//                out_ << "    lea "; operand(res); out_ << ", ["; 
//                operand(a1); out_ << "]\n";
//            } else if (res.is_float()) {
//                instr("movsd", res, a1);
//            } else {
//                instr("mov", res, a1); 
//            }
//            break;
//        case ADD: arith(inst); break;
//        case SUB: arith(inst); break;
//        case MUL: arith(inst); break;
//        case DIV: arith(inst); break;
//        case NEG: instr("mov", res, a1); instr("neg", res); break;
//        case STORE: store_hack(a1, a2); break;
//            // FIXME: Simplify the code path for labels, loads, stores,
//            // literals, etc.
//            // Convert literals to just labels with indirect flag set?
//            // Disallow indirect operations on labels?
//            // Investigate why leaq doesn't work
//        case NOTB: instr("mov", res, a1); instr("not", res); break;
//        case ANDB: instr("mov", res, a1); instr("and", res, a2); break;
//        case ORB: instr("mov", res, a1); instr("or", res, a2); break; 
//        case NOP: instr("nop"); break;
//        case RET: instr("leave"); instr("ret"); break;
        default: break;
        }
    }
}

bool Intel64Generator::is_extended_reg(RegisterId reg) const {
    return ((reg.id() - 1) >= 8);
}

void Intel64Generator::string(String::Ptr str) {
    // Outputs a string literal definition.
    std::string const label = "lit"+stringify((void*)str);
    std::string const out = str->unescaped();
    format_->local(env_->name(label));
    format_->ref(env_->name("String__vtable"), OutputFormat::RELOC_ABSOLUTE); 
    text_->uint64(0); // Placeholder for vtable ref
    text_->uint64(1); // Reference count
    text_->uint64(out.length()); // Length
    text_->buffer(out.c_str(), out.length()+1);
}

void Intel64Generator::instr(uint8_t op, uint8_t ext, RegisterId rm, uint32_t imm) {
    // Emits a register-immediate (32-bit immediate) instruction
    assert(!!rm&&"Invalid register ID");
    uint8_t const rmid = rm.id() - 1;
    uint8_t rex = REX_PREFIX|REX_W;
    if (is_extended_reg(rm)) {
        rex |= REX_B;
    }
    uint8_t modrm = MODRM_DIRECT;
    modrm |= (MODRM_REG & (ext << 3));
    modrm |= (MODRM_RM & (rmid));
    text_->uint8(rex);
    text_->uint8(op);
    text_->uint8(modrm);
    text_->uint32(imm);
}

void Intel64Generator::instr(uint8_t op, RegisterId reg, RegisterId rm) {
    // Emits a simple register-register instruction 
    // dst = reg, src = rm
    assert(!!reg&&"Invalid register ID");
    assert(!!rm&&"Invalid register ID");
    uint8_t const regid = reg.id() - 1;
    uint8_t const rmid = rm.id() - 1;
    uint8_t rex = REX_PREFIX|REX_W;
    if (is_extended_reg(reg)) {
        rex |= REX_R; 
    }
    if (is_extended_reg(rm)) {
        rex |= REX_B; 
    }
    uint8_t modrm = MODRM_DIRECT;
    modrm |= (MODRM_REG & (regid << 3));
    modrm |= (MODRM_RM & (rmid));
    text_->uint8(rex);
    text_->uint8(op);
    text_->uint8(modrm);
}

void Intel64Generator::instr(uint8_t op, RegisterId reg, String* label) {
    // Emits an instruction that uses the memory value at the given label as 
    // the second operand.
    uint8_t const regid = reg.id() - 1;
    uint8_t const rmid = RBP.id() - 1;
    // RBP = use rip-relative addressing to load the operand from a PC-relative
    // address.
    uint8_t rex = REX_PREFIX|REX_W;
    if (is_extended_reg(reg)) {
        rex |= REX_R;
    }
    uint8_t modrm = MODRM_INDIRECT;
    modrm |= (MODRM_REG & (regid << 3));
    modrm |= (MODRM_RM & rmid);
    text_->uint8(rex);
    text_->uint8(op);
    text_->uint8(modrm);
    format_->ref(label, OutputFormat::RELOC_SIGNED);
    text_->uint32(0); // Immediate
}

void Intel64Generator::instr(uint8_t op, RegisterId reg, Operand mem) {
    // Emit an instruction that uses a memory operand specified by register,
    // with a possible offset. 
    RegisterId rm = mem.reg();
    Address disp = mem.addr();
    assert("Not an indirect operand"&&mem.is_indirect());
    assert(reg.id() >= 1 && reg.id() <= 16);
    uint8_t const regid = reg.id() - 1;
    uint8_t const rmid = (rm.id() ? rm.id() : RBP.id()) - 1;
    uint8_t rex = REX_PREFIX|REX_W;
    if (is_extended_reg(reg)) {
        rex |= REX_R; 
    }
    if (is_extended_reg(rm)) {
        rex |= REX_B; 
    }
    int32_t const offset = disp.value() * machine_->word_size();
    uint8_t modrm = 0;
    if (offset == 0) {
        modrm = MODRM_INDIRECT;
    } else if (offset > INT8_MAX || offset < INT8_MIN) {
        modrm = MODRM_DISP32;
    } else {
        modrm = MODRM_DISP8;
    }
    modrm |= (MODRM_REG & (regid << 3));
    modrm |= (MODRM_RM & (rmid));
    text_->uint8(rex);
    text_->uint8(op);
    text_->uint8(modrm);
    if (offset == 0) {
        // No displacement
    } else if (offset > INT8_MAX || offset < INT8_MIN) {
        text_->uint32(offset);
    } else {
        text_->uint8(offset);
    }
}

void Intel64Generator::load(RegisterId res, Operand a1) {
    // Loads the literal or a register-indirect memory location specified into
    // a register
    assert(!!res);
    Expression* expr = a1.literal();
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        uint8_t const regid = res.id() - 1;
        uint8_t const op = MOV_IMM + regid;
        uint8_t rex = REX_PREFIX|REX_W;
        if (is_extended_reg(res)) {
            rex |= REX_B;
        }
        text_->uint8(rex);
        text_->uint8(op);
        std::string const label = "lit"+stringify((void*)le->value());
        format_->ref(env_->name(label), OutputFormat::RELOC_ABSOLUTE);
        // Ref the literal in the relocation table
        text_->uint64(0); // Immediate
        string_.insert(le->value());
    } else if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        mov(res, le->value()->string()!="0");
    } else if (NilLiteral* le = dynamic_cast<NilLiteral*>(expr)) {
        mov(res, (uint64_t)0);
    } else if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
        mov(res, atoll(le->value()->string().c_str()));
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        //assert(!"Not implemented");
    } else if (a1.label()) {
        assert("Dndirect label load"&&a1.is_indirect());
        assert("Offset in label load"&&!a1.addr().value());
        mov(res, a1.label());
    } else {
        mov(res, a1);
    }
}

void Intel64Generator::store(Operand a1, RegisterId a2) {
    //
    if (a1.label()) {
        assert("Direct label store"&&a1.is_indirect());
        assert("Offset in label store"&&!a1.addr().value());
        mov(a1.label(), a2);
    } else {
        mov(a1, a2);
    }
}

void Intel64Generator::mov(RegisterId dst, RegisterId src) {
    // Emits a register-register or move instruction.
    instr(MOV_REG_RM, dst, src);
}

void Intel64Generator::mov(RegisterId reg, Operand rm) {
    // Moves a value from memory into 'reg', using 'rm' as the base, and 'disp'
    // as an offset from the base.
    instr(MOV_REG_RM, reg, rm);
}

void Intel64Generator::mov(Operand rm, RegisterId reg) {
    // Moves a value to memory from 'reg', using 'rm' as the base, and 'disp'
    // as an offset from the base.
    instr(MOV_RM_REG, reg, rm);
}

void Intel64Generator::mov(RegisterId reg, String* label) {
    // Emits an instruction to move the memory value stored at the label into
    // the given register.
    instr(MOV_REG_RM, reg, label);
}

void Intel64Generator::mov(String* label, RegisterId reg) {
    // Emits an instruction to move a register to the memory location indicated
    // by the label.
    instr(MOV_RM_REG, reg, label);
}

void Intel64Generator::mov(RegisterId reg, uint64_t imm) {
    // Emits a register-immediate move instruction with a 64-bit immediate
    // value.  If the immediate is smaller than 32 bits, use a 32-bit
    // intruction instead.
    uint8_t const regid = reg.id() - 1;
    uint8_t const op = MOV_IMM + (MODRM_RM & regid);
    uint8_t rex = REX_PREFIX;
    if (imm > UINT32_MAX) {
        rex |= REX_W;
    }
    if (is_extended_reg(reg)) { 
        rex |= REX_B;
    }
    text_->uint8(rex);
    text_->uint8(op);
    if (imm > UINT32_MAX) {
        text_->uint64(imm);
    } else {
        text_->uint32(imm);
    }
}

void Intel64Generator::sub(RegisterId dst, uint64_t imm) {
    instr(0x81, 0x05, dst, imm);
}

void Intel64Generator::call(Operand a1) {
    text_->uint8(0xe8);
    format_->ref(a1.label(), OutputFormat::RELOC_BRANCH);
    text_->uint32(0); // Displacement
}

void Intel64Generator::push(RegisterId reg) {
    assert(reg.id() >= 1 && reg.id() <= 16);
    uint8_t const regid = reg.id() - 1;
    text_->uint8(0x50+regid);
}

void Intel64Generator::leave() {
    text_->uint8(0xc9);
}

void Intel64Generator::ret() {
    text_->uint8(0xc3); 
}
