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

#define __STDC_LIMIT_MACROS
#include "Intel64Generator.hpp"
#include <cstdlib>
#include <stdint.h>

#ifdef WINDOWS
#define strtoull _strtoui64
#endif

Machine::Ptr const Intel64Generator::MACHINE = Machine::intel64();
RegisterId const Intel64Generator::RAX(MACHINE->reg("rax")->id());
RegisterId const Intel64Generator::RSP(MACHINE->reg("rsp")->id());
RegisterId const Intel64Generator::RBP(MACHINE->reg("rbp")->id());
RegisterId const Intel64Generator::RBX(MACHINE->reg("rbx")->id());
RegisterId const Intel64Generator::RDX(MACHINE->reg("rdx")->id());
RegisterId const Intel64Generator::R13(MACHINE->reg("r13")->id());
RegisterId const Intel64Generator::R15(MACHINE->reg("r15")->id());
RegisterId const Intel64Generator::XMM0(MACHINE->reg("xmm0")->id());
RegisterId const Intel64Generator::XMM15(MACHINE->reg("xmm15")->id());

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
    format_->sym(env_->name("__file"), OutputFormat::SYM_LDATA);

    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }

    for (Constant::Itr cons = file->constants(); cons; ++cons) {
        if (cons->type()->is_value() && !cons->type()->is_primitive()) {
            assert(!"Not supported");
        }
        format_->sym(cons->label(), OutputFormat::SYM_DATA);
        Expression* init = cons->initializer();
        if (Construct* constr = dynamic_cast<Construct*>(init)) {
            init = constr->arguments();
            // Support for primitive constructors, e.g., Char(0xf).  FIXME:
            // Should use constant folding instead, to allow all constant
            // expressions.
        } 
        if (IntegerLiteral::Ptr lit = dynamic_cast<IntegerLiteral*>(init)) {
            data_->uint64(literal(lit)); 
        } else if (FloatLiteral::Ptr lit = dynamic_cast<FloatLiteral*>(init)) {
            data_->uint64(literal(lit));
        } else if (StringLiteral::Ptr lit = dynamic_cast<StringLiteral*>(init)) {
            // FixMe: Declare equal to the corresponding string literal symbol
            data_->uint64(0); // Data
        } else {
            data_->uint64(0); // Data
        }
    }

    for (std::set<String*>::iterator i = string_.begin(); 
        i != string_.end(); ++i) {
        string(*i);  
    }
    string_.clear();

    text_->align(machine_->word_size(), XNOP);
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
        case MOV: 
            if (res.is_float()) { 
                movsd(res.reg(), a1.reg()); 
            } else {
                mov(res.reg(), a1.reg()); 
            }
            break;
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
        case ADD: add(res.reg(), a1.reg(), a2.reg()); break;
        case SUB: sub(res.reg(), a1.reg(), a2.reg()); break;
        case MUL: mul(res.reg(), a1.reg(), a2.reg()); break;
        case DIV: div(res.reg(), a1.reg(), a2.reg()); break;
        case NEG: neg(res.reg(), a1.reg()); break;
        case NOTB: mov(res.reg(), a1.reg()); bnot(res.reg()); break;
        case ANDB: mov(res.reg(), a1.reg()); band(res.reg(), a2.reg()); break;
        case ORB: mov(res.reg(), a1.reg()); bor(res.reg(), a2.reg()); break; 
        case NOP: break;
        default: break;
        }
    }
}

uint64_t Intel64Generator::literal(FloatLiteral* lit) {
    // FIXME: This function assumes that the architecure the compiler is run on
    // is Intel 64.  1GThis is not necessarily the case.  If the architecure has
    // a binary format that is different from the output architecure, then this
    // function will not work!
    assert("Bad architecture" && sizeof(double) == sizeof(uint64_t));
    double immf = atof(lit->value()->string().c_str());
    uint64_t imm = 0;
    memcpy(&imm, &immf, sizeof(imm));
    return imm;
}

uint64_t Intel64Generator::literal(IntegerLiteral* lit) {
    // FIXME: This function assumes that the architecure the compiler is run on
    // is Intel 64.  This is not necessarily the case.  If the architecure has
    // a binary format that is different from the output architecure, then this
    // function will not work!
    return strtoull(lit->value()->string().c_str(), 0, 0);
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
    return reg_code(reg) >= 8;
}

bool Intel64Generator::is_sse_reg(RegisterId reg) const {
    return reg.is_float() && reg.id() >= XMM0.id() && reg.id() <= XMM15.id();
}

bool Intel64Generator::is_gp_reg(RegisterId reg) const {
    return !reg.is_float() && reg.id() >= RAX.id() && reg.id() <= R15.id();
}

uint8_t Intel64Generator::reg_code(RegisterId id) const {
    if (is_gp_reg(id)) {
        return id.id() - RAX.id();
    } else if (is_sse_reg(id)) {
        return id.id() - XMM0.id();
    } else {
        assert(!"Invalid RegisterId");
        return 0;
    }
}

void Intel64Generator::string(String::Ptr str) {
    // Outputs a string literal definition.
    static uint64_t const READONLY_MASK = 0xf000000000000000LL;
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

void Intel64Generator::gp(uint8_t op, uint8_t ext, RegisterId rm) {
    // Emits a single-operand instruction w/ extension field
    assert(!!rm&&"Invalid register ID");
    assert(is_gp_reg(rm));
    uint8_t const rmid = reg_code(rm);
    rex(RegisterId(), rm);
    text_->uint8(op);
    modrm(MODRM_DIRECT, ext, rmid);
}

void Intel64Generator::gp(uint8_t op, uint8_t ext, RegisterId rm, uint32_t imm) {
    // Emits a register-immediate (32-bit immediate) instruction
    assert(!!rm&&"Invalid register ID");
    assert(is_gp_reg(rm));
    uint8_t const rmid = reg_code(rm);
    rex(RegisterId(), rm);
    text_->uint8(op);
    modrm(MODRM_DIRECT, ext, rmid);
    text_->uint32(imm);
}

void Intel64Generator::gp(uint8_t op, RegisterId reg, RegisterId rm) {
    // Emits a simple register-register instruction 
    // dst = reg, src = rm
    assert(!!rm&&"Invalid register ID");
    assert(!!reg&&"Invalid register ID");
    assert(is_gp_reg(rm));
    assert(is_gp_reg(reg));
    rex(reg, rm);
    text_->uint8(op);
    modrm(MODRM_DIRECT, reg, rm);
}

void Intel64Generator::gp(uint8_t op, RegisterId reg, String* label) {
    // Emits an instruction that uses the memory value at the given label as
    // the second operand.  DO NOT use this for branch instructions, as it
    // assumes 'label' is in the data section!
    assert(!!reg&&"Invalid register ID");
    assert(is_gp_reg(reg));
    rex(reg, RegisterId());
    text_->uint8(op);
    pcrel(reg, label);
}

void Intel64Generator::gp(uint8_t op, RegisterId reg, Operand mem) {
    // Emit an instruction that uses a memory operand specified by register,
    // with a possible offset. 
    assert("Not an indirect operand" && (mem.is_indirect() || op == LEA));
    if (!mem.reg().id()) {
        mem.reg(RBP);
    }
    RegisterId const rm = mem.reg();
    // If the register in the 'mem' operand is 0, then the regiser is
    // understood to be the RBP reg.
    rex(reg, rm);
    text_->uint8(op);
    operands(reg, mem);
}

void Intel64Generator::gp2op(uint8_t op, RegisterId reg, RegisterId rm) {
    // Emits a simple register-register instruction  (2-byte opcode)
    // dst = reg, src = rm
    rex(reg, rm);
    text_->uint8(0x0f);
    text_->uint8(op);
    modrm(MODRM_DIRECT, reg, rm);
}

void Intel64Generator::ssesd(uint8_t op, RegisterId reg, RegisterId rm) {
    // Emit a double-precision, scalar SSE register-register instruction.
    text_->uint8(SD_PREFIX);
    rex(reg, rm);
    text_->uint8(0x0f);
    text_->uint8(op);
    modrm(MODRM_DIRECT, reg, rm);
}

void Intel64Generator::ssesd(uint8_t op, RegisterId reg, Operand mem) {
    // Emit an instruction that uses a memory operand specified by a register,
    // with a possible offset.
    assert("Not an indirect operand" && (mem.is_indirect() || op == LEA));
    if (!mem.reg().id()) {
        mem.reg(RBP);
    }
    RegisterId const rm = mem.reg();
    text_->uint8(SD_PREFIX);
    rex(reg, rm);
    text_->uint8(0x0f);
    text_->uint8(op);
    operands(reg, mem);
}

void Intel64Generator::ssesd(uint8_t op, RegisterId reg, String* label) {
    // Emits an instruction that uses the memory value at the given label as
    // the second operand.  DO NOT use this for branch instructions, as it
    // assumes 'label' is in the data section!
    assert(!!reg&&"Invalid register ID");
    text_->uint8(SD_PREFIX);
    rex(reg, RegisterId());
    text_->uint8(0x0f);
    text_->uint8(op);
    pcrel(reg, label);
}

void Intel64Generator::rex(RegisterId reg, RegisterId rm) {
    // Outputs the REX byte for the given registers, if necessary
    uint8_t rex = REX_PREFIX|REX_W;
    if (!!reg && is_extended_reg(reg)) {
        rex |= REX_R; 
    }
    if (!!rm && is_extended_reg(rm)) {
        rex |= REX_B; 
    }
    text_->uint8(rex);
}

void Intel64Generator::pcrel(RegisterId reg, String* label) {
    // Set up operands to load from a PC-relative address.  If
    // RBP+MODRM_INDIRECT is used, then the address given by 'reg' is
    // RIP-relative rather than RBP-relative.
    uint8_t const regid = reg_code(reg);
    uint8_t const rmid = reg_code(RBP);
    modrm(MODRM_INDIRECT, regid, rmid);
    format_->ref(label, OutputFormat::REF_SIGNED);
    text_->uint32(0); // 32-bit immediate offset
}

void Intel64Generator::operands(RegisterId reg, Operand mem) {
    // Outputs the MODRM byte given the register and a memory operand.  Also
    // outputs the SIB and IMM bytes if necessary.
    RegisterId const rm = mem.reg();
    Address const disp = mem.addr();
    assert("Float mem offset" && !rm.is_float());
    uint8_t const regid = reg_code(reg);
    uint8_t const rmid = reg_code(rm);
    int32_t offset = disp.value() * machine_->word_size();
    if (rm == RBP && offset > 0) {
        offset += machine_->word_size();
        // Add +1*word_size if loading from above the implied base pointer, b/c
        // the SP is stored at offset 0 from the base pointer.  The IR assumes
        // that arguments start at 0, not 1*word_size.  Offsets below the BP
        // are treated normally.
    } 

    // Select the mod field.   Note that R13/RBP is a special case: if mod is
    // 00, then the address loaded is [RIP+disp32] rather than [R13] or [RBP]
    // as one might expect.
    uint8_t modrm = 0;

    // Special case: R13/RBP cannot use MODRM_INDIRECT, b/c with that encoding
    // RIP is used as the indirect register memory operand instead of R13/RBP.
    if (offset == 0 && rm != R13 && rm != RBP) {
        modrm = MODRM_INDIRECT;
    } else if (offset > INT8_MAX || offset < INT8_MIN) {
        modrm = MODRM_DISP32;
    } else {
        modrm = MODRM_DISP8;
    }
    modrm |= (MODRM_REG & (regid << 3));
    modrm |= (MODRM_RM & (rmid));
    text_->uint8(modrm);

    // Special case: RSP requires an SIB byte in indirect addressing mode.
    if (rm == RSP) {
        uint8_t const rsp_sib = 0x24;
        // scale = 00, index = 100 (rsp), base = 100 (rsp) -> 0x24
        text_->uint8(rsp_sib);
    }

    if (offset == 0 && rm != R13 && rm != RBP) {
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

void Intel64Generator::modrm(uint8_t mod, RegisterId reg, RegisterId rm) {
    // Outputs the MODRM byte for a register-register instruction.
    assert(mod==MODRM_DIRECT&&"Only MODRM_DIRECT is supported");
    assert(!!reg&&"Invalid register ID");
    assert(!!rm&&"Invalid register ID");
    uint8_t const regid = reg_code(reg);
    uint8_t const rmid = reg_code(rm);
    modrm(mod, regid, rmid);
}

void Intel64Generator::modrm(uint8_t mod, uint8_t reg, uint8_t rm) {
    // Outputs the MODRM byte for a register-register instruction.
    uint8_t modrm = mod;
    modrm |= (MODRM_REG & (reg << 3));
    modrm |= (MODRM_RM & (rm));
    text_->uint8(modrm);
}

void Intel64Generator::load(RegisterId res, Operand a1) {
    // Loads the literal or a register-indirect memory location specified into
    // a register
    assert(!!res);
    Expression* expr = a1.literal();
    if (StringLiteral* le = dynamic_cast<StringLiteral*>(expr)) {
        uint8_t const rmid = reg_code(res);
        uint8_t const op = MOV_IMM + (MODRM_RM & rmid);
        rex(RegisterId(), res);
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
        mov(res, literal(le));
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        mov(RAX, literal(le));
        movq(res, RAX);
    } else if (a1.label() && a1.is_indirect()) {
        mov(res, a1.label());
    } else if (a1.label()) {
        assert("Not implemented" && !res.is_float());
        uint8_t const rmid = reg_code(res);
        uint8_t const op = MOV_IMM + (MODRM_RM & rmid);
        rex(RegisterId(), res);
        text_->uint8(op);
        format_->ref(a1.label(), OutputFormat::REF_TEXT);
        // Ref the literal in the relocation table
        text_->uint64(0); // Immediate
    } else if (!!a1.addr() && !a1.is_indirect()) {
        assert("Not implemented" && !res.is_float());
        lea(res, a1);
    } else {
        if (res.is_float()) {
            movsd(res, a1);
        } else {
            mov(res, a1);
        }
    }
}

void Intel64Generator::store(Operand a1, Operand a2) {
    // Implements the STORE IR opcode.  STORE is a more complex IR op; it can
    // be used to put a variety of operands into memory with different
    // addresses.
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
        mov(RAX, literal(le));
        mov(a1, RAX);
    } else if (FloatLiteral* le = dynamic_cast<FloatLiteral*>(expr)) {
        assert(!"Not implemented");
    } else if (a1.label()) {
        if (a2.is_float()) {
            assert(!"Not implemented");
        } else {
            assert("Direct label store"&&a1.is_indirect());
            assert("Offset in label store"&&!a1.addr().value());
            mov(a1.label(), a2.reg());
        }
    } else {
        if (a2.is_float()) {
            movsd(a1, a2.reg());
        } else {
            mov(a1, a2.reg());
        }
    }
}

void Intel64Generator::add(RegisterId res, RegisterId r1, RegisterId r2) {
    typedef void (Intel64Generator::*Func)(RegisterId, RegisterId);
    Func add = &Intel64Generator::add;
    Func mov = &Intel64Generator::mov;
    if (res.is_float()) {
        add = &Intel64Generator::addsd;
        mov = &Intel64Generator::movsd;
    }

    if (res == r1) { // res <- res + r2
        (this->*add)(res, r2);
    } else if (res == r2) { // res <- r1 + res
        (this->*add)(res, r1);
    } else { // res <- r1 + r2
        (this->*mov)(res, r1); 
        (this->*add)(res, r2);
    }
}

void Intel64Generator::sub(RegisterId res, RegisterId r1, RegisterId r2) {
    typedef void (Intel64Generator::*Func)(RegisterId, RegisterId);
    Func sub = &Intel64Generator::sub;
    Func mov = &Intel64Generator::mov;
    if (res.is_float()) {
        sub = &Intel64Generator::subsd;
        mov = &Intel64Generator::movsd;
    }

    if (res == r1) { // res <- res - r2
        (this->*sub)(res, r2);
    } else if (res == r2) { // res <- r1 - res
        assert("Not implemented"&&!res.is_float());
        neg(res);
        (this->*sub)(res, r1);
    } else { // res <- r1 - r2
        (this->*mov)(res, r1);
        (this->*sub)(res, r2); 
    }
}

void Intel64Generator::mul(RegisterId res, RegisterId r1, RegisterId r2) {
    typedef void (Intel64Generator::*Func)(RegisterId, RegisterId);
    Func mul = &Intel64Generator::imul;
    Func mov = &Intel64Generator::mov;
    if (res.is_float()) {
        mul = &Intel64Generator::mulsd;
        mov = &Intel64Generator::movsd;
    }

    if (res == r1) { // res <- res * r2
        (this->*mul)(res, r2);
    } else if (res == r2) { // res <- r1 * res
        (this->*mul)(res, r1);
    } else { // res <- r1 + r2
        (this->*mov)(res, r1); 
        (this->*mul)(res, r2);
    }
}

void Intel64Generator::div(RegisterId res, RegisterId r1, RegisterId r2) {
    if (!res.is_float()) {
        push(RDX);
        push(RBX);
        mov(RAX, r1);
        mov(RBX, r2);
        cqo();  
        idiv(RBX);
        pop(RBX);
        pop(RDX);
        mov(res, RAX);
        return;
    }
    if (res == r1) { // res <- res / r2
        divsd(res, r2);
    } else if (res == r2) { // res <- r1 / res
        movq(RAX, r1); // save r1
        divsd(r1, res);
        movsd(res, r1); // move r1 => res 
        movq(r1, RAX); // restore r1
    } else { // res <- r1 / r2
        movsd(res, r1);
        divsd(res, r2);
    }
}

void Intel64Generator::neg(RegisterId res, RegisterId a1) {
    // If reg is floating point, then we need to subtract from zero to do a
    // negation.
    if (res.is_float()) {
        mov(RAX, (uint64_t)0);
        cvtsi2sd(res, RAX);
        subsd(res, a1);
    } else {
        mov(res, a1);
        neg(res);
    }
}

void Intel64Generator::cvtsi2sd(RegisterId dst, RegisterId src) {
    // Moves a 64-bit GP reg value to an XMM register, doing conversion to
    // floating point.
    assert("Invalid cvtsi2sd" && dst.is_float() && !src.is_float());
    text_->uint8(SD_PREFIX); // CVTSI prefix
    rex(dst, src);  
    text_->uint8(0x0f);
    text_->uint8(0x2a);
    modrm(MODRM_DIRECT, dst, src); 
}

void Intel64Generator::movq(RegisterId dst, RegisterId src) {
    // Emits a MOVQ instruction, which moves a 64-bit value from an XMM
    // register to a GP register, or vice versa, without doing any conversions.
    assert("Invalid movq" && dst.is_float() != src.is_float());
    text_->uint8(0x66); // MOVQ prefix
    if (dst.is_float()) { // movq xmm r/m64 = 0x66 0f 6e
        rex(dst, src);
    } else {
        rex(src, dst);
    }
    text_->uint8(0x0f);
    if (dst.is_float()) { // movq xmm r/m64 = 0x66 0f 6e
        text_->uint8(0x6e);
        modrm(MODRM_DIRECT, dst, src);
    } else { // movq r/m64 xmm = 0x66 0f 7e
        text_->uint8(0x7e);
        modrm(MODRM_DIRECT, src, dst);     
    }
}

void Intel64Generator::movsd(RegisterId dst, Operand src) {
    ssesd(MOVSD_REG_RM, dst, src);
}

void Intel64Generator::movsd(Operand dst, RegisterId src) {
    ssesd(MOVSD_RM_REG, src, dst);
}

void Intel64Generator::movsd(RegisterId dst, RegisterId src) {
    ssesd(MOVSD_REG_RM, dst, src);
}

void Intel64Generator::addsd(RegisterId dst, RegisterId src) {
    ssesd(0x58, dst, src);
}

void Intel64Generator::subsd(RegisterId dst, RegisterId src) {
    ssesd(0x5c, dst, src);
}

void Intel64Generator::mulsd(RegisterId dst, RegisterId src) {
    ssesd(0x59, dst, src);
}

void Intel64Generator::divsd(RegisterId dst, RegisterId src) {
    ssesd(0x5e, dst, src);
}

void Intel64Generator::imul(RegisterId reg, RegisterId rm) {
    gp2op(0xaf, reg, rm);
}

void Intel64Generator::idiv(RegisterId reg) {
    gp(0xf7, 0x07, reg);
}

void Intel64Generator::neg(RegisterId reg) {
    gp(0xf7, 0x03, reg);
}

void Intel64Generator::bnot(RegisterId reg) {
    gp(0xf7, 0x02, reg);
}

void Intel64Generator::band(RegisterId dst, RegisterId src) {
    gp(0x23, dst, src);
}

void Intel64Generator::bor(RegisterId dst, RegisterId src) {
    gp(0x0b, dst, src);
}

void Intel64Generator::mov(RegisterId dst, RegisterId src) {
    // Emits a register-register or move instruction.
    if (dst != src) {
        gp(MOV_REG_RM, dst, src);
    }
}

void Intel64Generator::mov(RegisterId reg, Operand rm) {
    // Moves a value from memory into 'reg', using 'rm' as the base.
    gp(MOV_REG_RM, reg, rm);
}

void Intel64Generator::mov(Operand rm, RegisterId reg) {
    // Moves a value to memory from 'reg', using 'rm' as the base.
    gp(MOV_RM_REG, reg, rm);
}

void Intel64Generator::mov(RegisterId reg, String* label) {
    // Emits an instruction to move the memory value stored at the label into
    // the given register.
    if (reg.is_float()) {
        ssesd(MOVSD_REG_RM, reg, label);
    } else {
        gp(MOV_REG_RM, reg, label);
    }
}

void Intel64Generator::mov(String* label, RegisterId reg) {
    // Emits an instruction to move a register to the memory location indicated
    // by the label.
    gp(MOV_RM_REG, reg, label);
}

void Intel64Generator::mov(RegisterId reg, uint64_t imm) {
    // Emits a register-immediate move instruction with a 64-bit immediate
    // value.  If the immediate is smaller than 32 bits, use a 32-bit
    // instruction instead.
    uint8_t const regid = reg_code(reg);
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
    gp(LEA, reg, rm);
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
        gp(0xff, 0x02, target.reg());
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
    assert("Invalid RegisterId" && !!reg.id());
    uint8_t const regid = reg_code(reg);
    if (is_extended_reg(reg)) {
        text_->uint8(REX_PREFIX|REX_W|REX_B);
    }
    text_->uint8(0x50+(MODRM_RM & regid));
}

void Intel64Generator::pop(RegisterId reg) {
    assert("Invalid RegisterId" && !!reg.id());
    uint8_t const regid = reg_code(reg);
    if (is_extended_reg(reg)) {
        text_->uint8(REX_PREFIX|REX_W|REX_B);
    }
    text_->uint8(0x58+(MODRM_RM & regid));
}

void Intel64Generator::cmp(RegisterId dst, RegisterId src) {
    gp(0x3b, dst, src);  
}

void Intel64Generator::test(RegisterId dst, RegisterId src) {
    gp(0x85, dst, src);  
}

void Intel64Generator::add(RegisterId dst, RegisterId src) {
    gp(0x03, dst, src);
}

void Intel64Generator::add(RegisterId reg, uint32_t imm) {
    gp(0x81, 0x00, reg, imm);
}

void Intel64Generator::sub(RegisterId dst, RegisterId src) {
    gp(0x2b, dst, src);
}

void Intel64Generator::sub(RegisterId dst, uint64_t imm) {
    gp(0x81, 0x05, dst, imm);
}


void Intel64Generator::leave() {
    text_->uint8(0xc9);
}

void Intel64Generator::ret() {
    text_->uint8(0xc3); 
}

void Intel64Generator::cqo() {
    text_->uint8(0x99);
}
