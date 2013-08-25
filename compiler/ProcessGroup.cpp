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
#include <cassert>

void ProcessGroup::process(std::string const& args) {
#ifdef WINDOWS
/*
    if (system(args.c_str())) {
        errors_++;
    }
*/
    PROCESS_INFORMATION procinfo;
    STARTUPINFO sinfo;
    memset(&procinfo, 0, sizeof(procinfo));
    memset(&sinfo, 0, sizeof(sinfo));
    std::string args2 = args;
    // N.B.: CreateProcess modifies the lpCommandLine parameter
    BOOL ret = CreateProcess(
        NULL,
        (LPSTR)args2.c_str(),
        NULL,
        NULL,
        false,
        0,
        NULL,
        NULL,
        &sinfo,
        &procinfo);
    if (!ret) {
        std::cerr << "Error: Couldn't start subprocess: " << args << std::endl;
        errors_++;
    } else {
        CloseHandle(procinfo.hThread); 
        process_.push_back(procinfo.hProcess);
    }
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
    for (size_t i = 0; i < process_.size(); ++i) {
#ifdef WINDOWS
        DWORD ret = WaitForSingleObject(process_[i], INFINITE); 
        if (ret != WAIT_OBJECT_0) {
            std::cerr << "WaitForSingleObject() error: " << GetLastError() << std::endl;
            abort();
        }
        DWORD exit_code = 0;
        if(!GetExitCodeProcess(process_[i], &exit_code)) {
            std::cerr << "GetExitCodeProcess() error" << std::endl;
            abort();
        } 
        assert(exit_code != STILL_ACTIVE && "Process is still running"); 
        if (exit_code != 0) {
            errors_++;
        }
#else
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
#endif
    }
    process_.clear();
}

