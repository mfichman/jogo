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

#include "ClosureAnalyzer.hpp"
#include <algorithm>
#include <iterator>

using namespace std;

ClosureAnalyzer::ClosureAnalyzer(Environment* env) :
    env_(env) {
}

void ClosureAnalyzer::operator()(Function* feature) {
    enter_scope();
    Block::Ptr block = feature->block();
    
    // Add all formals, as they are basically declared local variables.
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        variable(f->name(), true);
    }  

    block(this);   
    exit_scope();

    std::set<String::Ptr>::iterator begin = unbound_var_set_.begin();
    std::set<String::Ptr>::iterator end = unbound_var_set_.end();
    copy(begin, end, std::back_inserter(unbound_var_));
}

void ClosureAnalyzer::operator()(HashLiteral* expression) {
    assert(!"Not implemented");
}

void ClosureAnalyzer::operator()(ArrayLiteral* expression) {
    assert(!"Not implemented");
}

void ClosureAnalyzer::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    left(this);
    right(this); 
}

void ClosureAnalyzer::operator()(Unary* expression) {
    Expression::Ptr expr = expression->child();
    expr(this);
}

void ClosureAnalyzer::operator()(Call* expression) {
    // Simply check the arguments recursively.
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }
}

void ClosureAnalyzer::operator()(Construct* expression) {
    // Check arguments recursively.
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }
}

void ClosureAnalyzer::operator()(Identifier* expression) {
    // Identifier usage; could be referencing a local var or a var outside the
    // closure, depending.
    String::Ptr id = expression->identifier();
    if (!variable(id)) {
        unbound_var_set_.insert(id);
    }
}

void ClosureAnalyzer::operator()(Block* statement) {
    enter_scope();
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
    exit_scope();
}

void ClosureAnalyzer::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void ClosureAnalyzer::operator()(Member* expression) {
	Expression::Ptr child = expression->expression();
	child(this);
}

void ClosureAnalyzer::operator()(Let* expression) {
    enter_scope();
    Statement::Ptr block = expression->block();
    for (Expression::Ptr v = expression->variables(); v; v = v->next()) {
        v(this);
    }
    block(this);
    exit_scope(); 
}

void ClosureAnalyzer::operator()(While* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    block(this);
}

void ClosureAnalyzer::operator()(Conditional* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    true_branch(this);
    if (false_branch) {
        false_branch(this);
    }
}

void ClosureAnalyzer::operator()(Assignment* statement) {
    // An assignment might be either a new local variable, or an assignment to
    // an existing var in the enclosing scope.  Thus, we have to mark it 
    // differently from variable uses.
    Expression::Ptr init = statement->initializer();
    String::Ptr id = statement->identifier();
    if (init) {
        init(this);
    }
    if (variable(id) && id->string()[0] != '_') {
        unbound_var_set_.insert(id); 
    }
}

void ClosureAnalyzer::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    expr(this);
}

void ClosureAnalyzer::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);
    for (Statement::Ptr c = statement->children(); c; c = c->next()) {
        c(this);
    }
}

void ClosureAnalyzer::operator()(Match* statement) {
    // Recursively check the guard expression and all of the child statements.
    Expression::Ptr guard = statement->guard();
    guard(this);
    
    for (Statement::Ptr b = statement->cases(); b; b = b->next()) {
        b(this);
    }
}

void ClosureAnalyzer::operator()(Fork* statement) {
    assert(!"Not supported");
}

void ClosureAnalyzer::operator()(Yield* statement) {
}

void ClosureAnalyzer::enter_scope() {
    variable_.push_back(std::set<String::Ptr>());
}

void ClosureAnalyzer::exit_scope() {
    variable_.pop_back();
}

void ClosureAnalyzer::variable(String* var, bool set) {
    // Add a new variable definition to the current scope.
    if (set) {
        variable_.back().insert(var); 
    } else {
        variable_.back().erase(var);
    }
}

bool ClosureAnalyzer::variable(String* var) {
    // Check the usage of variable 'var'.  If the variable is defined in an
    // enclosing scope, then do nothing.  Otherwise, add it to the collection 
    // of unbound variables.
     
    vector<set<String::Ptr> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        if (i->find(var) != i->end()) {
            return true;
        }
    }
    return false;
}
