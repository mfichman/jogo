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

#include "Environment.hpp"
#include "Lexer.hpp"
#include <cerrno>
#include <iostream>

typedef uint64_t DebugAddr;
typedef uint64_t ProcessId;


/* Platform-independent interface for debugging Jogo code */
class Debugger : public Object {
public:
    enum Event {
        NONE,
        ACCESS_VIOLATION,
        BREAKPOINT,
        STEP,
        EXIT,
        INTERRUPT, // CTRL-C  
    };

    DebugAddr symbol(String* name);
    // Looks up the address of a symbol by name.

    virtual void read(DebugAddr addr, char* buf, int len)=0; 
    // Reads len bytes from addr into buf.

    virtual void write(DebugAddr addr, char const* buf, int len)=0;
    // Writes len bytes from buf to addr.
    
    virtual void breakpoint(DebugAddr addr)=0;
    // Sets a breakpoint at address addr.

    virtual void exec(const char** argv, int argc)=0;
    // Executes the process using the given arguments, and begins debugging.
    
    virtual void kill()=0;
    // Kills the debugged process.r();

    virtual void cont()=0;
    // Continues exectuing the debugged process

    virtual void attach(ProcessId pid)=0;
    // Attaches to the process with the given id

    virtual Event wait()=0;
    // Waits for a debug event.

    

    typedef Pointer<Debugger> Ptr;
};

