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
#include <iostream>
#include <cassert>

std::ostream& operator<<(std::ostream& out, const Location& location) {
    out << location.first_line << ":" << std::endl;
    out << location.first_column << ": " << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, Type::Ptr type) {
    out << type->qualified_name()->string();
    if (type->generics()) {
        out << '[';
        for (Generic::Ptr g = type->generics(); g; g = g->next()) {
            out << g->type();
            if (g->next()) {
                out << ',';
            }
        }
        out << ']';
    } 
    return out;
}

TypeChecker::TypeChecker(Environment* environment) :
    environment_(environment) {

    for (Unit::Ptr u = environment_->units(); u; u = u->next()) {
        u(this);
    }  
}

void TypeChecker::operator()(Class* unit) {
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }
}

void TypeChecker::operator()(Interface* unit) {
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }
}

void TypeChecker::operator()(Structure* unit) {
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }
}

void TypeChecker::operator()(Module* unit) {
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }
}

void TypeChecker::operator()(StringLiteral* expression) {
    expression->type(new Type(0, environment_->name("String"), 0, environment_)); 
}

void TypeChecker::operator()(IntegerLiteral* expression) {
    expression->type(new Type(0, environment_->name("Integer"), 0, environment_)); 
}

void TypeChecker::operator()(Assignment* expression) {
    Expression::Ptr storage = expression->storage();
    Expression::Ptr value = expression->expression(); 
    storage(this);
    value(this);
    if (!value->type()->subtype(storage->type())) {
        std::cerr << expression->location();
        std::cerr << "Type does not conform in assignment";
        std::cerr << std::endl;
    }
}

void TypeChecker::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();

    if (environment_->name("||") == expression->operation()
        || environment_->name("&&") == expression->operation()) {

        if (!environment_->boolean_type()->equals(left->type()) 
            || !environment_->boolean_type()->equals(left->type())) {

            std::cerr << expression->location();
            std::cerr << "Operator can only be applied to type Boolean";
            std::cerr << std::endl;
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
            std::cerr << expression->location();
            std::cerr << "Operator '!' must be applied to a Boolean";
            std::cerr << std::endl;
        }
    } else {
        assert("Unary operator not implemented");
    }
        
    // TODO: Check expression
}

void TypeChecker::operator()(Call* expression) {
    // TODO: Check expression
}

void TypeChecker::operator()(Dispatch* expression) {
    // TODO: Check expression
}

void TypeChecker::operator()(Index* expression) {
    // TODO: Check expression
}

void TypeChecker::operator()(Identifier* expression) {
    Type::Ptr type = variable_type(expression->identifier());
    if (!type) {
        std::cerr << expression->location();
        std::cerr << "Undeclared identifier \"";
        std::cerr << expression->identifier()->string();
        std::cerr << "\"";
        std::cerr << std::endl;
        expression->type(environment_->no_type());
    } else {
        expression->type(type);
    }
}

void TypeChecker::operator()(Member* expression) {
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
        std::cerr << statement->location();
        std::cerr << "While statement guard expression must have type Boolean";
        std::cerr << std::endl;
    }
    block(this);
}

void TypeChecker::operator()(For* statement) {
    Expression::Ptr expression = statement->expression();
    Statement::Ptr block = statement->block();
    expression(this);
    if (!statement->type()->collection(expression->type())) {
        std::cerr << statement->location();
        std::cerr << "For expression is not a conforming collection";
        std::cerr << std::endl;
    }
    block(this);
}

void TypeChecker::operator()(Conditional* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (!environment_->boolean_type()->equals(guard->type())) {
        std::cerr << statement->location();
        std::cerr << "Conditional guard expression must have type Boolean";
        std::cerr << std::endl;
    } 
    true_branch(this);
    false_branch(this);
}

void TypeChecker::operator()(Variable* statement) {
    Expression::Ptr initializer = statement->initializer();
    initializer(this);
    if (!statement->type()->supertype(initializer->type())) {
        std::cerr << statement->location();
        std::cerr << "Expression does not conform to type ..." << std::endl;
        std::cerr << std::endl;
    } 
    variable_type(statement->identifier(), statement->type()); 
}

void TypeChecker::operator()(Return* statement) {
    Expression::Ptr expression = statement->expression();
    if (expression) {
        expression(this);
    } 
    assert("Check return value of function");
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
    block(this); 
}

void TypeChecker::operator()(Define* feature) {
}

void TypeChecker::operator()(Attribute* feature) {
    Expression::Ptr initializer = feature->initializer();
    initializer(this);
    assert("Fix variable check");
    if (!feature->type()->supertype(initializer->type())) {
        std::cerr << feature->location() << std::endl;
        std::cerr << "Expression does not conform to type ..." << std::endl;
        std::cerr << std::endl;
    }
    variable_type(feature->name(), feature->type());
}


void TypeChecker::operator()(Import* feature) {
}

Type* TypeChecker::variable_type(Name* name) {
    std::vector<std::map<Name::Ptr, Type::Ptr> >::reverse_iterator i;
    for (i = variable_type_.rbegin(); i != variable_type_.rend(); i++) {
        std::map<Name::Ptr, Type::Ptr>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return 0;
}

void TypeChecker::variable_type(Name* name, Type* type) {
    assert(variable_type_.size());
    variable_type_.back().insert(std::make_pair(name, type));
}

void TypeChecker::enter_scope() {
    variable_type_.push_back(std::map<Name::Ptr, Type::Ptr>());
}

void TypeChecker::exit_scope() {
    variable_type_.pop_back();
}

