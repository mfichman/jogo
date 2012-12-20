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
#include <mach/mach_traps.h>
#include <mach/mach_vm.h>
#include <mach/vm_map.h>
#include <mach/mach_init.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <cerrno>
#include <iostream>

Environment::Ptr env(new Environment());
pid_t pid = 0;
task_t task = 0;

#define vm_map_trunc_page(x) ((vm_map_offset_t)(x) & ~((signed)PAGE_MASK))

#define CHECK(x) {\
    errno = 0;\
    x;\
    if (errno) {\
        perror("Error ("#x")");\
    }\
}

void error(kern_return_t ret) {
    if (KERN_SUCCESS == ret) {
        printf("KERN_SUCCESS\n");
    } else if (KERN_INVALID_ADDRESS == ret) {
        printf("KERN_INVALID_ADDRESS\n");
    } else if (KERN_PROTECTION_FAILURE == ret) {
        printf("KERN_PROTECTION_FAILURE\n");
    } else {
        printf("unknown\n");
    }
}   

void read_mem(mach_vm_address_t addr, mach_vm_address_t buf, int len) {
    // Read the traced process's memory, starting at addr, up to addr+len.
    mach_vm_size_t count = len;
    kern_return_t ret = 0;

    ret = mach_vm_protect(task, addr, 1, false, VM_PROT_READ|VM_PROT_WRITE);
    error(ret);
    ret = mach_vm_read_overwrite(task, addr, len, buf, &count);
    error(ret);
/*
    size_t read = 0;
    
    while (read < len) {
        // Read and copy a single word of data at a time from the traced
        // process's address space.
        errno = 0;
        int data = ptrace(PT_READ_D, pid, addr+read, 0);
        if (errno) {
            perror("Error: PT_READ_D");
        } 
        memcpy(buf+read, &data, std::min(sizeof(data), len-read));
        read += sizeof(data);
    }
*/
}

void write_mem(mach_vm_address_t addr, mach_vm_address_t buf, int len) {
    // Write the traced process's memory, starting at addr, up to addr+len.
    kern_return_t ret = 0;
    ret = mach_vm_write(task, addr, buf, len);
    error(ret);
    ret = mach_vm_copy(task, addr, 10, addr);
    error(ret);
    
/*
    size_t written = 0;

    while (written < len) {
        // Write a single word of data at at time to the traced process's
        // address space.
        errno = 0;
        int data = 0;
        memcpy(&data, buf+written, std::min(sizeof(data), len-written));
        int ret = ptrace(PT_WRITE_D, pid, addr+written, data);
        if (errno) {
            perror("Error: PT_WRITE_D");
        }
        written += sizeof(data);
    }
*/
}

void breakpoint(mach_vm_address_t addr) {
    // Set the breakpoint to the given address
    //addr = (mach_vm_address_t)0x0000000100000ea0;
    //addr = vm_map_trunc_page(0x0000000100000ea0UL);// / 8;
    //addr = vm_map_trunc_page(0x00000001);// / 8;
    addr = vm_map_trunc_page(0x7fff5e8a6c08);// / 8;
 //(mach_vm_address_t)0; //0x00000ea0;

    //00 00 00 01 00 00 0e a0
    //a0 0e 00 00 01 00 00 00
    std::cerr << "Breaking at " << std::hex << (intptr_t)addr << std::dec << std::endl;
    //char* trap = new char[PAGE_SIZE];
//    trap[0] = char(0xcd);
//    trap[1] = char(0x80);
//    trap[2] = char(0xcc);
//    trap[3] = 0x00;
//    char* backup = new char[PAGE_SIZE];

    mach_vm_address_t trap = 0; 
    mach_vm_address_t backup = 0;
    
    error(mach_vm_allocate(mach_task_self(), &trap, PAGE_SIZE, VM_FLAGS_ANYWHERE));
    error(mach_vm_allocate(mach_task_self(), &backup, PAGE_SIZE, VM_FLAGS_ANYWHERE));
        
    read_mem(addr, backup, PAGE_SIZE);
    write_mem(addr, trap, PAGE_SIZE);
    // Set up the breakpoint by replacing it with an INT instruction
}

void run() {
    // Continue execution of the stopped child process.
    std::cerr << "Resuming execution" << std::endl;
    CHECK(ptrace(PT_CONTINUE, pid, (caddr_t)1, 0));
    wait(NULL);
}

void command() {
    // Parses a single comment
    Lexer lexer(env);
    lexer.input();
    Token cmd = lexer.token();
    if (cmd.value() == "break") {
        lexer.next();
        Token bp = lexer.token();
        std::stringstream ss(bp.value().substr(1));
        intptr_t addr = 0;
        ss >> addr;
        breakpoint((mach_vm_address_t)addr);
        std::cerr << bp.value() << std::endl;
    } else if (cmd.value() == "run") {
        run();
    } else if (cmd.value() == "continue") {
        run();
    }
}

pid_t run(char* argv) {
    // Forks the executable for debugging.
    pid_t pid = fork();
    if (pid < 0) {
        assert(!"fork failed");
        exit(1);
    } else if (pid) {
        return pid;
    }

    std::cerr << &pid << std::endl;
    std::cerr << sizeof(&pid) << std::endl;
    CHECK(ptrace(PT_TRACE_ME, 0, 0, 0));
    // Wait for the parent process to attach to the process.
    execlp(argv, argv, 0);
    assert(!"exec failed");
    exit(1);
    return -1;
}

int main(int argc, char** argv) {
    // Apollo debugger
    std::cerr << sizeof(argv) << std::endl;
    pid = run(argv[1]);
    task_for_pid(mach_task_self(), pid, &task);
    std::cerr << "Forked child at " << pid << std::endl;
    std::cerr << "Task " << task << std::endl;

    while (std::cin.good()) {
        std::cout << ">>> ";
        std::string line;
        
        int ch = std::cin.get();
        if (ch == ':') {
            command();
        } else {
            std::getline(std::cin, line);
            std::cerr << "Error: Not supported" << std::endl;
        }
        
    }
    std::cout << "Leaving jgi." << std::endl;

    return 0;
}
