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

#include "ArgParser.hpp"
#include <iostream>
#include <cstdlib>

ArgParser::ArgParser(Environment* env, int argc, char** argv) :
    env_(env),
    argindex_(0),
    argc_(argc),
    argv_(argv) {

    env->program_path(next().value());
#ifdef WINDOWS
    _CrtSetReportHook(report_hook);
#endif
}

ArgToken ArgParser::next() {
    // Returns the next argument, or the empty string.
    if (argindex_ >= argc_) {
        return ArgToken();
    }
    std::string val(argv_[argindex_++]);
    if (val == "-h" || val == "--help") {
        print_usage();
        exit(0);
    } else if (val == "--version") {
        print_version();
        exit(0);
    } else if (val[0] == '-' && val[1] != '-') {
        return ArgToken(ArgToken::SHORT, val.substr(1));
    } else if (val[0] == '-') {
        return ArgToken(ArgToken::LONG, val.substr(2));
    } else {
        return ArgToken(ArgToken::ARG, val);
    }
}

std::string ArgParser::required_arg() {
    // Parses the next argument, and checks to make sure it is a positional
    // argument, not an option.
    ArgToken tok = next();
    if (ArgToken::ARG != tok.type()) {
        print_usage();
        std::cerr << "Option requires an argument\n" << std::endl;
        exit(1);
    }
    return tok.value();
}


void ArgParser::usage(std::string const& msg) {
    // Sets the usage message
    usage_ = msg;
}

void ArgParser::print_usage() {
    std::cerr << usage_ << std::endl;
}

void ArgParser::print_version() {
    if (env_->verbose()) {
        std::cerr << env_->program_path() << " v" << VERSION;
        std::cerr << ", compiled on " << __DATE__ << " " << __TIME__ << std::endl;
        std::cerr << BRANCH << ", " << REVISION << std::endl;
    } else {
        std::cerr << "v" << VERSION;
    }
}

void ArgParser::bad_option(std::string const& flag) {
    print_usage();
    std::cerr << "Unknown flag '--" << flag << "'\n" << std::endl;
    exit(1);
}

void ArgParser::bad_short_option(std::string const& flag) {
    print_usage();
    std::cerr << "Unknown flag '-" << flag << "'\n" << std::endl;
    exit(1);
}

void ArgParser::error(std::string const& msg) {
    print_usage();
    std::cerr << "Error: " << msg << '\n' << std::endl;
    exit(1);
}

#ifdef WINDOWS
int ArgParser::report_hook(int, char* message, int*) {
    // Prevents an annoying assertion dialog from popping up in debug mode if
    // the process crashes.
    std::cerr << message << std::endl;
    int* zero = 0;
    *zero;
    return true;
}
#endif

