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
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    cout << "Dispatch" << endl;
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

void Printer::operator()(Index* expression) {
    indent_level_++; 
    Expression::Ptr object = expression->object();
    Expression::Ptr index = expression->index();
    
    cout << "Index" << endl;
    print_tabs(); cout << "object: ";
    object(this);
    print_tabs(); cout << "index: ";
    index(this);
    
    indent_level_--;
}

void Printer::operator()(Identifier* expression) {
    indent_level_++;
    cout << "Identifier" << endl;
    print_tabs(); cout << "name: ";
    cout << expression->identifier()->string() << endl;
    indent_level_--; 
}

void Printer::operator()(Member* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    cout << "Member" << endl;
    print_tabs(); cout << "name: ";
    cout << expression->identifier()->string() << endl;
    print_tabs(); cout << "expression: ";
    child(this);
    indent_level_--;
}

void Printer::operator()(Empty* empty) {
    cout << "Empty" << endl;
}

void Printer::operator()(Block* statement) {
    indent_level_++;
    Statement::Ptr children = statement->children();
    cout << "Block" << endl;

    int i = 0;
    for (Statement::Ptr c = children; c; c = c->next()) {
        print_tabs(); cout << "child" << i << ": ";
        c(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Simple* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    cout << "Statement" << endl;
    print_tabs(); cout << "expression: ";
    expression(this);
    indent_level_--;
}

void Printer::operator()(While* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    cout << "While" << endl;
    print_tabs(); cout << "guard: ";
    guard(this);
    print_tabs(); cout << "block: ";
    block(this);
    indent_level_--;
}

void Printer::operator()(For* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    Statement::Ptr block = statement->block();
    cout << "For" << endl;
    print_tabs(); cout << "variable: "; 
    cout << statement->variable()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << statement->type()->qualified_name()->string() << endl;
    print_tabs(); cout << "expression: ";
    expression(this);
    print_tabs(); cout << "block: ";
    block(this);
    indent_level_--;
}

void Printer::operator()(Conditional* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    cout << "Conditional" << endl;
    print_tabs(); cout << "guard: ";
    guard(this);
    print_tabs(); cout << "true: ";
    true_branch(this);
    print_tabs(); cout << "false: ";
    false_branch(this);
    indent_level_--;
}

void Printer::operator()(Variable* statement) {
    indent_level_++;
    Expression::Ptr initializer = statement->initializer();
    cout << "Variable" << endl;
    print_tabs(); cout << "name: ";
    cout << statement->identifier()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << statement->type()->qualified_name()->string() << endl;
    print_tabs(); cout << "initializer: ";
    initializer(this);
    indent_level_--; 
}

void Printer::operator()(Return* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    cout << "Return" << endl;
    print_tabs(); cout << "expression: "; 
    expression(this);
    indent_level_--;
}

void Printer::operator()(When* statement) {
    indent_level_++;
    Statement::Ptr block = statement->block();
    cout << "When" << endl;
    print_tabs(); cout << "name: ";
    cout << statement->variable()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << statement->type()->qualified_name()->string() << endl;
    print_tabs(); cout << "block: ";
    block(this);
    indent_level_--; 
}

void Printer::operator()(Case* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    cout << "Case" << endl;
    print_tabs(); cout << "guard: ";
    guard(this);

    int i = 0; 
    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        print_tabs(); cout << "branch" << i << ": ";
        b(this);
    } 
    indent_level_--;
}

void Printer::operator()(Function* feature) {
    indent_level_++;
    Statement::Ptr block = feature->block();
    cout << "Function" << endl;
    print_tabs(); cout << "name: ";
    cout << feature->name()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type()->qualified_name()->string() << endl;
    
    int i = 0;
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        print_tabs(); cout << "formal" << i << ": ";
        f(this);
    }
    print_tabs(); cout << "block: ";
    block(this);
    indent_level_--;
}

void Printer::operator()(Define* feature) {
    indent_level_++;
    cout << "Define" << endl;
    print_tabs(); cout << "name: ";
    cout << feature->name()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type()->qualified_name()->string() << endl;
    indent_level_--; 
}

void Printer::operator()(Attribute* feature) {
    indent_level_++;
    Expression::Ptr initializer = feature->initializer();
    cout << "Attribute" << endl;
    print_tabs(); cout << "name: ";
    cout << feature->name()->string() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type()->qualified_name()->string() << endl;
    print_tabs(); cout << "initializer: ";
    initializer(this);
    indent_level_--;
}

void Printer::operator()(Import* feature) {
    indent_level_++;
    cout << "Import" << endl;
    print_tabs(); cout << "file: ";
    cout << feature->file() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type()->qualified_name()->string() << endl;
    indent_level_--;
}

