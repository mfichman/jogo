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

#include "Apollo.hpp"
#include "Environment.hpp"
#include "BasicBlock.hpp"
#include "Object.hpp"
#include "Machine.hpp"
#include "Scope.hpp"
#include <vector>
#include <map>


/* Code generator structure; creates basic block flow graphs */
class BasicBlockGenerator : public TreeNode::Functor {
public:
    BasicBlockGenerator(Environment* env, Machine* mach);
    typedef Pointer<BasicBlockGenerator> Ptr; 
    void operator()(File* file);

private:
    void operator()(Constant* feature);
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Is* expression);
    void operator()(Member* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(ConstantIdentifier* expression);
    void operator()(Empty* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
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

    Operand emit(TreeNode* node, BasicBlock* yes, BasicBlock* no, bool inv); 
    Operand emit(TreeNode* node);
    Operand emit(Opcode op, Operand t2, Operand t3);
    Operand emit(Opcode op, Operand t2); 
    Operand mov(Operand res, Operand t2); 
    Operand mov(Operand t2) { return emit(MOV, t2); }
    Operand neg(Operand t2) { return emit(NEG, t2); }
    Operand add(Operand t2, Operand t3) { return emit(ADD, t2, t3); }
    Operand sub(Operand t2, Operand t3) { return emit(SUB, t2, t3); }
    Operand mul(Operand t2, Operand t3) { return emit(MUL, t2, t3); }
    Operand div(Operand t2, Operand t3) { return emit(DIV, t2, t3); }
    Operand notb(Operand t2) { return emit(NOTB, t2); }
    Operand andb(Operand t2, Operand t3) { return emit(ANDB, t2, t3); }
    Operand orb(Operand t2, Operand t3) { return emit(ORB, t2, t3); }
    Operand load(Operand addr) { return emit(LOAD, addr); }
    void store(Operand addr, Operand value);
    void call(Operand func);
    void ret();
    void bne(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void be(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void bnz(Operand t2, BasicBlock* branch, BasicBlock* next);
    void bz(Operand t2, BasicBlock* branch, BasicBlock* next);
    void bg(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void bl(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void bge(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void ble(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next);
    void jump(BasicBlock* target);
    void emit(BasicBlock* block); 
    void branch(Opcode o, Operand t2, Operand t3, BasicBlock* b, BasicBlock* n);

    void variable(Variable* var);
    void enter_scope();
    void exit_scope();
    void call(Function* func, Expression* args);
    void native_operator(Call* expression);
    void scope_cleanup(Variable* var);
	void exception_catch();
    void refcount_inc(Operand var);
    void refcount_dec(Operand var);
    void dispatch_table(Class* clazz);
    void func_return();
    void ctor_preamble(Class* clazz);
    void dtor_epilog(Function* func);
    void copier_preamble(Class* clazz);
    void free_temps();
    void calculate_size(Class* clazz);
    void constants();
    void local_slots_inc(int count);
    void local_slots_dec(int count);
    void arg_slots_inc(int count);
    void arg_slots_dec(int count);
    void attr_assignment(Assignment* expr);
    void initial_assignment(Assignment* expr);
    void secondary_assignment(Assignment* expr);  
    void value_assign_reg(Operand src, Operand dst, Type* type);
    void value_assign_mem(Operand src, Operand dst, Type* type);
    void value_copy(Operand src, Operand dst, Type* type);
    void value_move(Operand src, Operand dst, Type* type);
    void value_dtor(Operand op, Type* type);
    Operand bool_expr(Expression* expr);
    Operand pop_ret(Type* type);
    Operand id_operand(String* id);
    Operand stack_value(Type* type);
    Operand stack_value_temp(Type* type);
    RegisterId temp_inc() { return RegisterId(++temp_, 0); }
    Variable* variable(String* name);
    BasicBlock* basic_block();

    Environment::Ptr env_;
    Machine::Ptr machine_;
    Class::Ptr class_;
    Module::Ptr module_;
    Function::Ptr function_;
    BasicBlock::Ptr block_;
    BasicBlock::Ptr true_;
    BasicBlock::Ptr false_;
    Operand return_;
    
    std::vector<Scope::Ptr> scope_;
    // Mapping from var to temporary

    Operand assign_addr_;
    int local_slots_;
    int arg_slots_;
    // Mapping from a variable to a stack location

    Opcode branch_op_;
    bool invert_branch_;
    bool invert_guard_;
    // Next temporary to use
    int temp_;
    int label_;

    std::vector<Operand> object_temp_; 
    std::map<RegisterId, Variable::Ptr> value_temp_;
    // Temporaries to free at end of statement

    friend class FuncMarshal;
    friend class FuncUnmarshal;
};


/* Helper class for mashalling function parameters */
class FuncMarshal {
public:
    FuncMarshal(BasicBlockGenerator* gen) : 
        gen_(gen), int_args_(0), float_args_(0) {
    }
    void arg(Operand op); 
    void call(Operand func);

private:
    BasicBlockGenerator* gen_;
    std::vector<Operand> arg_;
    int int_args_;
    int float_args_;
};

/* Helper class for unmarshalling function parameters */
class FuncUnmarshal {
public:
    FuncUnmarshal(BasicBlockGenerator* gen) :
        gen_(gen), int_args_(0), float_args_(0), stack_args_(0) {
    }
    void arg(String* name, Type* type);

private:
    BasicBlockGenerator* gen_;
    int int_args_;
    int float_args_;
    int stack_args_;
};



