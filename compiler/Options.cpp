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

Options::Options(Environment* env, int argc, char** argv) {

    program_name_ = argv[0];
    std::string flag;
    std::vector<std::string> args;
    if (argc < 2) {
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            flag = argv[i] + 1;
            if ("p" == flag) { flag = "path"; }
            else if ("o" == flag) { flag = "output"; }
            else if ("h" == flag) { flag = "help"; }
            else {
                std::cerr << "Unknown flag '" << argv[i] << "'" << std::endl;
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
        case 0:
            if ("help" == flag) {
                print_usage();
                exit(0);
            } else if ("assembly" == flag) {
                assert(!"Flag '--assembly' not implemented");
            }
            break;
        case 1:
            if ("path" == flag) {
                env->include(argv[i]); 
            } else if ("output" == flag) {
                assert(!"Flag '--output' not implemented");
            } else if ("dump" == flag) {
                assert(!"Flag '--dump' not implemented");
            }
            break;
        case 2:
            args.clear();
        }
    } 
}

void Options::print_usage() {
    std::cout << "Usage: " << program_name_ << " [OPTIONS] FILE...\n\n";
    std::cout << "   -a, --assembly       Skip link stage and output assembly.\n";
    std::cout << "   -d, --dump FILE      Dump the AST to FILE.\n";
    std::cout << "   -h, --help           Print this help message.\n";
    std::cout << "   -l, --library FILE   Generate a shared library and write it to FILE.\n";
    std::cout << "   -o, --output FILE    Write compiler output to FILE.\n";
    std::cout << "   -p, --path DIR       Add the directory DIR to the source search path.\n";
    std::cout << std::endl;
    exit(0);
}
