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
#include <vector>
#include <map>

/* Variable structure, used to store info about assigned vars in a scope */
class Variable : public Object {
public:
    Variable(String* name, Operand op, Type* type) :
        name_(name),
        operand_(op),
        type_(type) {
    }

    String* name() const { return name_; }
    Operand operand() const { return operand_; }
    Type* type() const { return type_; }
    typedef Pointer<Variable> Ptr;

private:
    String::Ptr name_;
    Operand operand_;
    Type::Ptr type_; 
};

/* Scope structure for recording end-of-scope cleanup info */
class Scope : public Object {
public:
    Scope() : has_return_(0) {}

    void variable(Variable* variable) { 
        variable_.push_back(variable);
    }
    Variable* variable(String* name) const { 
        for (int i = 0; i < variable_.size(); i++) {
            if (variable_[i]->name() == name) {
                return variable_[i];
            }
        }
        return 0;
    }
    Variable* variable(int index) const {
        return variable_[index];
    }
    void return_val(Operand val) { return_val_ = val; }
    void has_return(bool ret) { has_return_ = ret; }
    int variables() const { return variable_.size(); }
    BasicBlock* cleanup() const { return cleanup_; }
    Operand return_val() const { return return_val_; }
    bool has_return() const { return has_return_; }

    typedef Pointer<Scope> Ptr;

private:
    BasicBlock::Ptr cleanup_; // Block for inserting stack cleanup code
    bool has_return_;
    Operand return_val_;
    std::vector<Variable::Ptr> variable_;
};

/* Code generator structure; creates basic block flow graphs */
class BasicBlockGenerator : public TreeNode::Functor {
public:
    BasicBlockGenerator(Environment* env, Machine* mach);
    typedef Pointer<BasicBlockGenerator> Ptr; 

private:
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Dispatch* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(Empty* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(Let* let);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(When* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);
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
    
    Operand mov(Operand t2) {
        block_->instr(MOV, ++temp_, t2, 0);
        return temp_;
    }

    Operand add(Operand t2, Operand t3) {
        block_->instr(ADD, ++temp_, t2, t3);
        return temp_;
    }

    Operand sub(Operand t2, Operand t3) {
        block_->instr(SUB, ++temp_, t2, t3);    
        return temp_;
    }

    Operand mul(Operand t2, Operand t3) {
        block_->instr(MUL, ++temp_, t2, t3);    
        return temp_;
    }

    Operand div(Operand t2, Operand t3) {
        block_->instr(DIV, ++temp_, t2, t3);    
        return temp_;
    }

    Operand notb(Operand t2) {
        block_->instr(NOTB, ++temp_, t2, 0);
        return temp_;
    }       

    Operand andb(Operand t2, Operand t3) {
        block_->instr(ANDB, ++temp_, t2, t3);    
        return temp_;
    }

    Operand orb(Operand t2, Operand t3) {
        block_->instr(ORB, ++temp_, t2, t3);    
        return temp_;
    }

    Operand load(Operand t2) {
        block_->instr(LOAD, ++temp_, t2, 0);    
        return temp_;
    }

    Operand pop() {
        block_->instr(POP, ++temp_, 0, 0);    
        return temp_;
    }
    
    void push(Operand t2) {
        block_->instr(PUSH, 0, t2, 0);    
    }

    void store(Operand t2, Operand t3) {
        block_->instr(STORE, 0, t2, t3);    
    }

    void call(Operand func) {
        block_->instr(CALL, 0, func, 0);
    }
    
    void ret() {
        block_->instr(RET, 0, 0, 0);
    }
    
    void bne(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BNE, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void be(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BE, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bnz(Operand t2, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BNZ, 0, t2, 0);
        block_->branch(branch);
        block_->next(next);
    }

    void bz(Operand t2, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BZ, 0, t2, 0);
        block_->branch(branch);
        block_->next(next);
    }

    void bg(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BG, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bl(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BL, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void bge(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BGE, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }
    
    void ble(Operand t2, Operand t3, BasicBlock* branch, BasicBlock* next) {
        block_->instr(BLE, 0, t2, t3);
        block_->branch(branch);
        block_->next(next);
    }

    void jump(BasicBlock* target) {
        block_->instr(JUMP, 0, 0, 0);
        block_->branch(target);
    }

    BasicBlock* basic_block();
    int stack(String* name);

    Variable* variable(String* name);
    void variable(Variable* var);
    void stack(String* name, int offset);
    void enter_scope();
    void exit_scope();
    void emit_operator(Dispatch* expression);
    void emit_var_cleanup(Variable* var);
    void emit_refcount_inc(Operand var);
    void emit_refcount_dec(Operand var);
    void emit_vtable(Class* clazz);
    void emit_return();
    void emit_push_arg(int i, Operand arg);
    void emit_ctor_preamble(Function* func);
    void calculate_size(Class* clazz);
    Operand emit_pop_ret();

    Environment::Ptr env_;
    Machine::Ptr machine_;
    Class::Ptr class_;
    Module::Ptr module_;
    Function::Ptr function_;
    BasicBlock::Ptr block_;
    BasicBlock::Ptr true_;
    BasicBlock::Ptr false_;
    Operand return_;
    
    // Mapping from var to temporary
    std::vector<Scope::Ptr> scope_;
    std::map<String::Ptr, int> stack_;

    // Next temporary to use
    Operand temp_;
    int label_;
    Opcode branch_op_;
    bool invert_branch_;
};

