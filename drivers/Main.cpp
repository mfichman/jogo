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
#include "SemanticAnalyzer.hpp"
#include "Builder.hpp"
#include "File.hpp"
#include "Machine.hpp"
#include "Options.hpp"
#include "InterfaceGenerator.hpp"
#include "TreePrinter.hpp"

#include <iostream>
#include <cstdlib>


int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    Environment::Ptr env(new Environment());
    Options(env, argc, argv);

#ifndef WINDOWS
    env->include("/usr/local/include/apollo");
    env->include("/usr/local/lib");
#else
    std::string program_files = getenv("PROGRAMFILES");
    std::string program_files_x86 = getenv("PROGRAMFILES(x86)");
    env->include(program_files + "\\Apollo\\include\\apollo");
    env->include(program_files_x86 + "\\Apollo\\include\\apollo");
    env->include(program_files + "\\Apollo\\lib");
    env->include(program_files_x86 + "\\Apollo\\lib");
#endif

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise,  the file directly.
    Parser::Ptr parser(new Parser(env));
    if (env->errors()) { return 1; }

    SemanticAnalyzer::Ptr checker(new SemanticAnalyzer(env));
    if (env->dump_ast()) {
        TreePrinter::Ptr tprint(new TreePrinter(env, Stream::stout()));
        return 0;
    }
    if (env->errors()) { return 1; }

    Builder::Ptr builder(new Builder(env));
    return builder->errors();
}
