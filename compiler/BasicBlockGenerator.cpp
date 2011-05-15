/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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

#include "BasicBlockGenerator.hpp"
#include <cassert>

#define REFCOUNT_OFFSET 0

using namespace std;

BasicBlockGenerator::BasicBlockGenerator(Environment* env) :
    environment_(env),
    temp_(0) {
}

typedef Pointer<BasicBlockGenerator> Ptr; 

void BasicBlockGenerator::operator()(Class* feature) {
}

void BasicBlockGenerator::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockGenerator::operator()(StringLiteral* expression) {
    // Load a pointer to the string from the string table
    assert(!"Not implemented");
}

void BasicBlockGenerator::operator()(IntegerLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = li(block_, expression->value());
}

void BasicBlockGenerator::operator()(Binary* expression) {
    // Emit the left and right expressions, then perform the operation on
    // the two operands, and return the result.
    Operand left = emit(expression->left());
    Operand right = emit(expression->right());

    if (environment_->name("or") == expression->operation()) {
        return_ = orl(block_, left, right);
    } else if (environment_->name("and") == expression->operation()) {
        return_ = andl(block_, left, right);
    }
}

void BasicBlockGenerator::operator()(Unary* expression) {
}

void BasicBlockGenerator::operator()(Call* expression) {
    // Push objects in anticipation of the call instruction.
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        push(block_, emit(a));
    }

    // Look up the function by name in the current context.
    String::Ptr id = expression->module();
    String::Ptr scope = expression->module();
    String::Ptr name;
    if (expression->unit()) {
        name = expression->unit()->function(scope, id)->name();
    } else {
        name = module_->function(scope, id)->name();
    }

    // Insert a call expression, then pop the return value off the stack.
    call(block_, name);
    return_ = pop(block_);
}

void BasicBlockGenerator::operator()(Dispatch* expression) {
}

void BasicBlockGenerator::operator()(Identifier* expression) {
    // Simply look up the value of the variable as stored previously.
    return_ = variable(expression->identifier());
}

void BasicBlockGenerator::operator()(Block* statement) {
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
}

void BasicBlockGenerator::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void BasicBlockGenerator::operator()(While* statement) {
    // Jump to the setup block, then emit the guard expression.
    block_ = jump(block_);
    Operand guard = emit(statement->guard());
    BasicBlock::Ptr pre = bneqz(block_, guard);
    
    // Now generate the loop (true) block.  Make sure that the fallthrough
    // faster the loop block is back to the guard block
    block_ = pre->branch();
    emit(statement->block());
    block_->next(pre);

    // Now set the pass-through block and continue code with the following
    // statement.
    block_ = pre->next();
}

void BasicBlockGenerator::operator()(For* statement) {
    assert(!"For loops should be converted into while loops by expansion");
}

void BasicBlockGenerator::operator()(Conditional* statement) {
    Operand guard = emit(statement->guard());
    BasicBlock::Ptr pre = beqz(block_, guard);

    // Emit the fall-through (true) block here.  Switch the insert block to
    // the 'next' block.
    block_ = pre->next();
    emit(statement->true_branch());

    // Emit the branch (false) block here.  Switch the insert block to the
    // 'branch' block.
    block_ = pre->branch();
    emit(statement->false_branch());

    // Jump to the post block (after the conditional) from the branch block,
    // and fall-through from the true block to the post block.
    block_ = jump(block_);
    pre->next()->next(block_);
}

void BasicBlockGenerator::operator()(Variable* statement) {
}

void BasicBlockGenerator::operator()(Return* statement) {
}

void BasicBlockGenerator::operator()(When* statement) {
}

void BasicBlockGenerator::operator()(Case* statement) {
}

void BasicBlockGenerator::operator()(Function* feature) {
}

void BasicBlockGenerator::operator()(Attribute* feature) {
}

void BasicBlockGenerator::operator()(Import* feature) {
}

Operand BasicBlockGenerator::variable(String* name) {
    vector<map<String::Ptr, Operand> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<String::Ptr, Operand>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return Operand();
}

void BasicBlockGenerator::variable(String* name, Operand temporary) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, temporary));
}

void BasicBlockGenerator::enter_scope() {
    variable_.push_back(map<String::Ptr, Operand>());
}

void BasicBlockGenerator::exit_scope() {
    variable_.pop_back();
}


