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

void
BasicBlockGenerator::operator()(Class* unit) {
}

void
BasicBlockGenerator::operator()(Module* unit) {
}

void
BasicBlockGenerator::operator()(StringLiteral* expression) {
}

void
BasicBlockGenerator::operator()(IntegerLiteral* expression) {
}

void
BasicBlockGenerator::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    

    left(this);
    int l = temp_;
    right(this);
    int r = temp_;

    if (environment_->name("or") == expression->operation()) {
        orl(block_, l, r);
    } else if (environment_->name("and") == expression->operation()) {
        andl(block_, l, r);
    }
}

void
BasicBlockGenerator::operator()(Assignment* expression) {
    Expression::Ptr child = expression->expression();

    int prev = variable(expression->identifier()); 
    //if (expression->type()->pointer() && prev) {

        BasicBlock::Ptr pass(new BasicBlock);
        BasicBlock::Ptr fail(new BasicBlock); 
        beqz(block_, prev); 
        block_->branch1(pass);
        block_->branch2(fail);
        pass->branch1(fail);
        
        int addr = add(pass, prev, REFCOUNT_OFFSET);       
        int refc = load(pass, addr);
        int one = li(pass, 1); 
        int result = add(pass, refc, one); 
        store(pass, addr, result);

        block_ = fail;

        // Dealoc if count < 0
    //}

    int r = temp_;
    child(this); 

    assert("Failure: Need to handle pointer case");
}

void
BasicBlockGenerator::operator()(Unary* expression) {
}

void
BasicBlockGenerator::operator()(Call* expression) {
    // Push all the arguments on the stack in order
    int nargs = 0;
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
        push(block_, temp_);
        nargs++;
    } 

    assert("Need to look up the actual function's name");
    // If it's a fn pointer, then don't need to lookup, load the ptr
    // from the local scope.  Could be inside the 'self' object
    call(block_, expression->identifier()); 
    block_->branch1(new BasicBlock);
    block_ = block_->branch1();
}

void
BasicBlockGenerator::operator()(Dispatch* expression) {
}

void
BasicBlockGenerator::operator()(Identifier* expression) {
}

void
BasicBlockGenerator::operator()(Member* expression) {
}

void
BasicBlockGenerator::operator()(Block* statement) {
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
}

void
BasicBlockGenerator::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void
BasicBlockGenerator::operator()(While* statement) {
}

void
BasicBlockGenerator::operator()(For* statement) {
}

void
BasicBlockGenerator::operator()(Conditional* statement) {
}

void
BasicBlockGenerator::operator()(Variable* statement) {
}

void
BasicBlockGenerator::operator()(Return* statement) {
}

void
BasicBlockGenerator::operator()(When* statement) {
}

void
BasicBlockGenerator::operator()(Case* statement) {
}

void
BasicBlockGenerator::operator()(Function* feature) {
}

void
BasicBlockGenerator::operator()(Attribute* feature) {
}

void
BasicBlockGenerator::operator()(Import* feature) {
}

int
BasicBlockGenerator::variable(Name* name) {
    vector<map<Name::Ptr, int> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<Name::Ptr, int>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    int temporary = temp_++;
    variable(name, temporary);
    return temporary;
}

void
BasicBlockGenerator::variable(Name* name, int temporary) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, temporary));
}

void
BasicBlockGenerator::enter_scope() {
    variable_.push_back(map<Name::Ptr, int>());
}

void
BasicBlockGenerator::exit_scope() {
    variable_.pop_back();
}


