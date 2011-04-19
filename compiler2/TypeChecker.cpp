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

#include "TypeChecker.hpp"
#include "Feature.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Unit.hpp"
#include "Location.hpp"
#include <iostream>
#include <cassert>

using namespace std;

TypeChecker::TypeChecker(Environment* environment) :
    environment_(environment) {

    if (environment_->errors()) {
        return;
    }

    for (Unit::Ptr u = environment_->units(); u; u = u->next()) {
        // Clear out old function - name bindings.
        enter_scope();
        function_.clear();
        current_unit_ = u;
        u(this);

        // Iterate though all features and add them to the scope
        for (Feature::Ptr f = u->features(); f; f = f->next()) {
            if (dynamic_cast<Function*>(f.pointer())) {
                Function::Ptr func = static_cast<Function*>(f.pointer());
                function(func->name(), func);
            } else if (dynamic_cast<Attribute*>(f.pointer())) {
                Attribute::Ptr attr = static_cast<Attribute*>(f.pointer());
                variable(attr->name(), attr->type());
            }
        }
        
        // Now compile the features
        for (Feature::Ptr f = u->features(); f; f = f->next()) {
            f(this);
        }
        exit_scope();
    }  
}

void TypeChecker::operator()(Class* unit) {
}

void TypeChecker::operator()(Interface* unit) {
}

void TypeChecker::operator()(Structure* unit) {
}

void TypeChecker::operator()(Module* unit) {
}

void TypeChecker::operator()(StringLiteral* expression) {
    Name* name = environment_->name("String");
    expression->type(new Type(0, name, 0, environment_)); 
}

void TypeChecker::operator()(IntegerLiteral* expression) {
    Name* name = environment_->name("Integer");
    expression->type(new Type(0, name, 0, environment_)); 
}

void TypeChecker::operator()(Assignment* expression) {
    Expression::Ptr storage = expression->storage();
    Expression::Ptr value = expression->expression(); 
    storage(this);
    value(this);
    if (!value->type()->subtype(storage->type())) {
        cerr << expression->location();
        cerr << "Type does not conform in assignment";
        cerr << endl;
    }
}

void TypeChecker::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();

    if (environment_->name("||") == expression->operation()
        || environment_->name("&&") == expression->operation()) {

        if (!environment_->boolean_type()->equals(left->type()) 
            || !environment_->boolean_type()->equals(left->type())) {

            cerr << expression->location();
            cerr << "Operator can only be applied to type ";
            cerr << environment_->boolean_type();
            cerr << endl;
        }
    } else if (environment_->name("?") == expression->operation()) {
        assert("Not implemented");
    } else {
        assert("Not implemented");
    }
}

void TypeChecker::operator()(Unary* expression) {
    Expression::Ptr child = expression->child();
    child(this);

    if (environment_->name("!") == expression->operation()) {
        if (!environment_->boolean_type()->equals(child->type())) {
            cerr << expression->location();
            cerr << "Operator '!' must be applied to type ";
            cerr << environment_->boolean_type();
            cerr << endl;
        }
    } else {
        assert("Unary operator not implemented");
    }
}

void TypeChecker::operator()(Call* expression) {
    // Evaluate types of argument expressions
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Look up the function by name in the current context
    Function::Ptr func;
    if (expression->unit()) {
        // Look up the unit by type name, then look up the function
        assert("Not implemented");
    } else {
        func = function(expression->identifier()); 
    }
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function ";
        cerr << expression->identifier()->string();
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    expression->type(func->type());
    
    // Check argument types versus formal parameter types
    Expression::Ptr arg = expression->arguments(); 
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        if (!arg->type()->subtype(formal->type())) {
            cerr << arg->location();
            cerr << "Argument does not conform to type ";
            cerr << *formal->type();
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function ";
        cerr << expression->identifier()->string();
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function ";
        cerr << expression->identifier()->string();
        cerr << endl;
    }
}

void TypeChecker::operator()(Dispatch* expression) {
    // Evaluate types of argument expressions
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Look up the function by name from the unit
    Expression::Ptr object = expression->arguments();
    Unit::Ptr unit = environment_->unit(object->type()->qualified_name());
    if (!unit) {
        expression->type(environment_->no_type());
        return;
    }

    Function::Ptr func = unit->function(expression->identifier()); 
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function ";
        cerr << expression->identifier()->string();
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    } 
    expression->type(func->type());

    // Check argument types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        if (!arg->type()->subtype(formal->type())) {
            cerr << arg->location();
            cerr << "Argument does not conform to type ";
            cerr << *formal->type();
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function ";
        cerr << expression->identifier()->string();
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function ";
        cerr << expression->identifier()->string();
        cerr << endl;
    }
}

void TypeChecker::operator()(Identifier* expression) {
    Type::Ptr type = variable(expression->identifier());
    if (!type) {
        cerr << expression->location();
        cerr << "Undeclared identifier \"";
        cerr << expression->identifier()->string();
        cerr << "\"";
        cerr << endl;
        expression->type(environment_->no_type());
    } else {
        expression->type(type);
    }
}

void TypeChecker::operator()(Member* expression) {
    Expression::Ptr object = expression->object(); 
    object(this);

    Unit::Ptr unit = environment_->unit(object->type()->qualified_name());
    if (!unit) {
        expression->type(environment_->no_type());
        return;
    }
    if (unit != current_unit_) {
        cerr << expression->location();
        cerr << "Cannot access private attribute ";
        cerr << expression->identifier()->string();
        expression->type(environment_->no_type());
        return;
    } 
    Attribute::Ptr attr = unit->attribute(expression->identifier());
    if (!attr) {
        cerr << expression->location();
        cerr << "Undefined attribute ";
        cerr << expression->identifier()->string();
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    expression->type(attr->type());
}

void TypeChecker::operator()(Block* statement) {
    enter_scope();
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
    exit_scope();
}

void TypeChecker::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void TypeChecker::operator()(While* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    if (!environment_->boolean_type()->equals(guard->type())) {
        cerr << statement->location();
        cerr << "While statement guard expression must have type ";
        cerr << environment_->boolean_type();
        cerr << endl;
    }
    block(this);
}

void TypeChecker::operator()(For* statement) {
    Expression::Ptr expression = statement->expression();
    Statement::Ptr block = statement->block();
    expression(this);
    if (!statement->type()->collection(expression->type())) {
        cerr << statement->location();
        cerr << "For expression is not a conforming collection";
        cerr << endl;
    }
    block(this);
}

void TypeChecker::operator()(Conditional* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (!environment_->boolean_type()->equals(guard->type())) {
        cerr << statement->location();
        cerr << "Conditional guard expression must have type ";
        cerr << environment_->boolean_type();
        cerr << endl;
    } 
    true_branch(this);
    false_branch(this);
}

void TypeChecker::operator()(Variable* statement) {
    Expression::Ptr initializer = statement->initializer();
    initializer(this);
    if (!initializer->type()) {
        initializer->type(statement->type());
    }
    if (!statement->type()->supertype(initializer->type())) {
        cerr << statement->location();
        cerr << "Expression does not conform to type ";
        cerr << *initializer->type();
        cerr << endl;
    } 
    variable(statement->identifier(), statement->type()); 
}

void TypeChecker::operator()(Return* statement) {
    Expression::Ptr expression = statement->expression();
    if (expression) {
        expression(this);
    }
    if (!expression->type()->subtype(current_function_->type())) {
        cerr << statement->location();
        cerr << "Expression does not conform to return type ";
        cerr << *current_function_->type();
        cerr << endl;
    }
}

void TypeChecker::operator()(When* statement) {
    Statement::Ptr block = statement->block();
    block(this);
}

void TypeChecker::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);
    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        b(this);
    }
}

void TypeChecker::operator()(Function* feature) {
    Statement::Ptr block = feature->block();
    current_function_ = feature;
    block(this); 
}

void TypeChecker::operator()(Attribute* feature) {
    Expression::Ptr initializer = feature->initializer();
    initializer(this);
    assert("Fix variable check");
    if (!feature->type()->supertype(initializer->type())) {
        cerr << feature->location();
        cerr << "Expression does not conform to type: ";
        cerr << *initializer->type();
        cerr << endl;
    }
    variable(feature->name(), feature->type());
}


void TypeChecker::operator()(Import* feature) {
}

Type* TypeChecker::variable(Name* name) {
    vector<map<Name::Ptr, Type::Ptr> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<Name::Ptr, Type::Ptr>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return 0;
}

void TypeChecker::variable(Name* name, Type* type) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, type));
}

Function* TypeChecker::function(Name* name) {
    map<Name::Ptr, Function::Ptr>::iterator i = function_.find(name);
    if (i != function_.end()) {
        return i->second;
    } else {
        return 0;
    }
}

void TypeChecker::function(Name* name, Function* function) {
    function_.insert(make_pair(name, function));
}

void TypeChecker::enter_scope() {
    variable_.push_back(map<Name::Ptr, Type::Ptr>());
}

void TypeChecker::exit_scope() {
    variable_.pop_back();
}
