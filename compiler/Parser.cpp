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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Parser.hpp"
#include "File.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cassert>

int yyparse(Parser* parser, void* scanner);
int yylex_init(void**);
int yylex_destroy(void*);
void yyset_extra(Parser*, void*);

Parser::Parser(Environment* env) :
	environment_(env),
    err_(Stream::sterr()) {

    for (int i = 0; i < env->inputs(); i++) {
        input(env->input(i));
    }
}

Parser::~Parser() {

}

void Parser::input(const std::string& import) {
    // Reads in a file or directory after searching through the list of include
    // paths.  The includes paths are searched in the order they are specified,
    // so if duplicate files are present in two include directories, then only
    // the first will be parsed.

    std::string file = Import::file_name(import); 
    std::vector<std::string> tests;

    for (int i = 0; i < environment_->includes(); i++) {
        const std::string& prefix = environment_->include(i);
        const std::string ext = ".ap";
        if (file.compare(file.length() - ext.length(), ext.length(), ext)) {
            if (File::is_reg(prefix + "/" + file + ".ap")) {
                std::vector<std::string>().swap(tests);
                Parser::file(prefix, file + ".ap");
                return;
            }
        } else {
            if (File::is_reg(prefix + "/" + file)) {
                std::vector<std::string>().swap(tests);
                Parser::file(prefix, file);
                return; 
            } 
        }
        if (File::is_dir(prefix + "/" + file)) {
            std::vector<std::string>().swap(tests);
            Parser::dir(prefix, file);
            return;
        }   
        tests.push_back(prefix + "/" + file + ".ap");
        tests.push_back(prefix + "/" + file);
    }
    environment_->error("Could not find " + import);
    //std::cerr << import->location();
    err_ << "Module '" << import << "' not found:\n";
    for (int i = 0; i < tests.size(); i++) {
        err_ << "    no file '" << tests[i] << "'\n";
    }
}

void Parser::file(const std::string& prefix, const std::string& file) {
    // Begin parsing a module file if it doesn't already exist.

    // The module can be loaded from the dir components of the file name
    String* scope = environment_->name(Import::scope_name(file));
    module_ = environment_->module(scope);
    if (!module_) {
        module_ = new Module(Location(), scope, environment_);
        environment_->module(module_);
    }

    // Create a file object for this file if it hasn't been parsed yet.
    std::string actual_file = (prefix == ".") ? file : prefix + "/" + file;
    String* fs = environment_->name(actual_file);
    file_ = environment_->file(fs);
    if (file_) {
        return;
    } else {
        file_ = new File(fs, module_, environment_);
        environment_->file(file_);
    }

    this->column_ = 1;
    this->input_.close();
    this->input_.open(actual_file.c_str()); 

    // Begin parsing
    yylex_init(&this->scanner_);
    yyset_extra(this, this->scanner_);
    yyparse(this, this->scanner_);
    yylex_destroy(this->scanner_);   

    // Now parse other modules that depend on the unit that was added
    for (Feature* f = file_->features(); f; f = f->next()) {
        if (Import* import = dynamic_cast<Import*>(f)) {
            input(import->scope()->string());
        }
    }
}

void Parser::dir(const std::string& prefix, const std::string& dir) {
    // Parses all files in the directory specified by 'dir'.  If 'dir' is not
    // a directory, then no files are parsed.

    for (File::Iterator i(dir); i; ++i) {
        std::string name = *i;
        if (name.length() >= 3 && name.substr(name.length()-4, 2) != ".ap") {
            file(prefix, dir + "/" + name);
        } 
    }
}

void yyerror(Location* loc, Parser* self, void* scanner, const char* msg) {

    self->environment()->error(msg);
	self->err_ << self->file()->name() << ":" << loc->first_line << ":";
	self->err_ << loc->first_column << ": ";
	self->err_ << (char)toupper(msg[0]) << msg + 1 << "\n";	
}
