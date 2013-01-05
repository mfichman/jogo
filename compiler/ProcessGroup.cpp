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

#include "ProcessGroup.hpp"
#include <iostream>

void ProcessGroup::process(std::string const& args) {
#ifdef WINDOWS
#error Not implemented
#else
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: Couldn't fork" << std::endl;
        abort();
    } else if (pid == 0) {
        if (system(args.c_str())) {
            exit(1);
        } else {
            exit(0);
        }
    } else {
        process_.push_back(pid);
    }
#endif
}

void ProcessGroup::wait() {
    // Wait for all child processes to complete.  Records the number of child
    // errors; these errors are stored in the 'errors' attribute.
#ifdef WINDOWS
#error Not implemented
#else
    for (size_t i = 0; i < process_.size(); ++i) {
        while (true) {
            int status = 0; 
            pid_t pid = waitpid(process_[i], &status, 0);
            if (pid < 0) { 
                errors_++;
                break;
            } else if (WIFEXITED(status)) {
                if (WEXITSTATUS(status)) { errors_++; }
                break;
            } else if (WIFSIGNALED(status)) {
                errors_++;
                break;
            }
        }
    }
#endif
}

