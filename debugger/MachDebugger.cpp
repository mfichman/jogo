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

#include "MachDebugger.hpp"

#ifdef DARWIN

#define VMCHECK(x) {\
    if (KERN_SUCCESS != (x)) {\
        printf("Error ("#x")");\
        abort();\
    }\
}

MachDebugger::MachDebugger() : scratch_(0) {
    // Creates a new debugger and starts accepting input from the user. 

    // Allocate the scratch area, which is used to read data from the traced
    // process.  This area must be aligned to a page boundary, so just alloc
    // two pages.  We need two pages to handle reading memory regions that span
    // a page boundary.
    int const flags = VM_FLAGS_ANYWHERE;
    VMCHECK(mach_vm_allocate(mach_task_self(), &scratch_, slen_, flags));
}

MachDebugger::~MachDebugger() {
    VMCHECK(mach_vm_deallocate(mach_task_self(), scratch_, slen_))
}


DebugAddr MachDebugger::symbol(String* name) {
    return 0;
}

void MachDebugger::read(DebugAddr addr, char const* buf, int len) {
    // Reads len bytes from addr  in the traced process into buf

}
 
void MachDebugger::write(DebugAddr addr, char const* buf, int len) {
    // Writes len bytes from addr  in the traced process into buf
}

void MachDebugger::breakpoint(DebugAddr addr) {
}

void MachDebugger::exec(char** argv, int argc) {
}

void MachDebugger::kill() {
}

void MachDebugger::cont() {
}

void MachDebugger::attach(ProcessId) {
}

#endif
