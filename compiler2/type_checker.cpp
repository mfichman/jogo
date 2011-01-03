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
}

void TypeChecker::operator()(IntegerLiteral* expression) {
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

void TypeChecker::operator()(Block* statment) {
}

void TypeChecker::operator()(Simple* statment) {
}

void TypeChecker::operator()(While* statment) {
}

void TypeChecker::operator()(For* statment) {
}

void TypeChecker::operator()(Conditional* statment) {
}

void TypeChecker::operator()(Variable* statment) {
}

void TypeChecker::operator()(Return* statment) {
}

void TypeChecker::operator()(When* statment) {
}

void TypeChecker::operator()(Case* statment) {
}

void TypeChecker::operator()(Function* feature) {
}

void TypeChecker::operator()(Define* feature) {
}

void TypeChecker::operator()(Attribute* feature) {
}

void TypeChecker::operator()(Import* feature) {
}


