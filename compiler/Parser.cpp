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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Parser.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cassert>

int yyparse(Parser* parser, void* scanner);
int yylex_init(void**);
int yylex_destroy(void*);
void yyset_extra(Parser*, void*);

Parser::Parser(Environment* env) :
	environment_(env) {

    yylex_init(&this->scanner_);
    yyset_extra(this, this->scanner_);
}

Parser::~Parser() {

    yylex_destroy(this->scanner_);   
}

void Parser::file(const std::string& file) {
    Name* scope = environment_->name(Import::scope_name(file));

    // Begin parsing a module file if it doesn't already exist
    module_ = environment_->module(scope);
    if (module_) {
        return;
    } else {
        module_ = new Module(Location(), scope);
        environment_->module(module_);
    }

    this->column_ = 1;
    this->input_.close();
    this->input_.open(file.c_str()); 
	this->file_ = file;
    if (this->input_.fail() || this->input_.bad()) {
        environment_->error("Could not find " + file);
        std::cerr << "Could not find + " + file << std::endl;
        return;
    }

    // Begin parsing
    yyparse(this, this->scanner_);

    // Now parse other modules that depend on the unit that was added
    for (Feature* f = module_->features(); f; f = f->next()) {
        if (Import* import = dynamic_cast<Import*>(f)) {
            Parser::file(import->file_name());
        }
    }
}

void yyerror(Location* loc, Parser* self, void* scanner, const char* msg) {

    self->environment()->error(msg);
	std::cerr << self->file() << ":" << loc->first_line << ":";
	std::cerr << loc->first_column << ": ";
	std::cerr << (char)toupper(msg[0]) << msg + 1 << std::endl;	
}
