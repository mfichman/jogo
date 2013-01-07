/*
 * Copyright (c) 2011 Matt Fichman
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

#include "Environment.hpp"
#include "Parser.hpp"
#include "Options.hpp"
#include "SemanticAnalyzer.hpp"
#include "MarkdownGenerator.hpp"

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    Environment::Ptr env(new Environment());
    Options(env, argc, argv);

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    Parser::Ptr parser(new Parser(env));
    SemanticAnalyzer::Ptr semant(new SemanticAnalyzer(env));
    MarkdownGenerator::Ptr md(new MarkdownGenerator(env));

    return 0;
}