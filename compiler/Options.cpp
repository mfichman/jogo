/*
 * Copyright (c) 2011 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Options.hpp"
#include "Environment.hpp"
#include "Stream.hpp"
#include <cstdlib>

Options::Options(Environment* env, int argc, char** argv) {
    // Parse command line options, and update the environment to match the 
    // specified options.

    env->program_path(argv[0]);
    program_name_ = argv[0];
    Stream::Ptr err = Stream::sterr();
    std::string flag;
    std::vector<std::string> args;

    for (int i = 1; i < argc; i++) {
        // Convert abbreviated flags into the longer descriptive form (e.g., 
        // convert -p to --path)
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            flag = argv[i] + 1;
            if ("i" == flag) { flag = "include"; }
            else if ("v" == flag) { flag = "verbose"; }
            else if ("a" == flag) { flag = "assembly"; }
            else if ("c" == flag) { flag = "compile"; }
            else if ("l" == flag) { flag = "library"; }
            else if ("o" == flag) { flag = "output"; }
            else if ("e" == flag) { flag = "execute"; }
            else if ("m" == flag) { flag = "make"; }
            else if ("h" == flag) { flag = "help"; }
            else if ("g" == flag) { flag = "generator"; }
            else {
                print_usage();
                err << "Unknown flag '" << argv[i] << "'\n\n";
                err->flush();
                exit(1);
            }
            args.clear();
        } else if (argv[i][0] == '-') {
            flag = argv[i] + 2;
            args.clear();
        } else if (flag.empty()) {
            env->input(argv[i]);
        } else {
            args.push_back(argv[i]);    
        }
    
        switch (args.size()) {
        // Zero-parameter arguments
        case 0:
            if ("help" == flag) {
                print_usage();
                exit(0);
            } else if ("verbose" == flag) {
                env->verbose(true);
            } else if ("optimize" == flag) {
                env->optimize(true);
            } else if ("assembly" == flag) {
                env->assemble(false);
                env->link(false);
            } else if ("compile" == flag) {
                env->link(false);
            } else if ("make" == flag) {
                env->make(true);
            } else if ("dump-ir" == flag) {
                env->dump_ir(true);
            } else if ("dump-ast" == flag) {
                env->dump_ast(true);
            } else if ("dump-liveness" == flag) {
                env->dump_liveness(true);
            } else if ("dump-regalloc" == flag) {
                env->dump_regalloc(true);
            } else if ("execute" == flag) {
                env->execute(true);
            } else if ("version" == flag) {
                print_version();
                exit(0); 
            } else if ("no-default-libs" == flag) {
                env->no_default_libs(true);
                // Debugger flag that prevents default modules from getting
                // loaded automatically by the parser.
            } else {
                break;
            }
            args.clear();
            flag = "";
            break;
        // Single-parameter arguments
        case 1:
            if ("include" == flag) {
                env->include(argv[i]); 
            } else if ("output" == flag) {
                env->output(argv[i]);
            } else if ("build-dir" == flag) {
                env->build_dir(argv[i]);
            } else if ("library" == flag) {
                env->lib(argv[i]);
            } else if ("generator" == flag) {
                env->generator(argv[i]);
            } else {
                break;
            }
            args.clear();
            flag = "";
            break;
        }
    } 

    std::string gen = env->generator();
    if (gen != "Intel64" && gen != "C" && gen != "Nasm64") {
        print_usage();
        err << "Invalid code generator (options: Intel64, Nasm64, C)\n\n";
        err->flush();
        exit(1);
    }

    if (!env->inputs()) {
        print_usage();
        err << "No input files.\n\n";
        err->flush(); 
        exit(1);
    }

    err->flush();
}

void Options::print_version() {
    Stream::Ptr out = Stream::stout();
    out << "Jogo version " << VERSION << ", compiled on ";
    out << __DATE__ << " " << __TIME__ << "\n";
    out->flush();
}

void Options::print_usage() {
    Stream::Ptr out = Stream::stout();
    out << "Usage: jogo [OPTIONS] FILE...\n\n";
    out << "   -a, --assembly       Compile, but do not assemble or link.\n";
    out << "   -c, --compile        Compile and assemble, but do not link.\n";
    out << "   -e, --execute        Execute program as a script.\n";
    out << "   -l, --library LIB    Compile and link with native library LIB.\n";
    out << "   -o, --output FILE    Write compiler output to FILE.\n";
    out << "   -i, --include DIR    Add the directory DIR to the search path.\n";
    out << "   -m, --make           Compile input files and out-of-date dependencies.\n";
    out << "   -h, --help           Print this help message.\n";
    out << "   -v, --verbose        Print extra information during compilation.\n";
    out << "   -g, --generator GEN  Use code generator GEN.\n";
    out << "   --build-dir DIR      Output directory for object files.\n";
    out << "   --dump-ir            Output the intermediate representation.\n";
    out << "   --dump-ast           Output the abstract syntax tree.\n";
    out << "   --dump-liveness      Output liveness info when printing the IR.\n";
    out << "   --version            Print the compiler version number.\n";
    out << "\n";
    out->flush();
}
