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
#include "Object.hpp"
#include "Machine.hpp"
#include "Scope.hpp"
#include <vector>
#include <map>


/* Keeps track of values emitted by expressions */
class IrValue : public Object {
public:
    typedef int Flags;

    IrValue(IrGenerator* gen, Operand op, Type* type, Flags flags=0);
    IrValue();
    ~IrValue();
    void is_param(bool param) { flags_ = param ? (flags_ | PARAM) : (flags_ & ~PARAM); }
    void is_var(bool var) { flags_ = var ? (flags_ | VAR) : (flags_ & ~VAR); }
    void is_dead(bool dead) { flags_ = dead ? ( flags_ | DEAD) : (flags_ & ~DEAD); }
    bool is_param() const { return flags_ & PARAM; }
    bool is_var() const { return flags_ & VAR; }
    bool is_dead() const { return flags_ & DEAD; }
    Operand const& operand() const { return operand_; }
    Type* type() const { return type_; }
    Flags flags() const { return flags_; }

    static const int DEAD = 0x1;
    static const int VAR = 0x2;
    static const int PARAM = 0x4;
    typedef Pointer<IrValue> Ptr;
private:
    IrGenerator* generator_;
    Operand operand_;
    Type* type_; 
    Flags flags_;
};

class IrVariable  {
public:
    IrVariable() : name_(0), value_(0) {}
    IrVariable(String* name, IrValue* val) :
        name_(name),
        value_(val) {

        assert("Literal or label in variable" && !val->operand().object());
        assert("Address in variable" && !val->operand().addr());
    } 
    IrValue* value() const { return value_; }
    String* name() const { return name_; }

private:
    String::Ptr name_;
    IrValue::Ptr value_;
};

/* Keeps track of variables in the current scope */
class IrScope : public Object {
public:
    ~IrScope();
    void variable(IrVariable const& var) { variable_.push_back(var); }
    void ret(IrValue* ret) { ret_ = ret; }
    IrVariable const& variable(int index) const { return variable_[index]; }
    IrValue* variable(String* name) const;
    IrValue* ret() const { return ret_; }
    int variables() const { return variable_.size(); }

    typedef Pointer<IrScope> Ptr;
private:
    IrValue::Ptr ret_;
    std::vector<IrVariable> variable_;  
};

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
    Operand mov(Operand res, Operand t2); 
    Operand mov(Operand t2);
    Operand neg(Operand t2) { return emit(NEG, t2); }
    Operand add(Operand t2, Operand t3) { return emit(ADD, t2, t3); }
    Operand sub(Operand t2, Operand t3) { return emit(SUB, t2, t3); }
    Operand mul(Operand t2, Operand t3) { return emit(MUL, t2, t3); }
    Operand div(Operand t2, Operand t3) { return emit(DIV, t2, t3); }
    Operand notb(Operand t2) { return emit(NOTB, t2); }
    Operand andb(Operand t2, Operand t3) { return emit(ANDB, t2, t3); }
    Operand orb(Operand t2, Operand t3) { return emit(ORB, t2, t3); }
    Operand load(Operand addr);
    Operand load(Operand res, Operand addr);
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
    void assign(IrValue* src, IrValue* dst);
    int arg_slots() const { return arg_slots_; }
    IrValue::Ptr assign_loc_alloc(Type* type, IrValue::Flags flags);
    IrValue::Ptr bool_expr(Expression* expr);
    IrValue::Ptr pop_ret(Type* type);
    IrValue::Ptr id_operand(String* id);
    IrValue::Ptr stack_value(Type* type);
    IrValue::Ptr stack_value_temp(Type* type);
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

    Operand assign_loc_;
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

