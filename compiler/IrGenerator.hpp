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

#pragma once

#include "Jogo.hpp"
#include "Environment.hpp"
#include "IrBlock.hpp"
#include "IrScope.hpp"
#include "Object.hpp"
#include "Machine.hpp"
#include <vector>
#include <map>


/* Code generator structure; creates basic block flow graphs */
class IrGenerator : public TreeNode::Functor {
public:
    IrGenerator(Environment* env, Machine* mach);
    typedef Pointer<IrGenerator> Ptr; 
    void operator()(File* file);

private:
    void operator()(Constant* feature);
    void operator()(Class* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(ArrayLiteral* expression);
    void operator()(HashLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Is* expression);
    void operator()(Member* expression);
    void operator()(Construct* expression);
    void operator()(IdentifierRef* expression);
    void operator()(ConstantRef* expression);
    void operator()(Empty* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Block* statement);
    void operator()(Let* let);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(Match* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);
    void operator()(Closure* expression);
    void operator()(Type* type);

    IrValue::Ptr emit(Expression* node, IrBlock* yes, IrBlock* no, bool inv); 
    IrValue::Ptr emit(Expression* node);
    IrValue::Ptr copy(IrValue* val);
    Operand emit(Opcode op, Operand t2, Operand t3);
    Operand emit(Opcode op, Operand t2); 
    Operand mov(Operand t2);
    Operand mov(Operand res, Operand t2);
    Operand neg(Operand t2) { return emit(NEG, t2); }
    Operand add(Operand t2, Operand t3) { return emit(ADD, t2, t3); }
    Operand sub(Operand t2, Operand t3) { return emit(SUB, t2, t3); }
    Operand mul(Operand t2, Operand t3) { return emit(MUL, t2, t3); }
    Operand div(Operand t2, Operand t3) { return emit(DIV, t2, t3); }
    Operand notb(Operand t2) { return emit(NOTB, t2); }
    Operand andb(Operand t2, Operand t3) { return emit(ANDB, t2, t3); }
    Operand orb(Operand t2, Operand t3) { return emit(ORB, t2, t3); }
    Operand phi(PhiArg* args) { return emit(PHI, Operand(args)); }
    Operand load(Operand addr);
    //Operand load(Operand res, Operand addr);
    void store(Operand addr, Operand value);
    void call(Operand func);
    void ret();
    void bne(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void be(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void bnz(Operand t2, IrBlock* branch, IrBlock* next);
    void bz(Operand t2, IrBlock* branch, IrBlock* next);
    void bg(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void bl(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void bge(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void ble(Operand t2, Operand t3, IrBlock* branch, IrBlock* next);
    void jump(IrBlock* target);
    void label(IrBlock* block); 
    void branch(Opcode o, Operand t2, Operand t3, IrBlock* b, IrBlock* n);

    void variable(IrVariable const& var);
    void enter_scope();
    void exit_scope();
    void call(Function* func, Expression* args, Expression* recv, Type* ret);
    void native_operator(Call* expression);
	void exception_catch();
    void refcount_inc(Operand var);
    void refcount_dec(Operand var);
    void dispatch_table(Class* clazz);
    void func_return(IrValue* val);
    void ctor_preamble(Class* clazz);
    void dtor_epilog(Function* func);
    void copier_preamble(Class* clazz);
    void drop_value(Operand val, Type* type);
    void calculate_size(Class* clazz);
    void constants();
    void local_slots_inc(int count);
    void local_slots_dec(int count);
    void arg_slots_inc(int count);
    void arg_slots_dec(int count);
    void attr_assignment(Assignment* expr);
    void initial_assignment(Assignment* expr);
    void secondary_assignment(Assignment* expr);  
    void value_copy(Operand src, Operand dst, Type* type);
    void value_dtor(Operand op, Type* type);
    void gen_phi(IrBlock* block);
    int arg_slots() const { return arg_slots_; }
    Operand last_assignment_of(IrBlock* block, String* name);
    IrValue::Ptr bool_expr(Expression* expr);
    IrValue::Ptr pop_ret(Type* type);
    IrValue::Ptr id_operand(String* id);
    IrValue::Ptr stack_value(Type* type);
    RegisterId temp_inc() { return RegisterId(++temp_, 0); }
    IrValue* variable(String* name);
    IrBlock* ir_block();

    Environment::Ptr env_;
    Machine::Ptr machine_;
    Class::Ptr class_;
    Function::Ptr function_;
    IrBlock::Ptr block_;
    IrBlock::Ptr true_;
    IrBlock::Ptr false_;
    IrValue::Ptr return_;
    
    std::vector<IrScope::Ptr> scope_;
    // Mapping from var to temporary

    int local_slots_;
    int arg_slots_;
    // Mapping from a variable to a stack location

    Opcode branch_op_;
    bool invert_branch_;
    bool invert_guard_;
    // Next temporary to use
    int temp_;
    int label_;

    friend class FuncMarshal;
    friend class FuncUnmarshal;
    friend class IrValue;
};


/* Helper class for mashalling function parameters */
class FuncMarshal {
public:
    FuncMarshal(IrGenerator* gen) : gen_(gen) {
    }
    void arg(Operand op); 
    void call(Operand func);

private:
    IrGenerator* gen_;
    std::vector<Operand> arg_;
};

/* Helper class for unmarshalling function parameters */
class FuncUnmarshal {
public:
    FuncUnmarshal(IrGenerator* gen) :
        gen_(gen), int_args_(0), float_args_(0), stack_args_(0) {
    }
    void arg(String* name, Type* type);

private:
    IrGenerator* gen_;
    int int_args_;
    int float_args_;
    int stack_args_;
};

