/*
 * Copyright (c) 2010 Matt Fichman
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

#include "InterfaceGenerator.hpp"

InterfaceGenerator::InterfaceGenerator(Environment* env, Stream* out) :
    env_(env),
    out_(out),
    indent_level_(0) {
}

void InterfaceGenerator::operator()(File* file) {
    if (env_->errors()) { return; }
    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this);
    }
    out_->flush();
}

void InterfaceGenerator::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void InterfaceGenerator::operator()(Class* feature) {
    out_ << feature->type() << " < ";

    if (feature->is_enum()) {
        out_ << "Enum";
    } else {
        out_ << feature->proto();
    }
    out_ << " {\n";
    indent_level_++;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    indent_level_--;
    out_ << "}\n";
}

void InterfaceGenerator::operator()(Constant* constant) {
    print_tabs();
    out_ << constant->name() << "\n";
}

void InterfaceGenerator::operator()(Function* function) {
    print_tabs();
    if (dynamic_cast<Module*>(function->parent())) {
        out_ << function->qualified_name() << "(";
    } else {
        out_ << function->name() << "(";
    }
    for (Formal::Ptr f = function->formals(); f; f = f->next()) {
        if (!f->type()->is_self()) {
           out_ << f->name() << " " << f->type(); 
           if (f->next()) {
               out_ << ", ";
           }
        }
    }
    out_ << ")";
    if (function->type() && !function->type()->is_void()) {
        out_ << " " << function->type();
    }
    out_ << "\n";
}

void InterfaceGenerator::print_tabs() {
    for (int i = 0; i < indent_level_; i++) {
        out_ << "    ";
    } 
}
