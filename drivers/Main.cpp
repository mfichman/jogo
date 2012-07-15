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
#include "CCodeGenerator.hpp"
#include "BasicBlockPrinter.hpp"
#include "DeadCodeEliminator.hpp"
#include "File.hpp"
#include "CopyPropagator.hpp"
#include "TreePrinter.hpp"
#include "Machine.hpp"
#include "Options.hpp"
#include "InterfaceGenerator.hpp"

#include <iostream>
#include <cstdlib>

Machine::Ptr machine = Machine::intel64();
Environment::Ptr env(new Environment());
BasicBlockGenerator::Ptr bgen(new BasicBlockGenerator(env, machine));
CopyPropagator::Ptr copy(new CopyPropagator(env, machine));
DeadCodeEliminator::Ptr elim(new DeadCodeEliminator(env, machine));
BasicBlockPrinter::Ptr bprint(new BasicBlockPrinter(env, machine));
RegisterAllocator::Ptr alloc(new RegisterAllocator(env, machine));
Intel64CodeGenerator::Ptr intel64gen(new Intel64CodeGenerator(env));
CCodeGenerator::Ptr cgen(new CCodeGenerator(env));
std::string program_path;

std::string tempnam() {
#ifdef WINDOWS
    static const char* prefix = "tmp";
    char path[MAX_PATH];
    char file[MAX_PATH];
    GetTempPath(sizeof(path), path);
    GetTempFileName(path, prefix, 0, file);
    return std::string(file);
#else
    return std::string(tmpnam(0));
#endif
}

void output_intel64(File* file) {
    // Outputs an object file for 'file' using the Intel64 code generator
    // and the NASM assembler.

    if (env->optimize()) {
        copy->operator()(file);
        elim->operator()(file); 
    }
    if (env->dump_ir() && file->is_input_file()) {
        bprint->out(new Stream(env->output()));
        bprint->operator()(file);
    } 
    alloc->operator()(file);
    if (env->dump_ir() && file->is_input_file()) {
        bprint->out(new Stream(env->output()));
        bprint->operator()(file);
        return;
    } 

    std::string name = File::no_ext_name(file->name()->string());
    std::string out_file = env->output_dir() + FILE_SEPARATOR + name;
    std::string asm_file = env->assemble() ? tempnam() : out_file + ".asm";
    file->output(env->name(asm_file));
    intel64gen->out(new Stream(asm_file));
    if (intel64gen->out()->error()) {
        Stream::sterr() << asm_file << ": " << intel64gen->out()->message() << "\n";  
        Stream::sterr()->flush();
        Stream::stout()->flush();
        exit(1);
    }
    intel64gen->operator()(file);

    if (!env->assemble()) { return; }

    // Run the assembler.  Output to a non-temp file if the compiler will stop
    // at the assembly stage
    std::string obj_file;
    if (env->link() && !env->make()) {
        obj_file = tempnam();
    } else {
        obj_file = out_file + ".ap.o";
    }
    file->output(env->name(obj_file));
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "nasm -fwin64 " << asm_file << " -o " << obj_file;
#elif defined(LINUX)
    ss << "nasm -felf64 " << asm_file << " -o " << obj_file;
#elif defined(DARWIN)
    ss << "nasm -fmacho64 " << asm_file << " -o " << obj_file;
#endif 
    if (env->verbose()) {
        Stream::stout() << ss.str() << "\n";
    }
    if (system(ss.str().c_str())) {
        exit(1);
    }
    remove(asm_file.c_str());
}

void compile_c(const std::string& c_file, const std::string& obj_file) {
    // Compiles a C source file, using the compiler flags specified by the
    // command-line options.
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "cl.exe " << c_file << " /c /TC /Fo:" << obj_file;
    if (env->optimize()) {
        ss << " /O2";
    } else {
        ss << " /O0";
    }
    ss << " /DCOROUTINE_STACK_SIZE=" << COROUTINE_STACK_SIZE;
#else
    ss << "gcc " << c_file << " -c -o " << obj_file;
    if (env->optimize()) {
        ss << " -O2";
    } else {
        ss << " -O0 -g";
    }
    ss << " -DCOROUTINE_STACK_SIZE=" << COROUTINE_STACK_SIZE;
#ifdef WINDOWS
    ss << " /DWINDOWS";
#elif defined(DARWIN)
    ss << " -DDARWIN";
#elif defined(LINUX)
    ss << " -DLINUX -m64 -lm";
#endif

#endif
    for (int i = 0; i < env->includes(); i++) {
        if (File::is_dir(env->include(i))) {
#if defined(WINDOWS)
            ss << " /I " << env->include(i); 
#else
            ss << " -I " << env->include(i);
#endif
        }
    }
    if (env->verbose()) {
        Stream::stout() << ss.str() << "\n";
        Stream::stout()->flush();
    }
    if (system(ss.str().c_str())) {
        exit(1);
    }
}

void output_c(File* file) {
    // Outputs an object file for 'file' using the C code generator and the
    // platform-native C compiler.
    std::string name = File::no_ext_name(file->name()->string());
    std::string out_file = env->output_dir() + FILE_SEPARATOR + name;
    std::string c_file = (env->assemble() ? tempnam() : out_file) + ".ap.c";
    file->output(env->name(c_file));
    cgen->out(new Stream(c_file));
    if (cgen->out()->error()) {
        Stream::sterr() << c_file << ": " << cgen->out()->message() << "\n";
        Stream::sterr()->flush();
        Stream::stout()->flush();
        exit(1);
    }
    cgen->operator()(file);
    if (env->verbose()) {
        Stream::stout()->flush();
    }

    if (!env->assemble()) { return; }

    // Run the C compiler.  Output to a non-temp file if the compiler will
    // stop at the 'assembly' stage.
    std::string obj_file;
    if (env->link() && !env->make()) {
        obj_file = tempnam();
    } else {
        obj_file = out_file + ".ap.o";
    }
    file->output(env->name(obj_file)); 
    compile_c(c_file, obj_file);
    remove(c_file.c_str());
}

void compile_native(File* file) {
    // Compiles the native version of this file, if it exists.
    if (!env->assemble()) {
        return;
    }

    std::string name = File::no_ext_name(file->name()->string()); 
    std::string c_file = File::no_ext_name(file->path()->string()) + ".c";
    std::string out_file = env->output_dir() + FILE_SEPARATOR + name;
    std::string obj_file = out_file + ".o";

    if (!File::is_reg(c_file)) {
        return;
    }
    File::mkdir(File::dir_name(out_file));
    file->native_output(env->name(obj_file));

    // Check to make sure that the .o file hasn't been compiled already.
    if (env->make() && env->link()) {
        time_t t1 = File::mtime(c_file);
        time_t t2 = File::mtime(obj_file);
        time_t t3 = File::mtime(program_path);
        if (t1 >= t2 && t1 >= t3) { return; }
    }
    
    if (env->verbose()) {
        Stream::stout() << "Compiling " << out_file << ".c\n";
    }

    compile_c(c_file, obj_file);
}

void output(File* file) {
    // Process 'file' and output the requested output path.  Multiple files 
    // may go through this function if the multi-compile option is enabled.
    if (!env->make() && !env->link() && !file->is_input_file()) {
        return;
    }
    compile_native(file);

    std::string name = File::no_ext_name(file->name()->string());
    std::string out_file = env->output_dir() + FILE_SEPARATOR + name;
    std::string boot_main = "Boot"FILE_SEPARATOR_STR"Main";
    // FIXME: Temporary fix so that 'main' is recompiled every time to ensure
    // that the static initialization is done properly.

    if (env->make() && env->link()) {
        time_t t1 = File::mtime(out_file + ".ap.o");
        time_t t2 = File::mtime(file->path()->string());
        time_t t3 = File::mtime(program_path);
        file->output(env->name(out_file + ".ap.o"));
        if (t1 >= t2 && t1 >= t3 && name != boot_main) { return; }
    }
 
    if (env->verbose()) {
        Stream::stout() << "Compiling " << out_file << ".ap\n";
    }
    
    if (!env->assemble() || !env->link() || env->make()) {
        File::mkdir(File::dir_name(out_file));
    }

    bgen->operator()(file);

    if (env->generator() == "Intel64") {
        output_intel64(file);
    } else if (env->generator() == "C") {
        output_c(file);
    }
}

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    program_path = argv[0];
    Options(env, argc, argv);

#ifndef WINDOWS
    env->include("/usr/local/include/apollo");
#else
    std::string program_files = getenv("PROGRAMFILES");
    std::string program_files_x86 = getenv("PROGRAMFILES(x86)");
    env->include(program_files + "\\Apollo\\include\\apollo");
    env->include(program_files_x86 + "\\Apollo\\include\\apollo");
    env->include(program_files + "\\Apollo\\lib");
    env->include(program_files_x86 + "\\Apollo\\lib");
#endif
    if (!env->no_default_mods()) {
        env->lib("apollo");
    }
    if (!env->gen_library() && !env->no_default_mods()) {
        env->input("Boot::Main");
    }

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    Parser::Ptr parser(new Parser(env));
    SemanticAnalyzer::Ptr checker(new SemanticAnalyzer(env));
    Stream::sterr()->flush();

    if (env->dump_ast()) {
        Stream::Ptr out(new Stream(env->output()));
        TreePrinter::Ptr print(new TreePrinter(env, out));
        return env->errors() ? 0 : 1;
    } 
    if (env->errors()) { return 1; }

    for (File::Ptr file = env->files(); file; file = file->next()) {
        if (file->is_output_file()) {
            output(file);
        }
    }     
    if (env->dump_ir()) { return 0; }
    if (env->errors()) { return 1; }
    if (!env->link()) { return 0; }

    std::string base_output = File::no_ext_name(env->output());
    if (env->gen_library()) {
        Stream::Ptr out(new Stream(base_output+".api"));
        InterfaceGenerator::Ptr igen(new InterfaceGenerator(env, out));
    }

    // Run the linker.  Always output to the given output file name.
    std::string obj_files;
    for (File::Ptr file = env->files(); file; file = file->next()) {
        if (file->is_output_file()) {
            obj_files += file->output()->string() + " ";
            if (file->native_output()) {
                obj_files += file->native_output()->string() + " ";
            }
        }
    } 
    std::string exe_file = env->execute() ? tempnam() : base_output;
    if (exe_file == "-") {
        exe_file = "out";
    }

    std::stringstream ss;
#if defined(WINDOWS)    
    ss << "link.exe /DEBUG /SUBSYSTEM:console /NOLOGO /MACHINE:amd64 ";
    ss << obj_files << " /OUT:" << exe_file << ".exe ";
#elif defined(LINUX)
    if (File::base_name(exe_file).find("libapollo") == 0) {
        obj_files += "build/runtime/Coroutine.Intel64.o";
    }
    if (env->gen_library()) {
        ss << "ar rcs " << exe_file << ".a " << obj_files;
    } else {
        ss << "gcc -m64 " << obj_files << " -o " << exe_file;
    }
#elif defined(DARWIN)
    if (File::base_name(exe_file).find("libapollo") == 0) {
        obj_files += "build/runtime/Coroutine.Intel64.o";
    }
    if (env->gen_library()) {
        ss << "ar rcs " << exe_file << ".a " << obj_files; 
    } else {
        ss << "gcc -Wl,-no_pie " << obj_files << " -o " << exe_file;
    }
    // FIXME: Remove dynamic-no-pic once rel addressing is fixed
#endif

    if (!env->gen_library()) {
        for (int i = 0; i < env->libs(); i++) {
#ifdef WINDOWS
            ss << " " << env->lib(i) << ".lib";
#else
            ss << " -l" << env->lib(i);
#endif
        }
        for (int i = 0; i < env->includes(); i++) {
            if (File::is_dir(env->include(i))) {
#ifdef WINDOWS
                ss << " /LIBPATH:\"" << env->include(i) << "\"";
#else
                ss << " -L" << env->include(i);  
#endif
            }
        }
    }
#ifdef LINUX
    ss << " -lm"; // Math library must be included last for some reason
#endif
#ifdef WINDOWS
    ss << " kernel32.lib";
#endif

    if (env->verbose()) {
        Stream::stout() << ss.str() << "\n";
    }
    if (system(ss.str().c_str())) { return 1; }
    if (!env->make()) {
        for (File::Ptr file = env->files(); file; file = file->next()) {
            if (file->output()) {
                remove(file->output()->string().c_str());   
            }
        }
    }

    if (!env->execute()) { return 0; }

    // Run the program, if the -e flag was specified.
    ss.str("");
#ifdef WINDOWS
    ss << exe_file << ".exe";
#else
    ss << exe_file;
#endif
    if(env->verbose()) {
        Stream::stout() << ss.str() << "\n";
    }
    int ret = system(ss.str().c_str());
    remove(exe_file.c_str());
    return ret;
}
