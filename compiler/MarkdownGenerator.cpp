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

#include "MarkdownGenerator.hpp"

MarkdownGenerator::MarkdownGenerator(Environment* env) :
    env_(env) {

    for (Module::Itr m = env_->modules(); m; ++m) {
        m(this);
    }
}

void MarkdownGenerator::operator()(Module* feature) {
    std::string qn = feature->qualified_name()->string(); 
    if (qn.empty()) {
        qn = "Index";
    }
    Stream::Ptr out = new Stream(env_->output()+FILE_SEPARATOR+qn+".md");
    for (Feature::Itr f = feature->features(); f; ++f) {
        out_ = out;
        f(this); // lol
    }
    out->flush();
}

void MarkdownGenerator::operator()(Class* feature) {
    out_ << "## [[" << feature->qualified_name()->string() << "]]\n";

    // Print comment up to the first '.'
    if (feature->comment()) {
        std::string const& comment = feature->comment()->string();
        for (int i = 0; i < comment.length(); ++i) {
            if (comment[i] == '.') { break; }
            out_ << comment[i];
        }
        out_ << '\n';
    }

    // Start a new file for the class itself
    std::string qn = feature->qualified_name()->string();
    out_ = new Stream(env_->output()+FILE_SEPARATOR+qn+".md"); 
    if (feature->comment()) {
        out_ << feature->comment() << '\n';
    }

    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    out_->flush();
    out_ = 0;
}

void MarkdownGenerator::operator()(Function* feature) {
    if (!out_) { return; }

    out_ << "## [" << feature->name() << "](";
    out_ << "https://github.com/mfichman/jogo/tree/master/";
    out_ << feature->file()->path()->string() << "#";
    out_ << "LC" << feature->location().first_line << ")";
    out_ << "(";
    // GitHub auto link.  GitHub puts in IDs for each line of code at LC#

    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        if (f->is_self()) {
            continue;
        }
        out_ << f->name() << " ";
        operator()(f->type());
        if (f->next()) {
            out_ << ", ";
        }
    }
    out_ << ") ";
    if (!feature->type()->is_void()) {
        operator()(feature->type());
    } 
    out_ << "\n";
    if(feature->block() && feature->block()->comment()) {
        out_ << feature->block()->comment() << "\n";
    } else {
        out_ << "_No comment_\n";
    }
}

void MarkdownGenerator::operator()(Type* type) {
    if (!type) { return; }

    out_ << "[[";
    if (type->clazz()) {
        out_ << type->clazz()->qualified_name();
    } else {
        out_ << type->name();
    }
    out_ << "]]";
    
    if (type->generics()) {
        out_ << '[';
        for (Generic::Ptr g = type->generics(); g; g = g->next()) {
            operator()(g->type());
            if (g->next()) {
                out_ << ',';
            }
        }
        out_ << ']';
    } 
}
