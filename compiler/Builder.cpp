/*
 * Copyright (c) 2010 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "Builder.hpp"
#include "CodeExpander.hpp"
#include "CCodeGenerator.hpp"
#include "IrGenerator.hpp"
#include "RegisterAllocator.hpp"
#include "Nasm64Generator.hpp"
#include "Intel64Generator.hpp"
#include "IrBlockPrinter.hpp"
#include "DeadCodeEliminator.hpp"
#include "CopyPropagator.hpp"
#include "TreePrinter.hpp"
#include "InterfaceGenerator.hpp"
#include "SemanticAnalyzer.hpp"
#include "Parser.hpp"
#include "OutputFormat.hpp"
#include "Mach64Output.hpp"
#include "Coff64Output.hpp"
#include "Elf64Output.hpp"
#include <iostream>
#include <cstdlib>

Builder::Builder(Environment* env) :
    env_(env),
    errors_(0) {

    // Initialize default includes to be used if the user-defined includes
    // do not point to the Jogo standard libraries.
    env->include(".");
#ifndef WINDOWS
    env->lib("m");
    env->include("/usr/local/lib");
    env->include("/usr/local/include/jogo");
#else
    env->lib("kernel32");
    env->lib("ws2_32");
    env->lib("mswsock");
    env->lib("wsock32");

    // Find a valid 64-bit MSVC compiler configuration

    std::string program_files = getenv("PROGRAMFILES");
    std::string program_files_x86 = getenv("PROGRAMFILES(x86)");
    env->include(program_files + "\\Jogo\\lib");
    env->include(program_files_x86 + "\\Jogo\\lib");
    env->include(program_files + "\\Jogo\\include\\jogo");
    env->include(program_files_x86 + "\\Jogo\\include\\jogo");
#endif
    process_path();

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise,  the file directly.
    Parser::Ptr parser(new Parser(env));
    if (env_->errors()) {
        errors_++;
    }
    if (env_->dump_lex()) {
        return;
    }

    // Semantic analysis/type checking phase.
	if (!env_->errors()) {
		SemanticAnalyzer::Ptr checker(new SemanticAnalyzer(env));
        // Code expansion
        if (!env_->errors()) {
            CodeExpander::Ptr expander(new CodeExpander(env, checker));
        }
	}

    if (env->dump_ast()) {
        TreePrinter::Ptr tprint(new TreePrinter(env, Stream::stout()));
        return;
    }
    if (env_->errors()) {
        errors_++;
        return;
    }

    // Final output generatation and linking phase.
    if (env_->monolithic_build()) {
        monolithic_build();
    } else {
        modular_build();
    }
}

void Builder::process_path() {
    // Read in additional include directories from JOGO_PATH
    char const* jogo_path = getenv("JOGO_PATH");
    if (!jogo_path) {
        return;
    }
    std::string include;
    for (char const* c = jogo_path; *c; ++c) {
        if (*c == PATH_SEPARATOR) {
            if (!include.empty()) {
                env_->include(include);
            }
            include.clear();
        } else {
            include += *c;
        }
    }
    if (!include.empty()) {
        env_->include(include);
    }
}

void Builder::monolithic_build() {
    // Builds all object files/inputs, etc. into one library or executable.
    // The builds result goes to the file specified by the 'output' option.
    std::stringstream ss;

    for (File::Itr file = env_->files(); file; ++file) {
        if (file->is_output_file()) {
            operator()(file);
            ss << file->jgo_file() << " ";
            if (File::is_reg(file->native_file())) {
                ss << file->o_file() << " ";
            }
        }
    }
    procs_.wait();
    errors_ += procs_.errors();
    if (!env_->link() || errors_ || env_->errors()) { return; }
    if (env_->dump_ir()) { return; }

    if (env_->root()->function(env_->name("main"))) {
#ifdef WINDOWS
        std::string exe = env_->output()+".exe";
#else
        std::string exe = env_->output();
#endif
        link(ss.str(), exe);
    } else {
        File::mkdir(File::dir_name(env_->output()));
        Stream::Ptr fout(new Stream(env_->output() + ".jgi"));
        InterfaceGenerator::Ptr iface(new InterfaceGenerator(env_, fout));
        for (File::Itr file = env_->files(); file; ++file) {
            if (file->is_output_file()) {
                iface(file.pointer());
            }
        }
        std::string lib = env_->output() + File::LIB;
        archive(ss.str(), lib);
    }

    if (!env_->execute()) { return; }
    execute(env_->output());
}

void Builder::modular_build() {
    // Build one static library per module, and build one executable per
    // executable module with a "main" function specified, using the "main"
    // function for the module as the entry point.

    // Create any out-of-date static libraries first.
    for (int i = 0; i < env_->inputs(); i++) {
        std::string name = Import::module_name(env_->input(i));
        Module::Ptr m = env_->module(env_->name(name));
        if (!m) {
            Stream::sterr() << "Module '" << env_->input(i) << "' not found\n";
            Stream::sterr()->flush();
            errors_++;
        } else if (!m->is_exe()) {
            env_->lib(m->lib_file());
            m(this);
        }
    }

    // Now, create or possibly relink exectubles.
    for (int i = 0; i < env_->inputs(); i++) {
        std::string name = Import::module_name(env_->input(i));
        Module::Ptr m = env_->module(env_->name(name));
        if (m && m->is_exe()) {
            m(this);
        }
    }
}

void Builder::operator()(Module* module) {
    // Generates the output for the module.  This function iterates over each
    // file in the module, and outputs the code for those files, then either
    // archives the results into a single static library, or performs the link
    // step if the output is an executable.
    if (env_->errors()) { return; }
    if (env_->make() && module->is_up_to_date()) {
        if (env_->verbose()) {
            Stream::stout() << "Skipping " << module->name() << "\n";
            Stream::stout()->flush();
        }
        return;
    }
    if (env_->verbose()) {
        Stream::stout() << "Building " << module->name() << "\n";
        Stream::stout()->flush();
    }

    // Set the entry point for the executable module
    env_->entry_point(module->label()->string() + "_main");

    for (int i = 0; i < module->files(); i++) {
        operator()(module->file(i));
    }
    procs_.wait();
    errors_ += procs_.errors();
    if (!env_->link() || errors_ || env_->errors()) { return; }

    if (module->function(env_->name("main"))) {
        link(module);
    } else  {
        File::mkdir(File::dir_name(module->jgi_file()));
        Stream::Ptr fout(new Stream(module->jgi_file()));
        InterfaceGenerator::Ptr iface(new InterfaceGenerator(env_, fout));
        iface->operator()(module);
        archive(module);
    }

    if (!env_->execute()) { return; }
    execute(module->exe_file());
}

void Builder::operator()(File* file) {
    // Generates the output files for the given source file.  Depending on the
    // options, this function will output the AST, IR, .jgo, or .c file.
    if (file->is_interface_file()) { return; }
    if (env_->errors()) { return; }

    // Generate native machine code, and then compile or assemble it.
    std::string const& source = file->path()->string();
    std::string const& jgo = file->jgo_file();
    if (!env_->make() || !File::is_up_to_date(source, jgo)) {
        File::mkdir(File::dir_name(file->jgo_file()));
        if (env_->verbose()) {
            Stream::stout() << "Compiling " << file->name() << "\n";
            Stream::stout()->flush();
        }
        if (env_->generator() == "Nasm64") {
            irgen(file);
            if (env_->dump_ir()) { return; }
            nasm64gen(file);
            if (!env_->assemble()) { return; }
            nasm(file->asm_file(), file->jgo_file());
        } else if (env_->generator() == "C") {
            cgen(file);
            if (!env_->assemble()) { return; }
            cc(file->jgc_file(), file->jgo_file());
        } else if (env_->generator() == "Intel64") {
            irgen(file);
            if (env_->dump_ir()) { return; }
            if (!env_->assemble()) { return; }
            intel64gen(file);
        }
    }

    // Compile any native files.  Native files have the same name as the source
    // file, but with a .c extension.
    std::string const& native = file->native_file();
    if (File::is_reg(native)) {
        std::string const& o = file->o_file();
        if (!env_->make() || !File::is_up_to_date(native, o)) {
            cc(file->native_file(), file->o_file());
        }
    }
}

void Builder::link(Module* module) {
    // Links the current module, and outputs an executable.
    std::stringstream ss;

    // Output object files and native object files.
    for (int i = 0; i < module->files(); i++) {
        ss << module->file(i)->jgo_file() <<  " ";
        if (File::is_reg(module->file(i)->native_file())) {
            ss << module->file(i)->o_file() << " ";
        }
    }

    link(ss.str(), module->exe_file());
}

void Builder::link(const std::string& in, const std::string& out) {
    // Links all the files specified in 'in' (space-delimited) and generates
    // the output file 'out'.
    // Select the correct linker command for the current OS/platform.
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "link.exe /DEBUG /SUBSYSTEM:console /NOLOGO /MACHINE:X64 /INCREMENTAL:no ";
    // N.B.: Incremental linking is not supported
#elif defined(LINUX)
    char const* compiler = getenv("CC")?getenv("CC"):"cc";
    ss << compiler << ' ';
    if (env_->debug()) {
        ss << "-g ";
    }
#elif defined(DARWIN)
    char const* compiler = getenv("CC")?getenv("CC"):"cc";
    ss << compiler << ' ';
    if (env_->debug()) {
        ss << "-g ";
    }
    ss << "-Wl,-no_pie -framework OpenGL -framework GLUT -framework Cocoa ";
#else
    #error "Unknown platform"
#endif
    env_->entry_module(out);

    procs_.wait();

#ifdef WINDOWS
    for (int i = 0; i < env_->includes(); ++i) {
        ss << "/LIBPATH:\"" << env_->include(i) << "\" ";
    }
    for (int i = 0; i < env_->libs(); ++i) {
        ss << '"' << env_->lib(i) << ".lib\" ";
    }
    ss << "jogomain.lib ";
#else
    for (int i = 0; i < env_->includes(); ++i) {
        if (File::is_dir(env_->include(i))) {
            ss << "-L\"" << env_->include(i) << "\" ";
        }
    }
    for (int i = 0; i < env_->libs(); ++i) {
        if (env_->lib(i).find('/') == 0 || env_->lib(i).find('.') == 0) {
            ss << env_->lib(i) << " ";
        } else {
            ss << "-l" << env_->lib(i) << " ";
        }
    }
    ss << "-ljogomain ";
#endif

    // If an interface file is loaded, then be sure to link in the
    // corresponding library as well.
    for (File::Itr i = env_->files(); i; ++i) {
        if (i->is_interface_file()) {
            ss << '"' << i->input(File::LIB) << "\" ";
        }
    }

    // Output link options for libraries and module dependencies.
#ifdef WINDOWS
    ss << in << " /OUT:" << out;
#else
    ss << in << "-o " << out << " ";
    // Dependencies must be linked after the dependent .o files.  Linux gcc is
    // stupid about this, so we link all the libraries both before and after
    // all the .o files in case there are circular dependencies.
    for (int i = 0; i < env_->libs(); i++) {
        if (env_->lib(i).find('/') == 0 || env_->lib(i).find('.') == 0) {
            ss << env_->lib(i) << " ";
        } else {
            ss << "-l" << env_->lib(i) << " ";
        }
    }
    ss << "-ljogomain ";

    for (File::Itr i = env_->files(); i; ++i) {
        if (i->is_interface_file()) {
            ss << i->input(File::LIB) << " ";
        }
    }
#endif

    if (env_->verbose()) {
        Stream::stout() << ss.str() << "\n";
        Stream::stout()->flush();
    }
    File::mkdir(File::dir_name(out).c_str());
    if (system(ss.str().c_str())) {
        errors_++;
    }
}

void Builder::archive(Module* module) {
    // Links the current module, and outputs an archive and interface file.
    std::stringstream ss;
    for (int i = 0; i < module->files(); i++) {
        ss << module->file(i)->jgo_file() <<  " ";
        if (File::is_reg(module->file(i)->o_file())) {
            ss << module->file(i)->o_file() << " ";
        }
    }
    archive(ss.str(), module->lib_file());
}

void Builder::archive(const std::string& in, const std::string& out) {
    // Output object files and native object files
    std::stringstream ss;
    remove(out.c_str());

    // Select the correct archive program for the current OS/platform.
#if defined(WINDOWS)
    ss << "lib.exe /SUBSYSTEM:console /MACHINE:X64 /NOLOGO /OUT:" << out << " " << in;
    if (env_->no_default_libs()) {
        ss << "build\\runtime\\Coroutine.Intel64.obj";
    }
#elif defined(DARWIN)
    ss << "ar rcs " << out << " " << in;
    if (env_->no_default_libs()) {
        ss << "build/runtime/Coroutine.Intel64.o";
    }
#elif defined(LINUX)
    ss << "ar rcs " << out << " " << in;
    if (env_->no_default_libs()) {
        ss << "build/runtime/Coroutine.Intel64.o";
    }
#endif
    if (env_->verbose()) {
        Stream::stout() << ss.str() << "\n";
        Stream::stout()->flush();
    }
    File::mkdir(File::dir_name(out).c_str());
    if (system(ss.str().c_str())) {
        errors_++;
    }
}

void Builder::cgen(File* file) {
    // Generates C code for all functions/classes in 'file.'  Outputs to a
    // temporary file if this is an intermediate step; otherwise, outputs
    // to a named file in the build directory.
    //CCodeGenerator::Ptr c(new CCodeGenerator(env_));
    CCodeGenerator::Ptr c(new CCodeGenerator(env_));
    c->out(new Stream(file->jgc_file()));
    if (c->out()->error()) {
        std::string msg = c->out()->message();
        Stream::sterr() << file->asm_file() << msg << "\n";
        Stream::sterr()->flush();
        Stream::stout()->flush();
        errors_++;
        return;
    }
    c->operator()(file);
}

void Builder::irgen(File* file) {
    // Generates code for all basic blocks using the Jogo intermediate
    // represenation.  Also performs optimizations, if enabled by the
    // environment options.
    Machine::Ptr machine = Machine::intel64();
    IrGenerator::Ptr bgen(new IrGenerator(env_, machine));
    RegisterAllocator::Ptr alloc(new RegisterAllocator(env_, machine));
    IrBlockPrinter::Ptr bprint(new IrBlockPrinter(env_, machine));
    Stream::Ptr out = Stream::stout();
    out->machine(machine);
    bprint->out(out);

    bgen->operator()(file);
    if (env_->dump_ir() && env_->verbose()) {
        bprint->operator()(file);
    }
    if (env_->optimize()) {
        CopyPropagator::Ptr copy(new CopyPropagator(env_, machine));
        DeadCodeEliminator::Ptr elim(new DeadCodeEliminator(env_, machine));
        copy->operator()(file);
        elim->operator()(file);
    }

    if (env_->dump_ir() && env_->verbose()) {
        bprint->operator()(file);
    }
    alloc->operator()(file);

    DeadCodeEliminator::Ptr elim(new DeadCodeEliminator(env_, machine));
    elim->operator()(file);

    if (env_->dump_ir()) {
        bprint->operator()(file);
    }
}

void Builder::nasm64gen(File* file) {
    // Generates NASM Intel 64 code for all functions/classes in 'file.'
    // Outputs to a temporary file if this is an intermediate step; otherwise,
    // outputs to a named file in the build directory.
    Nasm64Generator::Ptr nasm64gen(new Nasm64Generator(env_));
    nasm64gen->out(new Stream(file->asm_file()));
    if (nasm64gen->out()->error()) {
        std::string msg = nasm64gen->out()->message();
        Stream::sterr() << file->asm_file() << msg << "\n";
        Stream::sterr()->flush();
        Stream::stout()->flush();
        errors_++;
        return;
    }
    nasm64gen->operator()(file);
}

void Builder::intel64gen(File* file) {
    // Output Intel64 (x86-64) machine code directly to the correct object file
    // format for the current system.
    Intel64Generator::Ptr intel64gen(new Intel64Generator(env_));
    intel64gen->out(new Stream(file->jgo_file()));
    if (intel64gen->out()->error()) {
        std::string msg = intel64gen->out()->message();
        Stream::sterr() << file->jgo_file() << msg << "\n";
        Stream::sterr()->flush();
        Stream::stout()->flush();
        errors_++;
        return;
    }
#if defined(DARWIN)
    OutputFormat::Ptr format(new Mach64Output);
#elif defined(WINDOWS)
    OutputFormat::Ptr format(new Coff64Output(env_));
#elif defined(LINUX)
    OutputFormat::Ptr format(new Elf64Output);
#endif
    format->file(file);
    intel64gen->format(format);
    intel64gen->operator()(file);
}

void Builder::cc(const std::string& in, const std::string& out) {
    // Compiles a single C source file, and outputs the result to 'out.'
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "cl.exe " << in << " /Z7 /MD /nologo /c /Fo\"" << out << "\"";
    if (env_->optimize()) {
        ss << " /O2";
    }
    ss << " /DCOROUTINE_STACK_SIZE=" << COROUTINE_STACK_SIZE;
    if (!env_->verbose()) {
        ss << " > NUL";
    }
#else
    char const* compiler = getenv("CC")?getenv("CC"):"cc";
#if defined(LINUX)
    ss << compiler << ' ' << in << " -c -o " << out;
#elif defined(DARWIN)
    ss << compiler << ' ' << in << " -c -o " << out;
#else
    #error "Unknown platform"
#endif
    ss << " -std=c99 -Wall -Wno-missing-braces -Werror ";
    if (env_->optimize()) {;
        ss << " -O2";
    } else {
        ss << " -O0";
    }
    if (env_->debug()) {
        ss << " -g";
    }
    ss << " -DCOROUTINE_STACK_SIZE=" << COROUTINE_STACK_SIZE;
#ifdef DARWIN
    ss << " -D_XOPEN_SOURCE=700";
#else
    ss << " -D_GNU_SOURCE";
#endif
#endif

#if defined(WINDOWS)
    ss << " /DWINDOWS";
#elif defined(DARWIN)
    ss << " -DDARWIN";
#elif defined(LINUX)
    ss << " -DLINUX";
#else
    #error "Unknown platform"
#endif

    for (int i = 0; i < env_->includes(); i++) {
        if (File::is_dir(env_->include(i))) {
#if defined(WINDOWS)
            ss << " /I \"" << env_->include(i) << "\"";
#else
            ss << " -I " << env_->include(i);
#endif
        }
    }
    if (env_->verbose()) {
        Stream::stout() << ss.str() << "\n";
        Stream::stout()->flush();
    }
    procs_.process(ss.str());
}

void Builder::nasm(const std::string& in, const std::string& out) {
    // Assembles a single NASM assembly file, and outputs the result to 'out.'
    std::stringstream ss;
#if defined(WINDOWS)
    ss << "nasm -fwin64 ";
#elif defined(LINUX)
    ss << "nasm -felf64 ";
#elif defined(DARWIN)
    ss << "/usr/local/bin/nasm -fmacho64 ";
#endif
    ss << in << " -o " << out;
    if (env_->verbose()) {
        Stream::stout() << ss.str() << "\n";
        Stream::stout()->flush();
    }
    procs_.process(ss.str());
}

void Builder::execute(const std::string& exe) {
    // Executes the program generated by the given module.
    std::stringstream ss;
#ifdef WINDOWS
    ss << exe << ".exe";
#else
    if (exe[0] != '/') {
        ss << "./";
    }
    ss << exe << std::endl;
#endif
    if (system(ss.str().c_str())) {
        errors_++;
    }
}
