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

#include "printer.hpp"
#include <iostream>

using namespace std;

Printer::Printer(Environment* environment) :
    environment_(environment),
    indent_level_(0) {

    for (Unit::Ptr u = environment_->units(); u; u = u->next()) {
        u(this);
    }  
}

void Printer::print_tabs() {
    for (int i = 0; i < indent_level_;  i++) {
        cout << "  ";
    }
}

void Printer::operator()(Class* unit) {
    print_tabs(); cout << "class:" << endl;
    indent_level_++;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    print_tabs(); cout << "features: " << endl;
    indent_level_++;

    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }

    indent_level_--;
    indent_level_--;
}

void Printer::operator()(Interface* unit) {
    print_tabs(); cout << "interface:" << endl;
    indent_level_++;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    print_tabs(); cout << "features: " << endl;
    indent_level_++;

    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }

    indent_level_--;
    indent_level_--;
}

void Printer::operator()(Structure* unit) {
    print_tabs(); cout << "structure:" << endl;
    indent_level_++;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    print_tabs(); cout << "features: " << endl;
    indent_level_++;

    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }

    indent_level_--;
    indent_level_--;
}

void Printer::operator()(Module* unit) {
    print_tabs(); cout << "module:" << endl;
    indent_level_++;
    print_tabs(); cout << "name: " << unit->name()->string() << endl;
    print_tabs(); cout << "features: " << endl;
    indent_level_++;

    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }

    indent_level_--;
    indent_level_--;
}

void Printer::operator()(StringLiteral* expression) {
    print_tabs(); cout << "string: " << expression->value()->string() << endl;
}

void Printer::operator()(IntegerLiteral* expression) {
    print_tabs(); cout << "integer: " << expression->value()->string() << endl;
}

void Printer::operator()(Binary* expression) {
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();

    print_tabs(); cout << "binary:";
    indent_level_++;
    print_tabs(); cout << "operation: ";
    cout << expression->operation()->string() << endl;
    print_tabs(); cout << "left: ";
    left(this);
    print_tabs(); cout << "right: ";
    right(this);
}

void Printer::operator()(Assignment* expression) {
}

void Printer::operator()(Unary* expression) {
}

void Printer::operator()(Call* expression) {
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
    Expression::Ptr initializer = feature->initializer();
    print_tabs(); cout << "attribute: " << endl;
    indent_level_++;
    print_tabs(); cout << "name: " << feature->name()->string() << endl;

    if (initializer) {
        print_tabs(); cout << "initializer: " << endl;
        indent_level_++;
        initializer(this);
        indent_level_--;
    
    }
    
    indent_level_--;
}

void Printer::operator()(Import* feature) {
}

