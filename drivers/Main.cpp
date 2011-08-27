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
#include "BasicBlockGenerator.hpp"
#include "RegisterAllocator.hpp"
#include "Intel64CodeGenerator.hpp"
#include "TreePrinter.hpp"
#include "Machine.hpp"
#include "Options.hpp"

#include <iostream>

int main(int argc, char** argv) {
    Environment::Ptr env(new Environment());
    Options(env, argc, argv);

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    std::string asm_file = env->assemble() ? tmpnam(0) : env->output();
    Parser::Ptr parser(new Parser(env));
    TypeChecker::Ptr checker(new TypeChecker(env));
    if (env->errors()) { return 0; }

    Machine::Ptr machine = Machine::intel64();
    BasicBlockGenerator::Ptr generator(new BasicBlockGenerator(env, machine));
    RegisterAllocator::Ptr alloc(new RegisterAllocator(env, machine));
    Intel64CodeGenerator::Ptr codegen(new Intel64CodeGenerator(env, asm_file));

    if (!env->assemble()||env->errors()) { return 0; }

    // Run the assembler.  Output to a non-temp file if the compiler will stop
    // at the assembly stage
    std::string obj_file = env->link() ? tmpnam(0) : env->output();
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "nasm -fobj64 " << asm_file << " -o " << obj_file;
#elif defined(LINUX)
    ss << "nasm -felf64 " << asm_file << " -o " << obj_file;
#elif defined(DARWIN)
    ss << "nasm -fmacho64 " << asm_file << " -o " << obj_file;
#endif 
    system(ss.str().c_str());
    ss.str("");

    if (!env->link()) { return 0; }

    // Run the linker.  Always output to the given output file name.
    std::string exe_file = env->output();
#if defined(WINDOWS)    
    ss << "link.exe " << obj_file << " /OUT:" << exe_file;
#elif defined(LINUX)
    ss << "gcc -m64 -lapollo " << obj_file << " -o " << exe_file;
#elif defined(DARWIN)
    ss << "gcc -Llib -lapollo " << obj_file << " -o " << exe_file;
#endif
    system(ss.str().c_str());

    return 0;
}
