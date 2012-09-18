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

    void emit(BasicBlock* block) {
        if (block_) {
            block_->next(block);
        }
        block_ = block;
        function_->basic_block(block);
    }
    
    Operand emit(TreeNode* node, BasicBlock* yes, BasicBlock* no, bool inv) {
        BasicBlock* true_save = true_;
        BasicBlock* false_save = false_;
        bool invert_branch_save_ = invert_branch_;
        true_ = yes;
        false_ = no;
        invert_branch_ = inv;
        node->operator()(this);
        true_ = true_save;
        false_ = false_save;
        invert_branch_ = invert_branch_save_;
        return return_;
    }

    Operand emit(TreeNode* node) {
        node->operator()(this);
        return return_;
    }

    Operand emit(Opcode op, Operand t2, Operand t3) {
        block_->instr(op, RegisterId(++temp_, 0), t2, t3);
        return RegisterId(temp_, 0);
    }
    
    Operand emit(Opcode op, Operand t2) {
        block_->instr(op, RegisterId(++temp_, 0), t2, Operand());
        return RegisterId(temp_, 0);
    }
    
    Operand mov(Operand res, Operand t2) { 
        // FixMe: This could be a problem for SSA analysis.
        block_->instr(MOV, res, t2, Operand());
        return res;
    }

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

    Operand pop() {
        block_->instr(POP, RegisterId(++temp_, 0), Operand(), Operand());    
        return RegisterId(temp_, 0);
    }

    void popn(int num) {
        String::Ptr val = env_->integer(stringify(num)); 
        Operand op(new IntegerLiteral(Location(), val));
        block_->instr(POPN, Operand(), op, Operand());
    }
    
    void push(Operand t2) {
        block_->instr(PUSH, Operand(), t2, Operand());    
    }

    void pushn(int num) {
        String::Ptr val = env_->integer(stringify(num));
        Operand op(new IntegerLiteral(Location(), val));
        block_->instr(PUSHN, Operand(), op, Operand());
    }

    void store(Operand addr, Operand value) {
        block_->instr(STORE, Operand(), addr, value);    
    }

    void call(Operand func, int nargs) {
        block_->instr(CALL, Operand(), func, Operand());
        pop_args(nargs);
    }
    
    void ret() {
        block_->instr(RET, Operand(), Operand(), Operand());
    }
    
    void bne(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BNE, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void be(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BE, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bnz(Operand t2, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BNZ, Operand(), t2, Operand());
        block_->branch(branch);
        block_->next(next);
    }

    void bz(Operand t2, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BZ, Operand(), t2, Operand());
        block_->branch(branch);
        block_->next(next);
    }

    void bg(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BG, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bl(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BL, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bge(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BGE, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }
    
    void ble(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BLE, Operand(), t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void jump(BasicBlock* target) {
        block_->instr(JUMP, Operand(), Operand(), Operand());
        block_->branch(target);
    }

    BasicBlock* basic_block();
    Address stack(String* name);

    Variable* variable(String* name);
    void variable(Variable* var);
    void stack(String* name, Address offset);
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
    void push_arg(int i, Operand arg);
    void save_arg(int i, String* formal);
    void pop_args(int count);
    void ctor_preamble(Class* clazz);
    void dtor_epilog(Function* func);
    void copier_preamble(Class* clazz);
    void free_temps();
    void calculate_size(Class* clazz);
    void constants();
    void stack_values_inc(int count);
    void stack_values_dec(int count);
    void attr_assignment(Assignment* expr);
    void initial_assignment(Assignment* expr);
    void secondary_assignment(Assignment* expr);  
    void value_assign_reg(Operand src, Operand dst, Type* type);
    void value_assign_mem(Operand src, Operand dst, Type* type);
    void value_copy(Operand src, Operand dst, Type* type);
    void value_move(Operand src, Operand dst, Type* type);
    void value_dtor(Operand op, Type* type);
    Operand bool_expr(Expression* expr);
    Operand pop_ret();
    Operand id_operand(String* id);
    Operand stack_value(Type* type);

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

    std::map<String::Ptr, Address> stack_;
    int stack_values_;
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
};

