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

#include "type_checker.hpp"
#include "feature.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "unit.hpp"
#include <iostream>
#include <cassert>

std::ostream& operator<<(std::ostream& out, const Location& location) {
    out << location.first_line << ":" << std::endl;
    out << location.first_column << ": " << std::endl;
    return out;
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

void TypeChecker::operator()(Binary* expression) {
}

void TypeChecker::operator()(Unary* expression) {
}

void TypeChecker::operator()(Call* expression) {
}

void TypeChecker::operator()(Dispatch* expression) {
}

void TypeChecker::operator()(Index* expression) {
}

void TypeChecker::operator()(Identifier* expression) {
}

void TypeChecker::operator()(Member* expression) {
}

void TypeChecker::operator()(Block* statement) {
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
}

void TypeChecker::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void TypeChecker::operator()(While* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    if (!boolean_type_->equals(guard->type())) {
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
    assert("statement->type() is a collection of expression->type()");
    block(this);
}

void TypeChecker::operator()(Conditional* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (!boolean_type_->equals(guard->type())) {
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
    assert("Fix variable check");
    if (!statement->type()->supertype(initializer->type())) {
        std::cerr << statement->location();
        std::cerr << "Expression does not conform to type ..." << std::endl;
        std::cerr << std::endl;
    } 
    assert("Add variable to symbol table for function");
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
    assert("Add variable to symbol table for class");
}


void TypeChecker::operator()(Import* feature) {
}


