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
#include "BasicBlockGenerator.hpp"
#include "RegisterAllocator.hpp"
#include "Intel64CodeGenerator.hpp"
#include "BasicBlockPrinter.hpp"
#include "DeadCodeEliminator.hpp"
#include "CopyPropagator.hpp"
#include "TreePrinter.hpp"
#include "Machine.hpp"
#include "Options.hpp"

#include <iostream>
#include <cstdlib>

Machine::Ptr machine = Machine::intel64();
Environment::Ptr env(new Environment());
BasicBlockGenerator::Ptr bgen(new BasicBlockGenerator(env, machine));
CopyPropagator::Ptr copy(new CopyPropagator(env));
DeadCodeEliminator::Ptr elim(new DeadCodeEliminator(env, machine));
BasicBlockPrinter::Ptr bprint(new BasicBlockPrinter(env, machine));
RegisterAllocator::Ptr alloc(new RegisterAllocator(env, machine));
Intel64CodeGenerator::Ptr cgen(new Intel64CodeGenerator(env));
std::string program_path;

void output(File* file) {
    // Process 'file' and output the requested output path.  Multiple files 
    // may go through this function if the multi-compile option is enabled.
    if (!env->make() && !env->link() && !file->is_input_file()) {
        return;
    }

    std::string name = File::no_ext_name(file->name()->string());
    std::string out_file = env->output_dir() + "/" + name;

    if (env->make() && env->link()) {
        time_t t1 = File::mtime(out_file + ".apo");
        time_t t2 = File::mtime(file->path()->string());
        time_t t3 = File::mtime(program_path);
        file->output(env->name(out_file + ".apo"));
        if (t1 >= t2 && t1 >=t3) { return; }
    }
 
    if (env->verbose()) {
        std::cout << "Compiling " << out_file << std::endl;
    }
    
    if (!env->assemble() || !env->link() || env->make()) {
        File::mkdir(File::dir_name(out_file));
    }

    bgen->operator()(file);
    if (env->optimize()) {
        copy->operator()(file);
        elim->operator()(file); 
    }
    if (env->dump_ir()) {
        bprint->out(new Stream(env->output()));
        bprint->operator()(file);
    } 
    alloc->operator()(file);
    if (env->dump_ir()) {
        bprint->out(new Stream(env->output()));
        bprint->operator()(file);
        exit(0);
    } 

    std::string asm_file = env->assemble() ? tmpnam(0) : out_file + ".s";
    file->output(env->name(asm_file));
    cgen->out(new Stream(asm_file));
    cgen->operator()(file);

    if (!env->assemble()) { return; }

    // Run the assembler.  Output to a non-temp file if the compiler will stop
    // at the assembly stage
    std::string obj_file;
    if (env->link() && !env->make()) {
        obj_file = tmpnam(0);
    } else {

        obj_file = out_file + ".apo";
    }
    file->output(env->name(obj_file));
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "nasm -fobj64 " << asm_file << " -o " << obj_file;
#elif defined(LINUX)
    ss << "nasm -felf64 " << asm_file << " -o " << obj_file;
#elif defined(DARWIN)
    ss << "nasm -fmacho64 " << asm_file << " -o " << obj_file;
#endif 
    if (system(ss.str().c_str())) {
        exit(1);
    }
    remove(asm_file.c_str());
}

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    program_path = argv[0];
    Options(env, argc, argv);

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    Parser::Ptr parser(new Parser(env));
    SemanticAnalyzer::Ptr checker(new SemanticAnalyzer(env));
    if (env->dump_ast()) {
        Stream::Ptr out(new Stream(env->output()));
        TreePrinter::Ptr print(new TreePrinter(env, out));
        return env->errors() ? 0 : 1;
    } 
    if (env->errors()) { return 1; }

    for (File::Ptr file = env->files(); file; file = file->next()) {
        output(file);
    }     
    if (env->errors()) { return 1; }
    if (!env->link()) { return 0; }

    // Run the linker.  Always output to the given output file name.
    std::string obj_files;
    for (File::Ptr file = env->files(); file; file = file->next()) {
        obj_files += file->output()->string() + " ";
    } 
    std::string exe_file = env->execute() ? tmpnam(0) : env->output();
    if (exe_file == "-") {
        exe_file = "out";
    }

    std::stringstream ss;
#if defined(WINDOWS)    
    ss << "link.exe " << obj_files << " /OUT:" << exe_file << ".exe";
#elif defined(LINUX)
    ss << "gcc -m64 " << obj_files << " -o " << exe_file;
#elif defined(DARWIN)
    ss << "gcc -Wl,-no_pie " << obj_files << " -o " << exe_file;
    // FIXME: Remove dynamic-no-pic once rel addressing is fixed
#endif
    for (int i = 0; i < env->libs(); i++) {
#ifdef WINDOWS
        ss << env->lib(i) << ".lib";
#else
        ss << " -l" << env->lib(i);
#endif
    }
    for (int i = 0; i < env->includes(); i++) {
        if (File::is_dir(env->include(i))) {
#ifdef WINDOWS
            ss << "/L:" << env->include(i);
#else
            ss << " -L" << env->include(i);  
#endif
        }
    }

    if (system(ss.str().c_str())) { return 1; }
    if (!env->make()) {
        for (File::Ptr file = env->files(); file; file = file->next()) {
            remove(file->output()->string().c_str());   
        }
    }

    if (!env->execute()) { return 0; }

    // Run the program, if the -e flag was specified.
    ss.str("");
#ifdef WINDOWS
    ss << exe_file;
#else
    ss << exe_file;
#endif
    int ret = system(ss.str().c_str());
    remove(exe_file.c_str());
    return ret;
}
