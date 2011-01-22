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

#include "Printer.hpp"
#include <iostream>

using namespace std;

Printer::Printer(Environment* environment) :
    environment_(environment),
    indent_level_(0) {

    int i = 0;
    for (Unit::Ptr u = environment_->units(); u; u = u->next()) {
        cout << "unit" << i << ": ";
        u(this);
        i++;
    }  
}

void Printer::print_tabs() {
    for (int i = 0; i < indent_level_;  i++) {
        cout << "  ";
    }
}

void Printer::operator()(Class* unit) {
    indent_level_++; 
    cout << "Class" << endl;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    int i = 0;
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Interface* unit) {
    indent_level_++; 
    cout << "Interface" << endl;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    int i = 0;
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Structure* unit) {
    indent_level_++; 
    cout << "Structure" << endl;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    int i = 0;
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Module* unit) {
    indent_level_++; 
    cout << "Module" << endl;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    int i = 0;
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(StringLiteral* expression) {
    indent_level_++;
    cout << "StringLiteral" << endl;
    print_tabs(); cout << "value: ";
    cout << expression->value()->string() << endl;
    indent_level_--;
}

void Printer::operator()(IntegerLiteral* expression) {
    indent_level_++;
    cout << "IntegerLiteral(";
    cout << expression->value()->string() << ")" << endl;
    indent_level_--;
}

void Printer::operator()(Binary* expression) {
    indent_level_++;
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();

    cout << "Binary" << endl;
    print_tabs(); cout << "operation: ";
    cout << expression->operation()->string() << endl;
    print_tabs(); cout << "left: ";
    left(this);
    print_tabs(); cout << "right: ";
    right(this);
    indent_level_--;
}

void Printer::operator()(Assignment* expression) {
    indent_level_++;
    Expression::Ptr storage = expression->storage();
    Expression::Ptr right = expression->expression();
    cout << "Assignment" << endl;
    print_tabs(); cout << "storage:";
    storage(this);
    print_tabs(); cout << "expression:";
    right(this);
    indent_level_--;
}

void Printer::operator()(Unary* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    cout << "Unary" << endl;
    print_tabs(); cout << "child: ";
    child(this);
    print_tabs(); cout << "operation: ";
    cout << expression->operation()->string() << endl;
    indent_level_--;
}

void Printer::operator()(Call* expression) {
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    cout << "Call" << endl;
    print_tabs(); cout << "name: ";
    cout << expression->identifier()->string() << endl;

    int i = 0;
    for (Expression::Ptr a = arguments; a; a = a->next()) {
        print_tabs(); cout << "argument" << i << ": ";
        a(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Dispatch* expression) {
}

void Printer::operator()(Index* expression) {
}

void Printer::operator()(Identifier* expression) {
}

void Printer::operator()(Member* expression) {
}

void Printer::operator()(Block* statment) {
}

void Printer::operator()(Simple* statment) {
}

void Printer::operator()(While* statment) {
}

void Printer::operator()(For* statment) {
}

void Printer::operator()(Conditional* statment) {
}

void Printer::operator()(Variable* statment) {
}

void Printer::operator()(Return* statment) {
}

void Printer::operator()(When* statment) {
}

void Printer::operator()(Case* statment) {
}

void Printer::operator()(Function* feature) {
}

void Printer::operator()(Define* feature) {
}

void Printer::operator()(Attribute* feature) {
    indent_level_++;
    Expression::Ptr initializer = feature->initializer();
    cout << "Attribute" << endl;
    print_tabs(); cout << "name: " << feature->name()->string() << endl;

    if (initializer) {
        print_tabs(); cout << "initializer: ";
        initializer(this);
    }
    indent_level_--;
}

void Printer::operator()(Import* feature) {
}

