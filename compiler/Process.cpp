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

Process::Process(const std::string& command) : 
#ifdef WINDOWS
    proc_(0),
    handle_(0),
#else
    pid_(0),
#endif  
    command_(command) {

}

int Process::status() { 
    // Returns the process exit status, after waiting for the process to 
    // finish executing.
    exec();
#ifdef WINDOWS
    assert(!"Not implemented");
#else
    int status;
    waitpid(pid_, &status, 0);
    return WEXITSTATUS(status);
#endif

}

Stream::Ptr Process::pipe() {
    exec();
    return pipe_;
}

void Process::exec() {
    // Runs the process using the specified command and arguments, and returns
    // the process exit code.
#ifdef WINDOWS
    if (proc_) { return; }

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
    if (pid_) { return; }
    
    int fd[2]; // fd[0] = read end, fd[1] = write end
    ::pipe(fd);

    pid_ = fork();    
        
    if (pid_ == 0) {
        // Child process, run exec to spawn the external command
        close(fd[1]);
        dup2(fd[0], 0);

        std::vector<const char*> arg; 
        for (int i = 0; i < arg_.size(); i++) {
            arg.push_back(arg_[i].c_str());
        }
        arg.push_back(0);
        execvp(command_.c_str(), (char**)arg.front());
        // Should never get here
        exit(1);

    } else if (pid_ > 0) {
        // Parent process, set up the pipe
        close(fd[0]);
        pipe_ = new Stream(fd[1]);
    }
#endif 
}
