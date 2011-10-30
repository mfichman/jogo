/*
 * Copyright (c) 2011 Matt Fichman
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

#include "TreePrinter.hpp"
#include <iostream>

using namespace std;

TreePrinter::TreePrinter(Environment* env, Stream* out) :
    env_(env),
    out_(out),
    indent_level_(0) {

    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
    }    
    out_->flush();
}

void TreePrinter::print_tabs() {
    for (int i = 0; i < indent_level_;  i++) {
        out_ << "  ";
    }
}

void TreePrinter::print_comment(String* comment) {
    // Pretty-prints a comment so that it wraps to 80 columns and breaks at 
    // whitespace when it is output.
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
                    out_ << *begin++;
                    counter--;
                }
            } else {
                // Word is shorter than the length of the line, so print to
                // the last space.
                while (begin <= end && *begin) {
                    out_ << *begin++;
                }
            }
            out_ << "\n";
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
        out_ << *begin++;
    }
    out_ << "\n";

    indent_level_--;
}

void TreePrinter::operator()(Module* feature) {
    indent_level_++;
    out_ << "Module\n";
    print_tabs(); out_ << "name: " << feature->name() << "\n";
    int i = 0;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        const Location& loc = f->location();
        if (loc.file->path()->string().find("../runtime") != 0) {
            print_tabs(); out_ << "feature" << i << ": ";
            f(this);
            i++;
        }
    }
    indent_level_--;
}

void TreePrinter::operator()(Class* feature) {
    indent_level_++; 
    out_ << "Class\n";
    print_tabs(); out_ << "name: " << feature->name() << "\n";
    print_tabs(); out_ << "comment:\n";
    print_comment(feature->comment());
    int i = 0;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        print_tabs(); out_ << "feature" << i << ": ";
        f(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Formal* formal) {
    indent_level_++;
    out_ << "Formal\n";
    print_tabs(); out_ << "name: ";
    out_ << formal->name() << "\n";
    print_tabs(); out_ << "type: ";
    out_ << formal->type() << "\n";

    indent_level_--;
}

void TreePrinter::operator()(StringLiteral* expression) {
    indent_level_++;
    out_ << "StringLiteral\n";
    print_tabs(); out_ << "value: ";
    out_ << expression->value() << "\n";
    indent_level_--;
}

void TreePrinter::operator()(NilLiteral* expression) {
    indent_level_++;
    out_ << "nil\n";
    indent_level_--;
}

void TreePrinter::operator()(IntegerLiteral* expression) {
    indent_level_++;
    out_ << "IntegerLiteral(";
    out_ << expression->value() << ")\n";
    indent_level_--;
}

void TreePrinter::operator()(FloatLiteral* expression) {
    indent_level_++;
    out_ << "FloatLiteral(";
    out_ << expression->value() << ")\n";
    indent_level_--;
}

void TreePrinter::operator()(BooleanLiteral* expression) {
    indent_level_++;
    out_ << "BooleanLiteral(";
    out_ << expression->value() << ")\n";
    indent_level_--;
}

void TreePrinter::operator()(ArrayLiteral* expression) {
    indent_level_++;
    out_ << "ArrayLiteral\n";
    
    int i = 0;
    for (Expression::Ptr e = expression->arguments(); e; e = e->next()) {
        print_tabs(); out_ << "argument" << i << ": ";
        e(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(HashLiteral* expression) {
    indent_level_++;
    out_ << "HashLiteral\n";
    
    int i = 0;
    for (Expression::Ptr e = expression->arguments(); e; e = e->next()) {
        print_tabs(); out_ << "argument" << i << ": ";
        e(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Binary* expression) {
    indent_level_++;
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    out_ << "Binary\n";
    print_tabs(); out_ << "operation: ";
    out_ << expression->operation() << "\n";
    print_tabs(); out_ << "left: ";
    left(this);
    print_tabs(); out_ << "right: ";
    right(this);
    indent_level_--;
}

void TreePrinter::operator()(Let* expression) {
    indent_level_++;
    Statement::Ptr block = expression->block();
    out_ << "Let\n";
    
    int i = 0;
    for (Expression::Ptr v = expression->variables(); v; v = v->next()) {
        print_tabs(); out_ << "variable" << i << ": ";
        v(this);
        i++;
    }
    print_tabs(); out_ << "block: ";
    block(this);

    indent_level_--;
}

void TreePrinter::operator()(Cast* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    out_ << "Cast\n";
    print_tabs(); out_ << "child: ";
    child(this);
    print_tabs(); out_ << "type: ";
    out_ << expression->type() << "\n";
    indent_level_--;
}

void TreePrinter::operator()(Box* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    out_ << "Box\n";
    print_tabs(); out_ << "child: ";
    child(this);
    print_tabs(); out_ << "type: ";
    out_ << expression->type() << "\n";
    indent_level_--;
}

void TreePrinter::operator()(Unary* expression) {
    indent_level_++;
    Expression::Ptr child = expression->child();
    out_ << "Unary\n";
    print_tabs(); out_ << "child: ";
    child(this);
    print_tabs(); out_ << "operation: ";
    out_ << expression->operation() << "\n";
    indent_level_--;
}

void TreePrinter::operator()(Member* expression) {
    indent_level_++;
    Expression::Ptr expr = expression->expression();
    out_ << "Member\n";
    print_tabs(); out_ << "identifier: ";
    out_ << expression->identifier() << "\n";
    print_tabs(); out_ << "expression: ";
    expr(this);
    indent_level_--;
}

void TreePrinter::operator()(Call* expression) {
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    Expression::Ptr expr = expression->expression();
    out_ << "Call\n";
    print_tabs(); out_ << "expression: ";
    expr(this);

    int i = 0;
    for (Expression::Ptr a = arguments; a; a = a->next()) {
        print_tabs(); out_ << "argument" << i << ": ";
        a(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Construct* expression) {
    indent_level_++;
    Expression::Ptr arguments = expression->arguments();
    out_ << "Construct\n";
    print_tabs(); out_ << "type: ";
    out_ << expression->type() << "\n";

    int i = 0;
    for (Expression::Ptr a = arguments; a; a = a->next()) {
        print_tabs(); out_ << "argument" << i << ": ";
        a(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Identifier* expression) {
    indent_level_++;
    out_ << "Identifier\n";
    print_tabs(); out_ << "name: ";
    out_ << expression->identifier() << "\n";
    indent_level_--; 
}

void TreePrinter::operator()(Empty* empty) {
    out_ << "Empty\n";
}

void TreePrinter::operator()(Block* statement) {
    indent_level_++;
    Statement::Ptr children = statement->children();
    out_ << "Block\n";

    int i = 0;
    for (Statement::Ptr c = children; c; c = c->next()) {
        print_tabs(); out_ << "child" << i << ": ";
        c(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Simple* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    out_ << "Statement\n";
    print_tabs(); out_ << "expression: ";
    expression(this);
    indent_level_--;
}

void TreePrinter::operator()(While* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    out_ << "While\n";
    print_tabs(); out_ << "guard: ";
    guard(this);
    print_tabs(); out_ << "block: ";
    block(this);
    indent_level_--;
}

void TreePrinter::operator()(Conditional* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    out_ << "Conditional\n";
    print_tabs(); out_ << "guard: ";
    guard(this);
    print_tabs(); out_ << "true: ";
    true_branch(this);
    if (false_branch) {
        print_tabs(); out_ << "false: ";
        false_branch(this);
    }
    indent_level_--;
}

void TreePrinter::operator()(Assignment* statement) {
    indent_level_++;
    Expression::Ptr initializer = statement->initializer();
    out_ << "Assignment\n";
    print_tabs(); out_ << "name: ";
    out_ << statement->identifier() << "\n";
    print_tabs(); out_ << "type: ";
    out_ << statement->type() << "\n";
    print_tabs(); out_ << "initializer: ";
    initializer(this);
    indent_level_--; 
}

void TreePrinter::operator()(Return* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    out_ << "Return\n";
    print_tabs(); out_ << "expression: "; 
    expression(this);
    indent_level_--;
}

void TreePrinter::operator()(When* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    out_ << "When\n";
    print_tabs(); out_ << "guard: ";
    guard(this);
    print_tabs(); out_ << "block: ";
    block(this);
    indent_level_--; 
}

void TreePrinter::operator()(Case* statement) {
    indent_level_++;
    Expression::Ptr guard = statement->guard();
    out_ << "Case\n";
    print_tabs(); out_ << "guard: ";
    guard(this);

    int i = 0; 
    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        print_tabs(); out_ << "branch" << i << ": ";
        b(this);
        i++;
    }
    indent_level_--;
}

void TreePrinter::operator()(Fork* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    out_ << "Fork\n";
    print_tabs(); out_ << "expression: ";
    expression(this);
    indent_level_--;
}

void TreePrinter::operator()(Yield* statement) {
    indent_level_++;
    Expression::Ptr expression = statement->expression();
    out_ << "Yield\n";
    if (expression) {
        print_tabs(); out_ << "expression: ";
        expression(this);
    }
    indent_level_--;
}

void TreePrinter::operator()(Function* feature) {
    indent_level_++;
    Statement::Ptr block = feature->block();
    out_ << "Function\n";
    print_tabs(); out_ << "name: ";
    out_ << feature->name() << "\n";
    print_tabs(); out_ << "type: ";
    out_ << feature->type() << "\n";

    if (Block* block = dynamic_cast<Block*>(feature->block())) {
        if (block->comment()) {
            print_tabs(); out_ << "comment:\n";
            print_comment(block->comment());
        }
    }
    
    int i = 0;
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        print_tabs(); out_ << "formal" << i << ": ";
        f(this);
        i++;
    }
    print_tabs(); out_ << "block: ";
    if (block) { 
        block(this);
    } else {
        out_ << "\n";
    }
    indent_level_--;
}

void TreePrinter::operator()(Attribute* feature) {
    indent_level_++;
    Expression::Ptr initializer = feature->initializer();
    out_ << "Attribute\n";
    print_tabs(); out_ << "name: ";
    out_ << feature->name() << "\n";
    print_tabs(); out_ << "type: ";
    out_ << feature->type() << "\n";
    print_tabs(); out_ << "initializer: ";
    initializer(this);
    indent_level_--;
}

void TreePrinter::operator()(Import* feature) {
    indent_level_++;
    out_ << "Import\n";
    print_tabs(); out_ << "file: ";
    out_ << feature->file_name() << "\n";
    print_tabs(); out_ << "scope: ";
    out_ << feature->scope() << "\n";
    indent_level_--;
}

void TreePrinter::operator()(Type* type) {
    out_ << type << "\n";
}
