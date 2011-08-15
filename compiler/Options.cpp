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

    program_name_ = argv[0];
    Stream::Ptr err = Stream::stderr();
    std::string flag;
    std::vector<std::string> args;
    if (argc < 2) {
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        // Convert abbreviated flags into the longer descriptive form (e.g., 
        // convert -p to --path)
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            flag = argv[i] + 1;
            if ("p" == flag) { flag = "path"; }
            else if ("a" == flag) { flag = "assembly"; }
            else if ("c" == flag) { flag = "compile"; }
            else if ("d" == flag) { flag = "dump"; }
            else if ("l" == flag) { flag = "library"; }
            else if ("o" == flag) { flag = "output"; }
            else if ("h" == flag) { flag = "help"; }
            else {
                err << "Unknown flag '" << argv[i] << "'\n";
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
            } else if ("assembly" == flag) {
                assert(!"Flag '--assembly' not implemented");
            } else if ("compile" == flag) {
                assert(!"Flag '--compile' not implemented");
            } else {
                break;
            }
            args.clear();
            flag = "";
            break;
        // Single-parameter arguments
        case 1:
            if ("path" == flag) {
                env->include(argv[i]); 
            } else if ("output" == flag) {
                env->output(argv[i]);
            } else if ("dump" == flag) {
                assert(!"Flag '--dump' not implemented");
            } else if ("library" == flag) {
                assert(!"Flag '--library' not implemented");
            } else {
                break;
            }
            args.clear();
            flag = "";
            break;
        }
    } 

    err->flush();
}

void Options::print_usage() {
    Stream::Ptr out = Stream::stdout();
    out << "Usage: " << program_name_ << " [OPTIONS] FILE...\n\n";
    out << "   -a, --assembly       Compile, but do not assemble or link.\n";
    out << "   -c, --compile        Compile and assemble, but do not link.\n";
    out << "   -d, --dump FILE      Dump the AST to FILE.\n";
    out << "   -h, --help           Print this help message.\n";
    out << "   -l, --library FILE   Generate a shared library and write it to FILE.\n";
    out << "   -o, --output FILE    Write compiler output to FILE.\n";
    out << "   -p, --path DIR       Add the directory DIR to the source search path.\n";
    out << "\n";
    out->flush();
    exit(0);
}
