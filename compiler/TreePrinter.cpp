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

#include "TreePrinter.hpp"
#include <iostream>

using namespace std;

Printer::Printer(Environment* environment) :
    environment_(environment),
    indent_level_(0) {

    if (environment_->errors()) {
        return;
    }

    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void Printer::print_tabs() {
    for (int i = 0; i < indent_level_;  i++) {
        cout << "  ";
    }
}

void Printer::print_comment(String* comment) {
    if (!comment || comment->string().empty()) {
        return;
    }
    indent_level_++;

    std::string str = comment->string();
    const char* begin = str.c_str();
    const char* end = str.c_str();
    int counter = 2 * indent_level_;
    for (const char* c = str.c_str(); *c; c++) {
        counter++;
        if (counter > 79 || *c == '\n') {
            print_tabs();

            if (end <= begin) {
                // Word is longer than the maximum length of the line, so
                // break the word in half
                while (*begin && counter > 2 * indent_level_) {
                    std::cout << *begin++;
                    counter--;
                }
            } else {
                // Word is shorter than the length of the line, so print to
                // the last space.
                while (begin <= end && *begin) {
                    std::cout << *begin++;
                }
            }
            std::cout << std::endl;
            counter = 2 * indent_level_;
            while (isspace(*begin)) {
                begin++;
            }
            counter += c - begin + 1;
        } 
        if (!isspace(*c) && isspace(*(c + 1))) {
            end = c;
        }
    }
    print_tabs();
    while (*begin) {
        std::cout << *begin++;
    }
    std::cout << std::endl;

    indent_level_--;
}

void Printer::operator()(Module* feature) {
    indent_level_++;
    cout << "Module" << endl;
    print_tabs(); cout << "name: " << feature->name() << endl;
    int i = 0;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Class* feature) {
    indent_level_++; 
    cout << "Class" << endl;
    print_tabs(); cout << "name: " << feature->name() << endl;
    print_tabs(); cout << "comment:" << endl;
    print_comment(feature->comment());
    int i = 0;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        print_tabs(); cout << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Formal* formal) {
    indent_level_++;
    cout << "Formal" << endl;
    print_tabs(); cout << "name: ";
    cout << formal->name() << endl;
    print_tabs(); cout << "type: ";
    cout << formal->type() << endl;

    indent_level_--;
}

void Printer::operator()(StringLiteral* expression) {
    indent_level_++;
    cout << "StringLiteral" << endl;
    print_tabs(); cout << "value: ";
    cout << expression->value() << endl;
    indent_level_--;
}

void Printer::operator()(IntegerLiteral* expression) {
    indent_level_++;
    cout << "IntegerLiteral(";
    cout << expression->value() << ")" << endl;
    indent_level_--;
}


void Printer::operator()(FloatLiteral* expression) {
    indent_level_++;
    cout << "FloatLiteral(";
    cout << expression->value() << ")" << endl;
    indent_level_--;
}

void Printer::operator()(BooleanLiteral* expression) {
    indent_level_++;
    cout << "BooleanLiteral(";
    cout << expression->value() << ")" << endl;
    indent_level_--;
}

void Printer::operator()(Binary* expression) {
    indent_level_++;
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    cout << "Binary" << endl;
    print_tabs(); cout << "operation: ";
    cout << expression->operation() << endl;
    print_tabs(); cout << "left: ";
    left(this);
    print_tabs(); cout << "right: ";
    right(this);
    indent_level_--;
}

void Printer::operator()(Let* expression) {
    indent_level_++;
    Statement::Ptr block = expression->block();
    cout << "Let" << endl;
    
    int i = 0;
    for (Statement::Ptr v = expression->variables(); v; v = v->next()) {
        print_tabs(); cout << "variable" << i << ": ";
        v(this);
        i++;
    }
    print_tabs(); cout << "block: ";
    block(this);

    indent_level_--;
}

void Printer::operator()(Unary* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    cout << "Unary" << endl;
    print_tabs(); cout << "child: ";
    child(this);
    print_tabs(); cout << "operation: ";
    cout << expression->operation() << endl;
    indent_level_--;
}

void Printer::operator()(Call* expression) {
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    cout << "Call" << endl;
    print_tabs(); cout << "name: ";
    cout << expression->identifier() << endl;
    if (expression->module()) {
        print_tabs(); cout << "module: ";
        cout << expression->module() << endl;
    }

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
    cout << expression->identifier() << endl;

    int i = 0;
    for (Expression::Ptr a = arguments; a; a = a->next()) {
        print_tabs(); cout << "argument" << i << ": ";
        a(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Construct* expression) {
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    cout << "Construct" << endl;
    print_tabs(); cout << "type: ";
    cout << expression->type() << endl;

    int i = 0;
    for (Expression::Ptr a = arguments; a; a = a->next()) {
        print_tabs(); cout << "argument" << i << ": ";
        a(this);
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Identifier* expression) {
    indent_level_++;
    cout << "Identifier" << endl;
    print_tabs(); cout << "name: ";
    cout << expression->identifier() << endl;
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
    cout << statement->variable() << endl;
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
    cout << statement->identifier() << endl;
    print_tabs(); cout << "type: ";
    cout << statement->type() << endl;
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
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    cout << "When" << endl;
    print_tabs(); cout << "guard: ";
    guard(this);
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
        i++;
    }
    indent_level_--;
}

void Printer::operator()(Fork* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    cout << "Fork" << endl;
    print_tabs(); cout << "expression: ";
    expression(this);
    indent_level_--;
}

void Printer::operator()(Yield* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    cout << "Yield" << endl;
    print_tabs(); cout << "expression: ";
    expression(this);
    indent_level_--;
}

void Printer::operator()(Function* feature) {
    indent_level_++;
    Statement::Ptr block = feature->block();
    cout << "Function" << endl;
    print_tabs(); cout << "name: ";
    cout << feature->name() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type() << endl;

    if (Block* block = dynamic_cast<Block*>(feature->block())) {
        if (block->comment()) {
            print_tabs(); cout << "comment:" << endl;
            print_comment(block->comment());
        }
    }
    
    int i = 0;
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        print_tabs(); cout << "formal" << i << ": ";
        f(this);
        i++;
    }
    print_tabs(); cout << "block: ";
    block(this);
    indent_level_--;
}

void Printer::operator()(Attribute* feature) {
    indent_level_++;
    Expression::Ptr initializer = feature->initializer();
    cout << "Attribute" << endl;
    print_tabs(); cout << "name: ";
    cout << feature->name() << endl;
    print_tabs(); cout << "type: ";
    cout << feature->type() << endl;
    print_tabs(); cout << "initializer: ";
    initializer(this);
    indent_level_--;
}

void Printer::operator()(Import* feature) {
    indent_level_++;
    cout << "Import" << endl;
    print_tabs(); cout << "file: ";
    cout << feature->file_name() << endl;
    print_tabs(); cout << "scope: ";
    cout << feature->scope() << endl;
    indent_level_--;
}

void Printer::operator()(Type* type) {
    cout << type << endl;
}
