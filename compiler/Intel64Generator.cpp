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

RegisterId const Intel64Generator::RAX(1, 0);
RegisterId const Intel64Generator::RSP(5, 0);
RegisterId const Intel64Generator::RBP(6, 0);

#ifdef WINDOWS
#define atoll _atoi64
#endif

Intel64Generator::Intel64Generator(Environment* env) :
    env_(env),
    machine_(Machine::intel64()) {
}

void Intel64Generator::format(OutputFormat::Ptr format) { 
    format_ = format; 
    text_ = format->text();
    data_ = format->data();
}

void Intel64Generator::operator()(File* file) {
    // Output machine code for a single translation unit.
    if (env_->errors()) { return; }

    // FIXME: Output constants
    // FIXME: Output integer literals?
    // FIXME: Output strings
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }

    for (Constant::Itr cons = file->constants(); cons; ++cons) {
        if (cons->type()->is_value() && !cons->type()->is_primitive()) {
            assert(!"Not supported");
        }
        format_->sym(cons->label(), OutputFormat::SYM_TEXT);
        text_->uint64(0); // Data
    }

    for (std::set<String*>::iterator i = string_.begin(); 
        i != string_.end(); ++i) {
        string(*i);  
    }
    string_.clear();

    text_->align(machine_->word_size(), NOP);
    data_->align(machine_->word_size());
    format_->out(out_);
}

void Intel64Generator::operator()(Class* feature) {

    // Emit dispatch table
    if (feature->destructor()->parent() == feature) {
        dispatch_table(feature);
    }

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
    if (feature->label()->string() == env_->entry_point()) {
        format_->sym(env_->name("main_"), OutputFormat::SYM_TEXT);
    } else if (feature->label()->string() == "Boot_main") {
        format_->sym(env_->name("main"), OutputFormat::SYM_TEXT);
    } else {
        format_->sym(feature->label(), OutputFormat::SYM_TEXT); 
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
        format_->sym(block->label(), OutputFormat::SYM_LTEXT);
    }
    for (int i = 0; i < block->instrs(); i++) {
        Instruction const& inst = block->instr(i);
        Operand res = inst.result();
        Operand a1 = inst.first();
        Operand a2 = inst.second();
    
        if (LOAD != inst.opcode() && STORE != inst.opcode()) {
            assert("Memory operand not supported"&&!a1.is_indirect());
            assert("Memory operand not supported"&&!a2.is_indirect());
            assert("Memory operand not supported"&&!res.is_indirect());
            assert("Literal operand not supported"&&!a1.literal());
            assert("Literal operand not supported"&&!a2.literal());
            assert("Literal operand not supported"&&!res.literal());
        } 

        switch (inst.opcode()) {
        case RET: leave(); ret(); break;
        case MOV: mov(res.reg(), a1.reg()); break;
        case LOAD: load(res.reg(), a1); break;
        case STORE: store(a1, a2); break;
        case BNE: cmp(a1.reg(), a2.reg()); jne(branch->label()); break;
        case BE: cmp(a1.reg(), a2.reg()); je(branch->label()); break;
        case BZ: test(a1.reg(), a1.reg()); jz(branch->label()); break;
        case BNZ: test(a1.reg(), a1.reg()); jnz(branch->label()); break; 
        case BG: cmp(a1.reg(), a2.reg()); jg(branch->label()); break;
        case BGE: cmp(a1.reg(), a2.reg()); jge(branch->label()); break;
        case BL: cmp(a1.reg(), a2.reg()); jl(branch->label()); break;
        case BLE: cmp(a1.reg(), a2.reg()); jle(branch->label()); break;
        case CALL: call(a1); break;
        case JUMP: jmp(branch->label()); break;
        case ADD: arith(inst); break;
        case SUB: arith(inst); break;
        case MUL: arith(inst); break;
        case DIV: arith(inst); break;
        case NEG: mov(res.reg(), a1.reg()); neg(res.reg()); break;
        case NOTB: mov(res.reg(), a1.reg()); bnot(res.reg()); break;
        case ANDB: mov(res.reg(), a1.reg()); band(res.reg(), a2.reg()); break;
        case ORB: mov(res.reg(), a1.reg()); bor(res.reg(), a2.reg()); break; 
        case NOP: text_->uint8(0x90); break;
        default: break;
        }
    }
}

void Intel64Generator::dispatch_table(Class* feature) {
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

    // Output the vtable label 
    data_->align(machine_->word_size());
    format_->sym(env_->name(vtable), OutputFormat::SYM_DATA);

    // Emit the destructor and vtable length
    format_->ref(dtor->label(), OutputFormat::REF_DATA);
    data_->uint64(0);
    data_->uint64(feature->jump1s());

    // Emit the first jump table
    for (int i = 0; i < feature->jump1s(); i++) {
        data_->uint64(feature->jump1(i));
    } 
    
    // Emit the second jump table
    for (int i = 0; i < feature->jump2s(); i++) {
        if (feature->jump2(i)) {
            String* label = feature->jump2(i)->label();
            format_->ref(label, OutputFormat::REF_DATA);
            data_->uint64(0);
        } else {
            data_->uint64(0);
        }
    }
}

bool Intel64Generator::is_extended_reg(RegisterId reg) const {
    return ((reg.id() - 1) >= 8);
}

void Intel64Generator::string(String::Ptr str) {
    // Outputs a string literal definition.
    static uint64_t const READONLY_MASK = 0xf000000000000000;
    std::string const label = "lit"+stringify((void*)str);
    std::string const out = str->unescaped();
    String::Ptr const vtable = env_->name("String__vtable");
    data_->align(machine_->word_size());
    format_->sym(env_->name(label), OutputFormat::SYM_LDATA);
    format_->ref(vtable, OutputFormat::REF_DATA);
    data_->uint64(0); // Placeholder for vtable ref
    data_->uint64(1 | READONLY_MASK); // Reference count
    data_->uint64(out.length()); // Length
    data_->buffer(out.c_str(), out.length()+1);
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

void Intel64Generator::instr(uint8_t op, uint8_t ext, RegisterId rm) {
    // Emits a single-operand instruction w/ extension field
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
    // the second operand.  DO NOT use this for branch instructions, as it
    // assumes 'label' is in the data section!
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
    format_->ref(label, OutputFormat::REF_SIGNED);
    text_->uint32(0); // Immediate
}

void Intel64Generator::instr(uint8_t op, RegisterId reg, Operand mem) {
    // Emit an instruction that uses a memory operand specified by register,
    // with a possible offset. 
    RegisterId rm = mem.reg();
    Address disp = mem.addr();
    assert("Not an indirect operand" && (mem.is_indirect() || op == LEA));
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
    int32_t offset = disp.value() * machine_->word_size();
    if (!rm.id() && offset > 0) {
        offset += machine_->word_size();
        // Add +1*word_size if loading from above the implied base pointer, b/c
        // the SP is stored at offset 0 from the base pointer.  The IR assumes
        // that arguments start at 0, not 1*word_size.  Offsets below the BP
        // are treated normally.
    }
    uint8_t modrm = 0;
    // Select the mod field.   Note that r13 is a special case: if mod is 00,
    // then the address loaded is [RIP+disp32] rather than [R13] as one might
    // expect.
    int const r13 = 13;
    // Special case: R13 cannot use MODRM_INDIRECT, b/c with that encoding RIP
    // is used as the indirect register memory operand instead of R13.
    if (offset == 0 && rmid != r13) {
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

    int const rsp = 4;
    // Special case: RSP requires an SIB byte in indirect addressing mode.
    if (rmid == rsp) {
        uint8_t const rsp_sib = 0x24;
        // scale = 00, index = 100 (rsp), base = 100 (rsp) -> 0x24
        text_->uint8(rsp_sib);
    }

    if (offset == 0 && rmid != r13) {
        // No displacement
    } else if (offset > INT8_MAX || offset < INT8_MIN) {
        text_->uint32(offset);
    } else {
        text_->uint8(offset);
    }
    // FixMe: If using MODRM w/ R12, then SIB must be specified.  For now, the
    // R12 register is disabled -- the register allocator will not color
    // temporaries to that register.
}

void Intel64Generator::load(RegisterId res, Operand a1) {
    // Loads the literal or a register-indirect memory location specified into
    // a register
    assert(!!res);
    Expression* expr = a1.literal();
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        uint8_t const regid = res.id() - 1;
        uint8_t const op = MOV_IMM + (MODRM_RM & regid);
        uint8_t rex = REX_PREFIX|REX_W;
        if (is_extended_reg(res)) {
            rex |= REX_B;
        }
        text_->uint8(rex);
        text_->uint8(op);
        String::Ptr const label = env_->name("lit"+stringify((void*)le->value()));
        format_->ref(label, OutputFormat::REF_TEXT);
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
        assert(!"Not implemented");
    } else if (a1.label() && a1.is_indirect()) {
        mov(res, a1.label());
    } else if (a1.label()) {
        uint8_t const regid = res.id() - 1;
        uint8_t const op = MOV_IMM + (MODRM_RM & regid);
        uint8_t rex = REX_PREFIX|REX_W;
        if (is_extended_reg(res)) {
            rex |= REX_B;
        }
        text_->uint8(rex);
        text_->uint8(op);
        format_->ref(a1.label(), OutputFormat::REF_TEXT);
        // Ref the literal in the relocation table
        text_->uint64(0); // Immediate
    } else if (!!a1.addr() && !a1.is_indirect()) {
        lea(res, a1);
    } else {
        mov(res, a1);
    }
}

void Intel64Generator::store(Operand a1, Operand a2) {
    Expression* expr = a2.literal();
    assert("Direct store operand"&&a1.is_indirect());
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        assert(!"Not implemented");
    } else if (BooleanLiteral* le = dynamic_cast<BooleanLiteral*>(expr)) {
        mov(RAX, le->value()->string()!="0");
        mov(a1, RAX);
    } else if (NilLiteral* le = dynamic_cast<NilLiteral*>(expr)) {
        mov(RAX, (uint64_t)0);
        mov(a1, RAX);
    } else if (IntegerLiteral* le = dynamic_cast<IntegerLiteral*>(expr)) {
        mov(RAX, atoll(le->value()->string().c_str()));
        mov(a1, RAX);
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        assert(!"Not implemented");
    } else if (a1.label()) {
        assert("Direct label store"&&a1.is_indirect());
        assert("Offset in label store"&&!a1.addr().value());
        mov(a1.label(), a2.reg());
    } else {
        mov(a1, a2.reg());
    }
}

void Intel64Generator::arith(Instruction const& inst) {
    // Emits an arithmetic instruction.  Depending on the opcode and the
    // operands, the expression may have to be manipulated because all x86
    // instructions take only 2 arithmetic operands.
    Operand res = inst.result();
    Operand r1 = inst.first();
    Operand r2 = inst.second();
    
    assert(!res.is_float()&&"Not implemented");
    if (inst.opcode() == DIV && !res.is_float()) {
        assert(!"Not implemented");
/*
        out_ << "    push rdx\n";
        out_ << "    mov rax, "; operand(r1); out_ << "\n"; 
        out_ << "    cqo\n";
        out_ << "    idiv "; operand(r2); out_ << "\n";
        out_ << "    pop rdx\n";
        out_ << "    mov "; operand(res); out_ << ", rax\n";
*/
        return;
    }

    uint8_t op = 0;
    switch (inst.opcode()) {
    case ADD: op = 0x03; break;
    case SUB: op = 0x2b; break;
    //case MUL: name = (res.is_float() ? "mulsd" : "imul"); break;
    //case DIV: name = (res.is_float() ? "divsd" : "div"); break;
    default: assert(false);
    }

    // r10 <- rbx - rdi

    // mov r10, rbx
    // sub r10, rdi 

    if (res.reg() == r1.reg()) {
        // t1 <- t1 - t2
        instr(op, r1.reg(), r2.reg());
    } else if (res.reg() != r2.reg()) {
        // t1 <- t2 - t3
        mov(res.reg(), r1.reg()); 
        instr(op, res.reg(), r2.reg());
    } else if (inst.opcode() == ADD || inst.opcode() == MUL) {
        // t1 <- t2 + t1
        instr(op, r2.reg(), r1.reg()); 
    } else if (inst.opcode() == SUB) {
        // t1 <- t2 - t1 goes to t1 <- -t1 + t2; t1 <- -t1 
        neg(r1.reg());
        add(r1.reg(), r2.reg());
    } else {
        // t1 <- t2 / t1 
        //instr("push", r2);
        //instr(name, r2, r1);
        //instr((res.is_float() ? "movsd" : "mov"), r1, r2);
        //instr("pop", r2);        
        assert(!"Not implemented");
    }
}

void Intel64Generator::neg(RegisterId reg) {
    instr(0xf7, 0x03, reg);
}

void Intel64Generator::bnot(RegisterId reg) {
    instr(0xf7, 0x02, reg);
}

void Intel64Generator::band(RegisterId dst, RegisterId src) {
    instr(0x23, dst, src);
}

void Intel64Generator::bor(RegisterId dst, RegisterId src) {
    instr(0x0b, dst, src);
}

void Intel64Generator::mov(RegisterId dst, RegisterId src) {
    // Emits a register-register or move instruction.
    instr(MOV_REG_RM, dst, src);
}

void Intel64Generator::mov(RegisterId reg, Operand rm) {
    // Moves a value from memory into 'reg', using 'rm' as the base.
    // FIXME: This logic below is necessary to support an IR instruction like
    // rax <- rbx+4, which translates to an LEA.  If support for that IR
    // instruction is removed, then this logic is not necessary.  Alternatively,
    // this behavior could be made explicit by adding an LEA instruction.
    instr(MOV_REG_RM, reg, rm);
/*
    if (!rm.addr()) {
        mov(reg, rm.reg());
    } else if(rm.is_indirect()) {
        instr(MOV_REG_RM, reg, rm);
    } else {
        instr(LEA, reg, rm);
    }
*/
}

void Intel64Generator::mov(Operand rm, RegisterId reg) {
    // Moves a value to memory from 'reg', using 'rm' as the base.
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
    uint8_t rex = 0;
    if (imm > UINT32_MAX) {
        rex |= REX_W;
    }
    if (is_extended_reg(reg)) { 
        rex |= REX_B;
    }
    if (rex) {
        text_->uint8(REX_PREFIX|rex);
    }
    text_->uint8(op);
    if (imm > UINT32_MAX) {
        text_->uint64(imm);
    } else {
        text_->uint32(imm);
    }
}

void Intel64Generator::lea(RegisterId reg, Operand rm) {
    instr(LEA, reg, rm);
} 

void Intel64Generator::sub(RegisterId dst, uint64_t imm) {
    instr(0x81, 0x05, dst, imm);
}

void Intel64Generator::call(Operand target) {
    // Emits a call-label or call-register instruction with target 'target'
    String* label = target.label();
    if (label) {
        text_->uint8(0xe8);
        format_->ref(label, OutputFormat::REF_BRANCH);
        text_->uint32(0); // Displacement
    } else {
        assert("Missing call target register" && !!target.reg());
        instr(0xff, 0x02, target.reg());
    }    
}

void Intel64Generator::jmp(String* label) {
    text_->uint8(0xe9);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0); // Displacement
}

void Intel64Generator::jne(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x85);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::je(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x84);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jg(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x8f);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jge(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x8d);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jl(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x8c);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jle(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x8e);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jz(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x84);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::jnz(String* label) {
    text_->uint8(0x0f);
    text_->uint8(0x85);
    format_->ref(label, OutputFormat::REF_BRANCH);
    text_->uint32(0);
}

void Intel64Generator::push(RegisterId reg) {
    assert(reg.id() >= 1 && reg.id() <= 16);
    uint8_t const regid = reg.id() - 1;
    text_->uint8(0x50+regid);
}

void Intel64Generator::cmp(RegisterId arg1, RegisterId arg2) {
    instr(0x3b, arg1, arg2);  
}

void Intel64Generator::test(RegisterId arg1, RegisterId arg2) {
    instr(0x85, arg1, arg2);  
}

void Intel64Generator::add(RegisterId arg1, RegisterId arg2) {
    instr(0x01, arg1, arg2);
}

void Intel64Generator::leave() {
    text_->uint8(0xc9);
}

void Intel64Generator::ret() {
    text_->uint8(0xc3); 
}
