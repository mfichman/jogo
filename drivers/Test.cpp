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
#include "TypeChecker.hpp"
#include "BasicBlockPrinter.hpp"
#include "BasicBlockGenerator.hpp"
#include "LivenessAnalyzer.hpp"
#include "Intel64CodeGenerator.hpp"
#include "TreePrinter.hpp"
#include "Options.hpp"

#include <iostream>
#include <fstream>
#include <set>

int main(int argc, char** argv) {
    Environment::Ptr env(new Environment());
    Options(env, argc, argv);

    std::fstream in((std::string(argv[1]) + ".ap").c_str());
    std::set<std::string> test_options;
    std::string line;
    std::getline(in, line);
    while (!line.empty() && line[0] == '#') {
        test_options.insert(line);
        std::getline(in, line);
    } 

    Parser::Ptr parser(new Parser(env));
    if (test_options.find("# aptest print_tree off") == test_options.end()) {
        TreePrinter::Ptr printer(new TreePrinter(env));
    }
    TypeChecker::Ptr checker(new TypeChecker(env));
    BasicBlockGenerator::Ptr generator(new BasicBlockGenerator(env));
    if (test_options.find("# aptest print_ir off") == test_options.end()) {
        BasicBlockPrinter::Ptr printer(new BasicBlockPrinter(env));
    }
    if (test_options.find("# aptest run_program on") != test_options.end()) {
        std::cout << std::endl;
        Machine::Ptr machine = Machine::intel64();
        RegisterAllocator::Ptr alloc(new RegisterAllocator(env, machine));
        BasicBlockPrinter::Ptr printer(new BasicBlockPrinter(env));
        Intel64CodeGenerator::Ptr codegen(new Intel64CodeGenerator(env));
        system("nasm -fmacho64 out.asm -o /tmp/out.o");
        system("gcc -lapollo -L../lib /tmp/out.o -o /tmp/out");
        system("/tmp/out");
    }
   
    
    return 0;
}
