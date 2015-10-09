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

#pragma once
#define NOMINMAX

#include <string>
#include <vector>

#if defined(WINDOWS)
#include <windows.h>
#elif defined(DARWIN)
#include <unistd.h>
#else
#include <sys/wait.h>
#endif
#include <cstdlib>

/* Tracks the execution of a group of processes */
class ProcessGroup {
public:
    ProcessGroup() : errors_(0) {}
    void process(std::string const& args);
    void wait();
    int errors() const { return errors_; }

private:
#ifdef WINDOWS
    std::vector<HANDLE> process_;
#else
    std::vector<pid_t> process_;
#endif
    int errors_;
};
