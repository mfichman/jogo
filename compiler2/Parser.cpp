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

int yyparse(Parser* parser, void* scanner);
int yylex_init(void**);
int yylex_destroy(void*);
void yyset_extra(Parser*, void*);


void Parser::file(const std::string& file) {
    /* Begin parsing at the root file */
    this->column_ = 1;
    this->error_ = 0;
    this->input_.open(file.c_str()); 
	this->file_ = file;
    if (this->input_.bad()) {
        throw std::runtime_error("Could not find " + file);
    }

    /* Begin parsing */
    yylex_init(&this->scanner_);
    yyset_extra(this, this->scanner_);
    yyparse(this, this->scanner_);
    yylex_destroy(this->scanner_);   

    /* Now parse other units that depend on this unit */
    Unit* unit = environment_->units();
    for (Feature* feat = unit->features(); feat; feat = feat->next()) {
        Import* import = dynamic_cast<Import*>(feat);
        if (import) {
            Parser::file(import->file());
            break;
        }
    }
}

void yyerror(Location* loc, Parser* self, void* scanner, const char* msg) {

	self->error(self->error() + 1);
	std::cerr << self->file() << ":" << loc->first_line << ":";
	std::cerr << loc->first_column << ": ";
	std::cerr << (char)toupper(msg[0]) << msg + 1 << std::endl;	
}
