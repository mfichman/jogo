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
#include "Builder.hpp"
#include "Options.hpp"
#include "Iterator.hpp"

#include <iostream>
#include <cstdlib>

#ifdef WINDOWS
#include <crtdbg.h>
#endif

Environment::Ptr env(new Environment);
int argindex = 0;
int argcg = 0;
char** argvg = 0;

void print_usage() {
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

std::string parse_next() {
    // Returns the next argument in the argument list
    if (argindex >= argcg) {
        return std::string(); 
    } else {
        return argvg[argindex++];
    } 
}

std::string parse_required_arg() {
    std::string arg = parse_next();
    if (arg.empty() || arg[0] == '-') {
        print_usage();
        Stream::Ptr err = Stream::sterr();
        err << "Option requires an argument\n\n";
        err->flush();
        exit(1);
    }
	return arg;
}

void print_version() {
    Stream::Ptr out = Stream::stout();
    out << "jogo version " << VERSION << ", compiled on ";
    out << __DATE__ << " " << __TIME__ << "\n";
    out->flush();
}

void parse_option(std::string const& flag) {
    // Parses an option and all the corresponding arguments to that option.
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
    } else if ("include" == flag) {
        env->include(parse_required_arg()); 
    } else if ("output" == flag) {
        env->output(parse_required_arg());
    } else if ("build-dir" == flag) {
        env->build_dir(parse_required_arg());
    } else if ("library" == flag) {
        env->lib(parse_required_arg());
    } else if ("generator" == flag) {
        env->generator(parse_required_arg());
    } else {
        print_usage();
        Stream::Ptr err = Stream::sterr();
        err << "Unknown flag '--" << flag << "'\n\n";
        err->flush();
        exit(1);
    }
}

void parse_simple_option(std::string const& flag) {
    // Parses a short-form option ('-x')
    if ("i" == flag) { parse_option("include"); }
    else if ("v" == flag) { parse_option("verbose"); }
    else if ("a" == flag) { parse_option("assembly"); }
    else if ("c" == flag) { parse_option("compile"); }
    else if ("l" == flag) { parse_option("library"); }
    else if ("o" == flag) { parse_option("output"); }
    else if ("e" == flag) { parse_option("execute"); }
    else if ("m" == flag) { parse_option("make"); }
    else if ("h" == flag) { parse_option("help"); }
    else if ("g" == flag) { parse_option("generator"); }
    else {
        print_usage();
        Stream::Ptr err = Stream::sterr();
        err << "Unknown flag '-" << flag << "'\n\n";
        err->flush();
        exit(1);
    }
}

void parse_options(int argc, char** argv) {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argcg = argc;
    argvg = argv;

    env->program_path(parse_next());
    Stream::Ptr err = Stream::sterr();
    std::string arg = parse_next();
    while (!arg.empty()) {
        // Convert abbreviated flags into the longer descriptive form (e.g.,
        // convert -p to --path)
        if (arg[0] == '-' && arg[1] != '-') {
            parse_simple_option(arg.substr(1));
        } else if (arg[0] == '-') {
            parse_option(arg.substr(2));
        } else {
            env->input(arg);
        } 
        arg = parse_next();
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

#ifdef WINDOWS
int report_hook(int, char* message, int*) {
    // Prevents an annoying assertion dialog from popping up in debug mode if
    // the process crashes.
    std::cerr << message << std::endl;
    int* zero = 0;
    *zero;
    return true;
}
#endif

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
#ifdef WINDOWS
    _CrtSetReportHook(report_hook);
#endif
    parse_options(argc, argv);
    Builder::Ptr builder(new Builder(env));
    exit(builder->errors());
    return 1;
}
