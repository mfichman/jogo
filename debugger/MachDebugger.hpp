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

/* Debugging for Mach-O formatted executables with the Mach kernel */

#include "Debugger.hpp"

#ifdef DARWIN

#include <mach/mach_traps.h>
#include <mach/mach_vm.h>
#include <mach/vm_map.h>
#include <mach/mach_init.h>
#include <sys/types.h>
#include <sys/ptrace.h>

class MachDebugger : public Debugger {
public:
    MachDebugger();
    ~MachDebugger();
    DebugAddr symbol(String* name);
    void read(DebugAddr addr, char const* buf, int len); 
    void write(DebugAddr addr, char const* buf, int len);
    void breakpoint(DebugAddr addr);
    void exec(char** argv, int argc);
    void kill();
    void cont();
    void attach(ProcessId);

private:
    vm_map_t port_; // Mach kernel port for the traced process
    pid_t pid_; // Process identifier for the traced process
    mach_vm_address_t scratch_; // Scratch memory region used for read/write
    static mach_vm_size_t const slen_ = 2 * PAGE_SIZE;
};

#endif
