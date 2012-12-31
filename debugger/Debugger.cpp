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

#include "Debugger.hpp"

#define VMCHECK(x) {\
    if (KERN_SUCCESS != (x)) {\
        printf("Error ("#x")");\
        abort();\
    }\
}

Debugger::Debugger() : scratch_(0) {
    // Creates a new debugger and starts accepting input from the user. 

    // Allocate the scratch area, which is used to read data from the traced
    // process.  This area must be aligned to a page boundary, so just alloc
    // two pages.  We need two pages to handle reading memory regions that span
    // a page boundary.
    int const flags = VM_FLAGS_ANYWHERE;
    VMCHECK(mach_vm_allocate(mach_task_self(), &scratch_, slen_, flags));
}

Debugger::~Debugger() {
    VMCHECK(mach_vm_deallocate(mach_task_self(), scratch_, slen_));  
}

void Debugger::read(DebugAddr addr, char const* buf, int len) {
    // Reads len bytes from addr in the traced process into 'buf'
}

void Debugger::write(DebugAddr addr, char const* buf, int len) {
    // Reads len bytes from addr  in the traced process into buf
    mach_vm_address_t vmaddr = (mach_vm_address_t)addr;
    mach_vm_size_t count = slen_;
    VMCHECK(mach_vm_read_overwrite(port_, addr, slen_, scratch_, &count));
}

void Debugger::breakpoint() {
    // Adds a new breakpoint at breakpoint location
}

void Debugger::del() {
    // Deletes a breakpoin
}

void Debugger::cont() {
    // Continues process execution, and waits until the process halts
}

void Debugger::run() {
    // Runs an executable with the given arguments.
}

void Debugger::setenv() {
    // Sets an environment variable.
}

void Debugger::getenv() {
    // Gets an environment variable.
}

