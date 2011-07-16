/*
 * Copyright (c) 2011 Matt Fichman
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

#include "BasicBlockGenerator.hpp"
#include <cassert>

#define REFCOUNT_OFFSET 0

using namespace std;

BasicBlockGenerator::BasicBlockGenerator(Environment* env) :
    environment_(env),
    label_(0) {

    if (environment_->errors()) {
        return;
    }
    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void BasicBlockGenerator::operator()(Class* feature) {
    class_ = feature;
    enter_scope();
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockGenerator::operator()(Formal* formal) {
}

void BasicBlockGenerator::operator()(StringLiteral* expression) {
    // Load a pointer to the string from the string table
    //assert(!"Not implemented");
    return_ = str(block_, expression->value());
}

void BasicBlockGenerator::operator()(IntegerLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = li(block_, expression->value());
}

void BasicBlockGenerator::operator()(FloatLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = li(block_, expression->value());
}

void BasicBlockGenerator::operator()(BooleanLiteral* expression) {
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
    } else {
        assert(!"Unsupported binary operation");
    }
}

void BasicBlockGenerator::operator()(Unary* expression) {
    // Emit the code for the expression, then perform the operation on the
    // operand and return the result.
    Operand op = emit(expression->child());

    if (environment_->name("not")) {
        return_ = notl(block_, op); 
    } else {
        assert(!"Unsupported unary operation");
    }
}

void BasicBlockGenerator::operator()(Call* expression) {
    // Push objects in anticipation of the call instruction.
    std::vector<Operand> args;
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }
    for (size_t i = 0; i < args.size(); i++) {
        push(block_, args[i]);
    }

    // Look up the function by name in the current context.
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope();
    String::Ptr name = expression->file()->function(scope, id)->name();

    // Insert a call expression, then pop the return value off the stack.
    call(block_, name);
    return_ = pop(block_);
}

void BasicBlockGenerator::operator()(Dispatch* expression) {
}

void BasicBlockGenerator::operator()(Construct* expression) {
}

void BasicBlockGenerator::operator()(Identifier* expression) {
    // Simply look up the value of the variable as stored previously.
    return_ = variable(expression->identifier());
}

void BasicBlockGenerator::operator()(Empty* expression) {
    // Do nothing
    return_ = li(block_, environment_->integer("0"));
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

void BasicBlockGenerator::operator()(Let* statement) {
    // Enter a new scope, then emit code for initializing all the let
    // variables, and initialize code for the body.
    enter_scope();
    for (Statement::Ptr v = statement->variables(); v; v = v->next()) {
        emit(v);
    } 
    emit(statement->block());
    exit_scope();
}

void BasicBlockGenerator::operator()(While* statement) {
    // Emit the guard expression in a new basic block
    BasicBlock::Ptr test = new BasicBlock;
    test->label(environment_->string("l" + stringify(++label_)));
    block_->next(test);
    block_ = test;
    Operand guard = emit(statement->guard());
    BasicBlock::Ptr pre = bneqz(block_, guard);
    
    // Now generate the loop (true) block.
    block_ = pre->next();
    emit(statement->block());
    
    // Emit jump back up to the test condition
    block_->instr(JUMP, 0, 0, 0);
    block_->branch(test);

    // Now set the pass-through block and continue code with the following
    // statement.
    block_ = pre->branch();
}

void BasicBlockGenerator::operator()(Conditional* statement) {
    Operand guard = emit(statement->guard());
    BasicBlock::Ptr pre = bneqz(block_, guard);

    // Emit the fall-through (true) block here.  Switch the insert block to
    // the 'next' block.
    block_ = pre->next();
    emit(statement->true_branch());
    BasicBlock::Ptr t = block_;

    // Emit the branch (false) block here.  Switch the insert block to the
    // 'branch' block.
    block_ = pre->branch();
    emit(statement->false_branch());
    BasicBlock::Ptr f = block_;

    // Jump to the post block (after the conditional) from the branch block,
    // and fall-through from the true block to the post block.
    if (f->terminated()) {
        t->next(new BasicBlock);
        t->label(environment_->name("l" + stringify(++label_)));
    } else {
        block_ = jump(block_);
        t->next(block_);
    }
}

void BasicBlockGenerator::operator()(Variable* statement) {
    // FixMe: This breaks SSA form, because variables are not renamed.  With
    // this code the way it is, the generator only generates SSA for temporary
    // expressions.  This limitation is here as a punt on introducing
    // phi-functions until optimizations are needed, since without
    // optimizations, SSA is not needed anyway.


    Operand value;
    if (dynamic_cast<Empty*>(statement->initializer())) {
        value = environment_->integer("0");
    } else {
        value = emit(statement->initializer());
    }

    Operand var = variable(statement->identifier());
    if (!var.temporary()) {
        var = ++temp_;
        variable(statement->identifier(), var);
    }

    block_->instr(MOV, var, value, 0);
}

void BasicBlockGenerator::operator()(Return* statement) {
    if (!dynamic_cast<Empty*>(statement->expression())) {
        Operand value = emit(statement->expression());
        push(block_, value);
    }
    ret(block_);
}

void BasicBlockGenerator::operator()(When* statement) {
}

void BasicBlockGenerator::operator()(Case* statement) {
}

void BasicBlockGenerator::operator()(Fork* statement) {
}

void BasicBlockGenerator::operator()(Yield* statament) {
}

void BasicBlockGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) {
        return;
    }

    // Reset the temporaries for the function.
    temp_ = Operand();

    std::string name = module_->name()->string();
    if (class_) {
        if (!name.empty()) {
            name += "::";
        }
        name += class_->name()->string();
    }
    if (!name.empty()) {
        name += "::";
    }
    name += feature->name()->string();

    block_ = feature->code();
    block_->label(environment_->name(name));
    enter_scope();

    // Pop the formal parameters off the stack in reverse order, and save them
    // to a temporary.
    std::vector<Formal::Ptr> formals;
    for (Formal* f = feature->formals(); f; f = f->next()) {
        formals.push_back(f);
    } 
    while (!formals.empty()) {
        variable(formals.back()->name(), pop(block_));
        formals.pop_back();
    }
    
    // Generate code for the body of the function.
    emit(feature->block());


    if (feature->type()->equals(environment_->void_type())) {
        ret(block_); 
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Attribute* feature) {
    // Pass, handled by constructor
}

void BasicBlockGenerator::operator()(Import* feature) {
    // Pass
}

void BasicBlockGenerator::operator()(Type* feature) {
    // Pass
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


