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
#include "Location.hpp"
#include <iostream>
#include <cassert>

using namespace std;

TypeChecker::TypeChecker(Environment* environment) :
    environment_(environment) {

    if (environment_->errors()) {
        return;
    }

    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void TypeChecker::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void TypeChecker::operator()(Class* feature) {
    class_ = feature;
    enter_scope();

    // Check interface/struct/object baseclass and make sure that 
    // disallowed things aren't included.
    for (Type::Ptr m = feature->mixins(); m; m = m->next()) {
        if (m->clazz()) {
            if (feature->is_interface() && !m->clazz()->is_interface()) {
                cerr << m->location();
                cerr << "Mix-in for interface '" << feature->name();
                cerr << "' must be an interface" << endl;
                break;
            } else if (feature->is_object() && m->clazz()->is_interface()) {
                cerr << m->location();
                cerr << "Mix-in for object type '" << feature->name();
                cerr << "' cannot be an interface" << endl; 
                break;
            } else if (feature->is_value() && !m->clazz()->is_value()) {
                cerr << m->location();
                cerr << "Mix-in for value type '" << feature->name();
                cerr << "' must be a value type" << endl; 
                break;
            }
         
        }
    }

    if (feature->is_object() && feature->is_interface()) {
        cerr << feature->location();
        cerr << "Class has both object and interface mixins";
        cerr << endl;    
    } else if (feature->is_interface() && feature->is_value()) {
        cerr << feature->location();
        cerr << "Class has both interfaces and value mixins";
        cerr << endl;
    } else if (feature->is_value() && feature->is_object()) {
        cerr << feature->location();
        cerr << "Class has both value and object mixins";
        cerr << endl;
    }

    // Iterate through all the features and add the functions and variables to
    // the current scope.
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            function(func->name(), func);
            continue;
        }
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            variable(attr->name(), attr->type());
            continue;
        }
    }

    // Now type-check all the sub-features.
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this); // lol
    }

    exit_scope();
    function_.clear();
}

void TypeChecker::operator()(Formal* formal) {
    Type::Ptr type = formal->type();
    type(this);
}

void TypeChecker::operator()(StringLiteral* expression) {
    expression->type(environment_->string_type()); 
}

void TypeChecker::operator()(IntegerLiteral* expression) {
    expression->type(environment_->integer_type()); 
}

void TypeChecker::operator()(BooleanLiteral* expression) {
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Let* expression) {
    Statement::Ptr block = expression->block();
    for (Statement::Ptr v = expression->variables(); v; v = v->next()) {
        v(this);
    }
    block(this);
}

void TypeChecker::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    left(this);
    right(this);

    if (environment_->name("or") == expression->operation()
        || environment_->name("and") == expression->operation()) {

        if (left->type()->clazz()->is_value()) {
            cerr << left->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
        if (right->type()->clazz()->is_value()) {
            cerr << right->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Unary* expression) {
    Expression::Ptr child = expression->child();
    child(this);

    if (environment_->name("not") == expression->operation()) {
        if (child->type()->clazz()->is_value()) {
            cerr << expression->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Call* expression) {
    // Evaluate types of argument expressions
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Look up the function by name in the current context
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->module();
    
    Function::Ptr func;
    if (expression->unit()) {
        func = expression->unit()->function(scope, id);  
    } else {
        func = module_->function(scope, id);
    }
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function '";
        cerr << expression->identifier() << "'";
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
            cerr << formal->type();
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function ";
        cerr << expression->identifier();
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function ";
        cerr << expression->identifier();
        cerr << endl;
    }
}

void TypeChecker::operator()(Dispatch* expression) {

    // Evaluate types of argument expressions and the receiver
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Get the class associated with the receiver (always the first argument)
    Expression::Ptr receiver = expression->arguments();
    if (receiver->type() == environment_->no_type()) {
        expression->type(environment_->no_type());
        return;
    }
    Class::Ptr clazz = receiver->type()->clazz();
    if (!clazz) {
        cerr << expression->location();
        cerr << "Undefined class '";
        cerr << receiver->type() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    
    // Look up the function
    Function::Ptr func = clazz->function(expression->identifier()); 
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    } 
    expression->type(func->type());

    // Check argument types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        Type::Ptr ft = formal->type();
        if (formal->type() == environment_->self_type()) {
            ft = receiver->type();
        }
        if (!arg->type()->subtype(ft)) {
            cerr << arg->location();
            cerr << "Argument does not conform to type '";
            cerr << formal->type() << "'";
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(Construct* expression) {
    // Evaluate type of argument expression
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this); 
    }

    // Look up constructor class
    Class::Ptr clazz = expression->type()->clazz(); 
    if (!clazz) {
        cerr << expression->location();
        cerr << "Undefined class '";
        cerr << expression->type() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    if (clazz->is_interface()) {
        cerr << expression->location();
        cerr << "Constructor called for interface type '";
        cerr << clazz->name() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }

    // Look up the constructor
    Function::Ptr constr = clazz->function(environment_->name("@init"));

    // Check arguments types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = constr ? constr->formals() : 0;
    while (arg && formal) {
        if (!arg->type()->subtype(formal->type())) {
            cerr << arg->location();
            cerr << "Argument does not conform to type '";
            cerr << formal->type() << "'";
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    } 
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to constructor '";
        cerr << expression->type() << "'";
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to constructor '";
        cerr << expression->type() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(Identifier* expression) {
    Type::Ptr type = variable(expression->identifier());
    if (!type) {
        cerr << expression->location();
        cerr << "Undeclared identifier \"";
        cerr << expression->identifier();
        cerr << "\"";
        cerr << endl;
        expression->type(environment_->no_type());
    } else {
        expression->type(type);
    }
}

void TypeChecker::operator()(Empty* expression) {
    expression->type(environment_->no_type());
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
    Type* t = guard->type();
    assert(t);
    if (!environment_->boolean_type()->equals(guard->type())) {
        cerr << statement->location();
        cerr << "While statement guard expression must have type '";
        cerr << environment_->boolean_type() << "'";
        cerr << endl;
    }
    block(this);
}

void TypeChecker::operator()(For* statement) {
    Expression::Ptr expression = statement->expression();
    Statement::Ptr block = statement->block();
    expression(this);
    block(this);
}

void TypeChecker::operator()(Conditional* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (guard->type()->clazz()->is_value()) {
        cerr << guard->location();
        cerr << "Value types cannot be converted to 'Bool'";
        cerr << endl;
    }
    true_branch(this);
    false_branch(this);
}

void TypeChecker::operator()(Variable* statement) {
    Expression::Ptr initializer = statement->initializer();
    initializer(this);

    if (!statement->type()->equals(environment_->no_type())) {
        Type::Ptr type = statement->type();
        type(this);
        if (!type->clazz()) {
            variable(statement->identifier(), environment_->no_type());
            return;
        }
    }

    Type::Ptr type = variable(statement->identifier());

    if (type && !statement->type()->equals(environment_->no_type())) {
        cerr << statement->location();
        cerr << "Duplicate definition of variable '";
        cerr << statement->identifier() << "'";
        cerr << endl;
        return;
    }
    if (type && !initializer->type()->subtype(type)) {
        cerr << initializer->location();
        cerr << "Expression does not conform to type '";
        cerr << type << "'";
        cerr << " in assignment of '";
        cerr << statement->identifier() << "'";
        cerr << endl;
        return;
    }
    if (!initializer->type()->subtype(statement->type())) {
        cerr << initializer->location();
        cerr << "Expression does not conform to type '";
        cerr << statement->type() << "'";
        cerr << endl;
        return;
    }
    if (initializer->type()->equals(environment_->no_type())) {
        variable(statement->identifier(), statement->type()); 
    } else if (initializer->type()->equals(environment_->void_type())) {
        cerr << initializer->location();
        cerr << "Void value assigned to variable '";
        cerr << statement->identifier() << "'";
        cerr << endl;
        variable(statement->identifier(), environment_->no_type());
    } else if (!statement->type()->equals(environment_->no_type())) {
        variable(statement->identifier(), statement->type());
    } else {
        variable(statement->identifier(), initializer->type()); 
    }
}

void TypeChecker::operator()(Return* statement) {
    Expression::Ptr expression = statement->expression();
    if (expression) {
        expression(this);
    }
    if (!expression->type()->subtype(scope_->type())) {
        cerr << statement->location();
        cerr << "Expression does not conform to return type '";
        cerr << scope_->type() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(When* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    block(this);
}

void TypeChecker::operator()(Fork* statement) {
    assert("Not supported");
}

void TypeChecker::operator()(Yield* statement) {
    assert("Not supported");
}

void TypeChecker::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);

    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        b(this);
        When::Ptr when = static_cast<When*>(b.pointer());
        if (!guard->type()->equals(when->guard()->type())) {
            cerr << statement->location();
            cerr << "Case expression does not conform to type '";
            cerr << guard->type() << "'";
            cerr << endl;
        }
    }
}

void TypeChecker::operator()(Function* feature) {
    Statement::Ptr block = feature->block();
    scope_ = feature;
    
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        Type::Ptr type = f->type();
        type(this);
        variable(f->name(), f->type());
    }
    
    Type::Ptr type = feature->type();
    type(this);

    if (block) {
        if (class_ && class_->is_interface()) {
            cerr << block->location();
            cerr << "Interface method '" << feature->name();
            cerr << "' has a body" << endl;
        }
        block(this); 
    } else if (!class_) {
        cerr << feature->location();
        cerr << "Function '" << feature->name() << "' has no body" << endl;
    } else if (!class_->is_interface()) {
        cerr << feature->location();
        cerr << "Method '" << feature->name() << "' has no body" << endl;
    }
}

void TypeChecker::operator()(Attribute* feature) {
    Expression::Ptr initializer = feature->initializer();
    initializer(this);

    if (!initializer->type()->subtype(feature->type())) {
        cerr << feature->location();
        cerr << "Expression does not conform to type '";
        cerr << initializer->type() << "'";
        cerr << endl;
    }
    variable(feature->name(), feature->type());
}


void TypeChecker::operator()(Import* feature) {
}

void TypeChecker::operator()(Type* type) {
    if (type == environment_->self_type()) {
        return;
    }
    if (type == environment_->void_type()) {
        return;
    }

    if (!type->clazz()) {
        cerr << type->location();
        cerr << "Undefined type '" << type << "'";
        cerr << endl;
        return;
    }

    for (Generic::Ptr g = type->generics(); g; g = g->next()) {
        if (!g->type()->clazz() && !type->equals(environment_->self_type())) {

            cerr << type->location() << endl;
            cerr << "Undefined type '" << type << "'";
            cerr << endl;
        }
    }
}

Type* TypeChecker::variable(String* name) {
    vector<map<String::Ptr, Type::Ptr> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<String::Ptr, Type::Ptr>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return 0;
}

void TypeChecker::variable(String* name, Type* type) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, type));
}

Function* TypeChecker::function(String* name) {
    map<String::Ptr, Function::Ptr>::iterator i = function_.find(name);
    if (i != function_.end()) {
        return i->second;
    } else {
        return 0;
    }
}

void TypeChecker::function(String* name, Function* function) {
    function_.insert(make_pair(name, function));
}

void TypeChecker::enter_scope() {
    variable_.push_back(map<String::Ptr, Type::Ptr>());
}

void TypeChecker::exit_scope() {
    variable_.pop_back();
}

