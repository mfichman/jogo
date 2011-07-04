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

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            last_flag_ = argv[i] + 2;
        } else if (argv[i][0] == '-') {
            std::string arg = argv[i] + 1;
            if ("I" == arg) last_flag_ = "include";
            if ("o" == arg) last_flag_ = "output";
        } else if ("include" == last_flag_) {
            env->include(argv[i]); 
            last_flag_ = "";
        } else if ("output" == last_flag_) {
            assert(!"Flag '--output' not implemented");
            last_flag_ = "";
        } else if (last_flag_.empty()) {
            env->input(argv[i]);
        }
    } 
}
