/*
 * Copyright (c) 2013 Matt Fichman
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

#include "Debugger.hpp"

#ifdef WINDOWS
#include <windows.h>

class WinDebugger : public Debugger {
public:
    WinDebugger();
    ~WinDebugger();
    DebugAddr symbol(String* name);
    void read(DebugAddr addr, char* buf, int len); 
    void write(DebugAddr addr, char const* buf, int len);
    void breakpoint(DebugAddr addr);
    void exec(const char** argv, int argc);
    void kill();
    void cont();
    void attach(ProcessId pid);
    Event wait();

private:
    HANDLE process_;
    HANDLE thread_;
    DEBUG_EVENT event_; 
};

#endif
