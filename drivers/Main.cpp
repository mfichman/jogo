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
#include "Iterator.hpp"
#include "ArgParser.hpp"

#include <iostream>
#include <cstdlib>

#ifdef WINDOWS
#include <crtdbg.h>
#endif

Environment::Ptr env(new Environment);
ArgParser::Ptr argp;

void parse_option(std::string const& flag) {
    // Parses an option and all the corresponding arguments to that option.
    if ("verbose" == flag) {
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
    } else if ("debug" == flag) {
        env->debug(true);
    } else if ("dump-ir" == flag) {
        env->dump_ir(true);
    } else if ("dump-ast" == flag) {
        env->dump_ast(true);
    } else if ("dump-lex" == flag) {
        env->dump_lex(true);
    } else if ("dump-liveness" == flag) {
        env->dump_liveness(true);
    } else if ("dump-regalloc" == flag) {
        env->dump_regalloc(true);
    } else if ("dump-reggraph" == flag) {
        env->dump_reggraph(true);
    } else if ("execute" == flag) {
        env->execute(true);
    } else if ("no-default-libs" == flag) {
        env->no_default_libs(true);
        // Debugger flag that prevents default modules from getting
        // loaded automatically by the parser.
    } else if ("include" == flag) {
        env->include(argp->required_arg()); 
    } else if ("output" == flag) {
        env->output(argp->required_arg());
    } else if ("build-dir" == flag) {
        env->build_dir(argp->required_arg());
    } else if ("library" == flag) {
        env->lib(argp->required_arg());
    } else if ("generator" == flag) {
        env->generator(argp->required_arg());
    } else {
        argp->bad_option(flag);
    }
}

void parse_short_option(std::string const& flag) {
    // Parses a short-form option ('-x')
    switch (flag[0]) {
    case 'i': parse_option("include"); break;
    case 'd': parse_option("debug"); break;
    case 'v': parse_option("verbose"); break;
    case 'a': parse_option("assembly"); break;
    case 'c': parse_option("compile"); break;
    case 'l': parse_option("library"); break;
    case 'o': parse_option("output"); break;
    case 'e': parse_option("execute"); break;
    case 'm': parse_option("make"); break;
    case 'h': parse_option("help"); break;
    case 'g': parse_option("generator"); break;
    default: argp->bad_short_option(flag); break;
    }
}

void parse_options() {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argp->usage(
        "Usage: jogo [OPTIONS] FILE...\n\n"
        "Compiles Jogo source files into a library or executable.\n\n"
        "   -a, --assembly       Compile, but do not assemble or link.\n"
        "   -c, --compile        Compile and assemble, but do not link.\n"
        "   -e, --execute        Execute program as a script.\n"
        "   -d, --debug          Emit debug information.\n"
        "   -l, --library LIB    Compile and link with native library LIB.\n"
        "   -i, --include DIR    Add the directory DIR to the search path.\n"
        "   -o, --output FILE    Write compiler output to FILE.\n"
        "   -m, --make           Compile input files and out-of-date dependencies.\n"
        "   -h, --help           Print this help message.\n"
        "   -v, --verbose        Print extra information during compilation.\n"
        "   -g, --generator GEN  Use code generator GEN.\n"
        "   --build-dir DIR      Output directory for object files.\n"
        "   --dump-ir            Output the intermediate representation.\n"
        "   --dump-ast           Output the abstract syntax tree.\n"
        "   --dump-liveness      Output liveness info when printing the IR.\n"
        "   --dump-regalloc      Dump register allocations.\n"
        "   --dump-reggraph      Dump register interference graphs.\n"
        "   --version            Print the compiler version number.\n");

    for (ArgToken tok = argp->next(); tok; tok = argp->next()) {
        // Convert abbreviated flags into the longer descriptive form (e.g.,
        // convert -p to --path)
        if (ArgToken::SHORT == tok.type()) {
            parse_short_option(tok.value());
        } else if (ArgToken::LONG == tok.type()) {
            parse_option(tok.value());
        } else {
            env->input(tok.value());
        }
    } 

    std::string gen = env->generator();
    if (gen != "Intel64" && gen != "C" && gen != "Nasm64") {
        argp->error("Invalid code generator (options: Intel64, NAsm64, C)"); 
    }
    if (!env->inputs()) {
        argp->error("No input files.");
    }
}

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    argp = new ArgParser(env, argc, argv);
    parse_options();
    Builder::Ptr builder(new Builder(env));
    exit(builder->errors());
    return 1;
}
