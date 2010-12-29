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

#ifndef PARSER_H
#define PARSER_H

#include "apollo.hpp"
#include "environment.hpp"
#include "object.hpp"

#include <fstream>

/* Primary parser structure; creates compilation units */
class Parser : public Object {
public:
    void parse(Environment* env, const std::string& filename);
    Environment* environment() const { return environment_; }
    std::fstream& input() { return input_; }
	const std::string& filename() const { return filename_; }
    int column() const { return column_; }
    void column(int column) { column_ = column; }
	int error() const { return error_; }
	void error(int error) { error_ = error; }
    typedef Pointer<Parser> Ptr;

private:
    Environment::Ptr environment_;
    int column_;
    int error_;
    std::fstream input_;
    std::string filename_;
    void *scanner_;
};

/* Union used by Bison and Flex for actions */
union ParseNode {
	Expression* expression;
	Statement* statement;
	Unit* unit;
	Formal* formal;
	When* when;
	Feature* feature;
	Name* name;
	int null;
	int flag;
};


#endif
