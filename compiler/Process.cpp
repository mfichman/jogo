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

#include "Process.hpp"
#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

int Process::status() const {
    // Runs the process using the specified command and arguments, and returns
    // the process exit code.
#ifdef WINDOWS
    std::string args;
    for (int i = 0; i < arg_.size(); i++) {
        args += arg_[i];
    }
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    if (!CreateProcess(command_.c_str(), args.c_str(), NULL, NULL, false, 0, 
            NULL, &sinfo, &pinfo)) {
        return 1; 
    }
    assert(!"Not implemented");
#else
    pid_t pid = fork();    
    if (pid < 0) {
        return pid; 
    } else if (pid) {
        int status;
        waitpid(pid, &status, 0);
        return status;
    } else {
        std::vector<const char*> arg; 
        for (int i = 0; i < arg_.size(); i++) {
            arg.push_back(arg_[i].c_str());
        }
        arg.push_back(0);
        execvp(command_.c_str(), (char**)arg.front());
        return 0;
    }
#endif 
}
