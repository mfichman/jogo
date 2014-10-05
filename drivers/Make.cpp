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
#include "ArgParser.hpp"
#include "Builder.hpp"
#include <fstream>

Environment::Ptr env(new Environment);
ArgParser::Ptr argp;

void clean(std::string const& dir) {
    for(File::Iterator i(dir);i;++i) {
        std::string const path = dir + FILE_SEPARATOR + (*i);
        if((*i) == "." || (*i) == "..") {
            // Do nothing
        } else if(File::is_dir(path)) {
            clean(path);
        } else {
            if (env->verbose()) {
                std::cout << "Removing " << path << std::endl;
            }
            File::unlink(path); 
        }
    }
}

void parse_option(std::string const& flag) {
    if ("library" == flag) {
        env->lib(argp->required_arg());
    } else if ("include" == flag) {
        env->include(argp->required_arg()); 
    } else if ("generator" == flag) {
        env->generator(argp->required_arg());
    } else if ("debug" == flag) {
        env->debug(true);
    } else if ("optimize" == flag) {
        env->optimize(true);
    } else if ("verbose" == flag) {
        env->verbose(true);
    } else if ("clean" == flag) {
        clean(env->build_dir());
        clean("bin");
        clean("lib");
        exit(0);
    } else if ("reset" == flag) {
        unlink(".jgmake");
        exit(0);
    } else {
        argp->bad_option(flag);
    }
}

void parse_short_option(std::string const& flag) {
    // Parses a short-form option ('-x')
    switch (flag[0]) {
    case 'l': parse_option("library"); break;
    case 'i': parse_option("include"); break;
    case 'g': parse_option("generator"); break;
    case 'd': parse_option("debug"); break;
    case 'o': parse_option("optimize"); break;
    case 'v': parse_option("verbose"); break;
    case 'c': parse_option("clean"); break;
    case 'r': parse_option("reset"); break;
    default: argp->bad_short_option(flag); break;
    }
}

void parse_options() {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argp->usage(
        "Usage: jgmake [OPTIONS] PACKAGE...\n\n"
        "Builds a Jogo project from the current working directory.  Source files should\n"
        "be under ./src; output files go to ./build, ./lib, ./bin, and ./doc.  Options\n"
        "given to jgmake are recorded.  After an option is set, it will remain set the\n"
        "next time jgmake is run.\n\n"
        "   -l, --library LIB    Compile and link with native library LIB.\n"
        "   -i, --include DIR    Add the directory DIR to the search path.\n"
        "   -d, --debug          Emit debug information.\n"
        "   -o, --optimize       Enable compiler optimizations.\n"
        "   -g, --generator GEN  Use code generator GEN.\n"
        "   -h, --help           Print this help message.\n"
        "   -v, --verbose        Print extra information during compilation.\n"
        "   -r, --reset          Erase all build options.\n"
        "   -c, --clean          Remove all output directories.\n"
        "   --version            Print the program version number.\n");

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
        argp->error("Invalid code generator (options: Intel64, Nasm64, C)"); 
    }
}

void load_options() {
    // Loads options from the options file.  FIXME: This settings file loader
    // is pretty haggard.  It should be replaced with JSON.
    std::ifstream opts(".jgmake");
    int c = opts.get();
    while (c != '\n' && !opts.fail()) {
        std::string include;
        while (c != ',' && c != '\n' && !opts.fail()) {
            include += c;
            c = opts.get();
        }
        env->include(include);
        c = opts.get(); 
    }
    c = opts.get();
    while (c != '\n' && !opts.fail()) {
        std::string lib;
        while (c != ',' && c != '\n' && !opts.fail()) {
            lib += c;
            c = opts.get();
        }
        env->lib(lib);
        c = opts.get(); 
    }
}

void save_options() {
    // Saves options to the options file.
    std::ofstream opts(".jgmake");
    for (int i = 0; i < env->includes(); ++i) {
        opts << env->include(i) << ",";
    }
    opts << std::endl;
    for (int i = 0; i < env->libs(); ++i) {
        opts << env->lib(i) << ",";
    }
    opts << std::endl;
}

int main(int argc, char** argv) {
    // This program recursively finds and builds all files in the source
    // directory, and then generates the output in the lib/bin directory.
    env->workspace_load();
    env->output(".");
    env->make(true);
    env->monolithic_build(false);

    argp = new ArgParser(env, argc, argv);
    load_options(); 
    parse_options();
    save_options();

    Builder::Ptr builder(new Builder(env));
    return builder->errors();
}
