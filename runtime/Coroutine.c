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

#if defined(WINDOWS)
#include <windows.h>
#elif defined(DARWIN)
#define _DARWIN_C_SOURCE
#define MAP_ANONYMOUS MAP_ANON
#include <sys/mman.h>
#include <unistd.h>
#elif defined(LINUX)
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Coroutine.h"
#include "Array.h"
#include "Io/Manager.h"
#include "Boot/Boot.h"
#include "Os/Os.h"
#include "Object.h"
#include "String.h"

struct Coroutine Coroutine__main = {
    Coroutine__vtable,
    Object__READONLY_MASK,
    0,
    0,
    0,
    0,
    0, 
    0,
};
Coroutine Coroutine__current = &Coroutine__main;
Int Exception__current = 0;

void Coroutine__marshal(Coroutine self, Int ra) {
    // Marshals a function call on the coroutine stack
#ifdef WINDOWS
    Int stack_arg_regs = 4;
    Int i = 0;
    for(i = 0; i < stack_arg_regs; i++) {
        --self->sp;
    }
#endif
    self->stack->data[--self->sp] = ra;

    // Stack diagram:
    // low memory 
    // [+5] arg3 (Windows only)
    // [+4] arg2 (Windows only)
    // [+3] arg1 (Windows only)
    // [+2] arg0 (Windows only)
    // [+1] return address
    // high memory
}

Coroutine Coroutine__init(Object func) {
    // Initializes a function with a new stack and instruction pointer. When
    // the coroutine is resumed, it will begin executing at 'function' with its
    // own stack.
    Array arr = coroutines();
    Coroutine ret = Boot_calloc(sizeof(struct Coroutine)); 
    ret->_vtable = Coroutine__vtable;
    ret->_refcount = 1; 
    ret->stack = CoroutineStack__init();
    ret->stack_end = ((U64)ret->stack)+sizeof(ret->stack->data);
    ret->function = func;
    ret->caller = 0;
    ret->sp = COROUTINE_STACK_SIZE;
    ret->index = Array_count__g(arr);
    Coroutine__commit_page(ret, ret->stack_end-1);
    Object__refcount_inc(func);
    if (func) {
        Int bogus = 0; // Bogus return addr for exit's stack frame
        ret->sp -= 1; // MUST ensure that the SP starts out aligned!!
        Coroutine__marshal(ret, bogus); // Simulate a fn call to exit()
        ret->stack->data[--ret->sp] = (Int)Coroutine__exit;
        ret->stack->data[--ret->sp] = (Int)Coroutine__start;
        ret->stack->data[--ret->sp] = (Int)ret; 
        // ARG0 in the .asm file.  This is the 'self' pointer for coroutine.
        ret->sp -= 15; // For the 15 registers pushed in the .asm file
        ret->sp = (Int)(ret->stack->data + ret->sp); 
        // Make relative to the top-of-stack
        ret->status = CoroutineStatus_NEW;
        // At the time Coroutine__swap is called for the first time, the call
        // stack looks like this:
        //
        // align arg3 arg2 arg1 arg0 bogus exit call tos ARG0 regs...+15
        //
        // Coroutine__swap pops regs, ARG0, and tos for its own use.  When
        // Coroutine__swap's ret instruction is executed, the coroutine will
        // jump to the function 'call.' Upon entering 'call', the stack looks
        // like this:
        //
        // align arg3 arg2 arg1 arg0 bogus exit 
        // 
        //
        // From here, 'call' can build its stack frame.  When 'call' exits, it
        // pops the return address for 'exit' off the stack, and 'exit' begins
        // executing.  Upon entering exit, the stack looks like this:
        //
        // align arg3 arg2 arg1 arg0 bogus 
        //
        // Coroutine__exit never returns, so 'bogus' is never jumped to.
        // Instead, the coroutine swaps to another coroutine before it falls
        // off the end of 'exit'.  arg3-0 are needed on Windows as a backing
        // store for the arg regs (according to the Win64 calling convention),
        // even though Coroutine__exit has no parameters.  WARNING: The stack
        // frame for Coroutine__exit will be misaligned by 8 bytes...this could
        // cause bugs if that function ever calls anything else.
    } else {
        ret->status = CoroutineStatus_DEAD; 
    }
    Array_push(arr, (Object)ret); 
    arr->_refcount--;
    ret->_refcount--; // Make sure the reference in the Array is weak.

    return ret;
}

void Coroutine__destroy(Coroutine self) {
    // Set the exception flag, then resume the coroutine and unwind the stack,
    // calling destructors for objects referenced by the coroutine.
    Array arr = coroutines();
    Int save_except = Exception__current;
    Coroutine last = 0;
    Exception__current = 1;

    if (self->status == CoroutineStatus_SUSPENDED) {
        // Temporarily re-increment the refcount so that there's no double 
        // free; otherwise, the reference counting in resume() would cause
        // the coroutine to be destroyed twice.
        self->_refcount++;
        Coroutine__call(self);
        self->_refcount--;
    } else if (self->status == CoroutineStatus_RUNNING) {
        Os_cpanic("Coroutine freed while running");
    } else if (self->status == CoroutineStatus_IO) {
        Os_cpanic("Coroutine freed while waiting for I/O");
    } else if (self->status == CoroutineStatus_DEAD) {
        // Stack references are already freed.
    }

    self->_refcount++;
    last = (Coroutine)Array_last__g(arr); // +1
    last = (Coroutine)Array_pop(arr); // +1
    assert(last);
    if (last != self) {
        self->_refcount++;
        Array__insert(arr, self->index, (Object)last); // +1
        last->_refcount--;
        last->index = self->index;
    }
    last->_refcount--;
    self->_refcount--;
    assert(self->_refcount == 0);

    // Temporarily re-increment the refcount to remove the coroutine from the
    // coroutine list
    
    // Free the stack, and the pointer to the closure object so that no memory
    // is leaked.
#ifdef WINDOWS
    VirtualFree(self->stack, sizeof(self->stack), MEM_RELEASE);
#else
    munmap(self->stack, sizeof(self->stack));
#endif
    Object__refcount_dec(self->function);
    Boot_free(self);

    Exception__current = save_except;
}

void Coroutine_resume(Coroutine self) {
    // Resumes a coroutine, and sets the 'caller' coroutine to the current 
    // coroutine.  Returns immediately if the coroutine is DEAD or nil.
    //printf("resuming coroutine %p\n", self);
    if (!self) { return; }
    if (self->status == CoroutineStatus_DEAD) { return; }
    if (self->status == CoroutineStatus_RUNNING) { return; }
    // If the user tries to resume a coroutine that is in the 'IO' state, then
    // it will just immediately block again.  So, to avoid that tricky case,
    // just return immediately from resume().
    if (self->status == CoroutineStatus_IO) { return; }

    // Note: The coroutine's refcount gets incremented by one while the 
    // coroutine is running, so that the coroutine won't get freed while its
    // stack is active.
    self->status = CoroutineStatus_RUNNING;
    Object__refcount_inc((Object)self);
    self->caller = Coroutine__current;
    //printf("swapping to %x\n", self);
    Coroutine__swap(Coroutine__current, self);
    self->caller = 0; 
    Object__refcount_dec((Object)self);
}

void Coroutine_dump(Coroutine self) {
    // Dump information about the coroutine's current status
    char const* yield_loc = "nil"; 
    char const* status = 0;
    if (self->yield_loc) {
        yield_loc = (char*)self->yield_loc->data; 
    }
    switch (self->status) {
    case 0: status = "NEW"; break;
    case 1: status = "RUNNING"; break;
    case 2: status = "SUSPENDED"; break;
    case 3: status = "DEAD"; break;
    case 4: status = "IO"; break;
    default: assert(!"Unknown coroutine state");
    }
    fprintf(stderr, "%p CoroutineStatus::%s %s\n", self, status, yield_loc);
    fflush(stderr);
}

void Coroutine__start(Coroutine self) {
    // Starts a coroutine
    static struct String call_str = String__static("@call");
    typedef void (*CallFunc)(Object);
    CallFunc call = Object__dispatch(self->function, &call_str);
    call(self->function);
}

void Coroutine__exit() {
    // Yields the the current coroutine to the coroutine's caller.
    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        //printf("swapping to %x\n", Coroutine__current->caller);
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_DEAD;
        //printf("swapping to main\n");
        Coroutine__swap(Coroutine__current, &Coroutine__main);
        // If there is no caller, then yield to the main coroutine.
    } else {
        Os_cpanic("No coroutine to yield to");
    }
}

void Coroutine__yield_with_msg(String msg) {
    // Yields the current coroutine, but sets the yield_loc message first.
    if(Coroutine__current != &Coroutine__main) {
        Coroutine_yield_loc__s(Coroutine__current, msg);
    }
    Coroutine__yield();
    if(Coroutine__current != &Coroutine__main) {
        Coroutine_yield_loc__s(Coroutine__current, 0);
    }
}

void Coroutine__yield() {
    // Yields the the current coroutine to the coroutine's caller.  This is a
    // no-op if the coroutine is the main coroutine.
    //printf("yielding coroutine %p\n", Coroutine__current);
    if (Coroutine__current == &Coroutine__main) {
        Os_cpanic("Coroutine::yield() called by main coroutine");
    } else if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_SUSPENDED;
        //printf("swapping to %x\n", Coroutine__current->caller);
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_SUSPENDED;
        //printf("swapping to main\n");
        Coroutine__swap(Coroutine__current, &Coroutine__main);
    } else {
        Os_cpanic("No coroutine to yield to");
    }
}

void Coroutine__iowait() {
    // Causes this coroutine to wait until I/O is available.  Note: calling
    // this function if no I/O is pending will cause the Coroutine to block
    // indefinitely. 
    //printf("waiting on I/O for %p\n", Coroutine__current);
    Object__refcount_inc((Object)Coroutine__current);
    Io_manager()->waiting++;

    if (Coroutine__current && Coroutine__current->caller) {
        Coroutine__current->status = CoroutineStatus_IO;
        //printf("swapping to %x\n", Coroutine__current->caller);
        Coroutine__swap(Coroutine__current, Coroutine__current->caller);
    } else if (Coroutine__current) {
        Coroutine__current->status = CoroutineStatus_IO;
        //printf("swapping to main\n");
        Coroutine__swap(Coroutine__current, &Coroutine__main);
    } else {
        Os_cpanic("Coroutine::iowait() called by main coroutine");
    }
    // Same as the impl of yield, except it sets the status to 'IO' rather than
    // 'SUSPENDED'

    Object__refcount_dec((Object)Coroutine__current);
    Io_manager()->waiting--;
}

void Coroutine__ioresume(Coroutine self) {
    // Resume the coroutine, but preserve the caller.  This function is called
    // by the I/O manager when a pending I/O request completes.
    //printf("I/O complete for %p\n", self);
    if (!self) { return; }
    if (self->status == CoroutineStatus_DEAD) { return; }
    if (self->status == CoroutineStatus_RUNNING) { return; }
    if (self->status != CoroutineStatus_IO) {
        Os_cpanic("Illegal coroutine state");
    }

    self->status = CoroutineStatus_RUNNING;
    Object__refcount_inc((Object)self);
    //printf("swapping to %x\n", self);
    Coroutine__swap(Coroutine__current, self);
    Object__refcount_dec((Object)self);
}

Coroutine coroutine() {
    Object__refcount_inc((Object)Coroutine__current);
    return Coroutine__current;
}

Array coroutines() {
    static Array arr = 0;
    if (!arr) {
        arr = Array__init(0);
    }
    arr->_refcount++;
    return arr;
}

U64 page_round(U64 addr, U64 multiple) {
    // Rounds 'base' to the nearest 'multiple'
    return (addr/multiple)*multiple;
}

U64 page_size() {
    // Returns the system page size.
#ifdef WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize*8; 
#else
    return sysconf(_SC_PAGESIZE);
#endif
}

CoroutineStack CoroutineStack__init() {
    // Lazily initializes a large stack for the coroutine.  Initially, the
    // coroutine stack is quite small, to lower memory usage.  As the stack
    // grows, the Coroutine__fault handler will commit memory pages for
    // the coroutine.
    U64 size = sizeof(struct CoroutineStack);
#ifdef WINDOWS
    CoroutineStack ret = VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
    if (!ret) {
        Boot_abort();
    }
#else
    CoroutineStack ret = mmap(0, size, PROT_NONE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if (ret == MAP_FAILED) {
        Boot_abort();
    }
#endif
    return ret;
}

void Coroutine__commit_page(Coroutine self, U64 addr) {
    // Ensures that 'addr' is allocated, and that the next page in the stack is
    // a guard page on Windows. On Windows, the guard page triggers a one-time
    // STATUS_GUARD_PAGE exception, which triggers the vectored exception
    // handler.  Since the exception handler uses the same stack as the code
    // that triggered the exception, it then uses the guard page as a stack
    // area, to prevent a double stack fault upon entering the vectored
    // exception handler.

    U64 psize = page_size();
    U64 page = page_round(addr, psize);
    U64 len = self->stack_end-page;
    // Commit all pages between stack_min and the current page.
#ifdef WINDOWS
    U64 glen = psize;
    U64 guard = page-glen;
    assert(page < self->stack_end);
    if (!VirtualAlloc((LPVOID)page, len, MEM_COMMIT, PAGE_READWRITE)) {
        abort();     
    }
    if (!VirtualAlloc((LPVOID)guard, glen, MEM_COMMIT, PAGE_READWRITE|PAGE_GUARD)) {
        abort();
    }
#else
    assert(page < self->stack_end);
    if (mprotect((void*)page, len, PROT_READ|PROT_WRITE) == -1) {
        abort();
    }
#endif
}


#ifdef WINDOWS
void pexcept(DWORD code) {
    // Print out the error, b/c Windows doesn't normally print an error during
    // an exception/signal like Linux and OS X do.
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION: fprintf(stderr, "Access violation\n"); break;
    case EXCEPTION_DATATYPE_MISALIGNMENT: fprintf(stderr, "Datatype misalignment\n"); break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO: fprintf(stderr, "Divide by zero\n"); break;
    case EXCEPTION_FLT_DENORMAL_OPERAND: fprintf(stderr, "Floating point exception\n"); break;
    case EXCEPTION_FLT_INEXACT_RESULT: fprintf(stderr, "Floating point exception\n"); break;
    case EXCEPTION_FLT_OVERFLOW: fprintf(stderr, "Floating point exception\n"); break;
    case EXCEPTION_FLT_STACK_CHECK: fprintf(stderr, "Floating point exception\n"); break;
    case EXCEPTION_FLT_UNDERFLOW: fprintf(stderr, "Floating point exception\n"); break;
    case EXCEPTION_ILLEGAL_INSTRUCTION: fprintf(stderr, "Illegal instruction\n"); break;
    case EXCEPTION_IN_PAGE_ERROR: fprintf(stderr, "Page error\n"); break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO: fprintf(stderr, "Divide by zero\n"); break;
    case EXCEPTION_INT_OVERFLOW: fprintf(stderr, "Integer overflow\n"); break;
    case EXCEPTION_INVALID_DISPOSITION: fprintf(stderr, "Internal error\n"); break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: fprintf(stderr, "Internal error\n"); break;
    case EXCEPTION_PRIV_INSTRUCTION: fprintf(stderr, "Illegal instruction\n"); break;
    case EXCEPTION_SINGLE_STEP: break;
    case EXCEPTION_STACK_OVERFLOW: fprintf(stderr, "Stack overflow\n"); break;
    }
	fflush(stderr);
}
#endif

#ifdef WINDOWS
void Coroutine__set_signals() {
    AddVectoredExceptionHandler(1, Coroutine__fault);
}
#endif

#ifdef WINDOWS
LONG WINAPI Coroutine__fault(LPEXCEPTION_POINTERS info) {
    // From MSDN: The first element of the array contains a read-write flag
    // that indicates the type of operation that caused the access violation.
    // If this value is zero, the thread attempted to read the inaccessible
    // data. If this value is 1, the thread attempted to write to an
    // inaccessible address. If this value is 8, the thread causes a user-mode
    // data execution prevention (DEP) violation.  The second array element
    // specifies the virtual address of the inaccessible data.
    U64 stack_start = (U64)Coroutine__current->stack;
    U64 stack_end = Coroutine__current->stack_end;
    U64 type = (U64)info->ExceptionRecord->ExceptionInformation[0];
    U64 addr = (U64)info->ExceptionRecord->ExceptionInformation[1];
	DWORD code = info->ExceptionRecord->ExceptionCode;
    U64 const access_read = 0;
    U64 const access_write = 1;

    if (type != access_read && type != access_write) {
        pexcept(code);
        return EXCEPTION_CONTINUE_SEARCH;
    } else if (Coroutine__current == &Coroutine__main) {
        pexcept(code);
        return EXCEPTION_CONTINUE_SEARCH;
    } else if (addr < stack_start || addr >= stack_end) {
        pexcept(code);
        return EXCEPTION_CONTINUE_SEARCH;
    } else if (code == EXCEPTION_ACCESS_VIOLATION) {
        Coroutine__commit_page(Coroutine__current, addr);
        return EXCEPTION_CONTINUE_EXECUTION;
    } else if (code == STATUS_GUARD_PAGE_VIOLATION) {
        Coroutine__commit_page(Coroutine__current, addr);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    pexcept(code);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

#ifndef WINDOWS
static struct sigaction sigsegv;
static struct sigaction sigbus;
static char signalstack[SIGSTKSZ];

void Coroutine__set_signals() {
    // Set up the signal handlers for SIGBUS/SIGSEGV to catch stack protection
    // errors and grow the stack when it runs out of space.
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigfillset(&sa.sa_mask);
    sa.sa_sigaction = Coroutine__fault;
    sa.sa_flags = SA_SIGINFO|SA_ONSTACK;
    sigaction(SIGSEGV, &sa, &sigsegv);
    sigaction(SIGBUS, &sa, &sigbus); 
  
    stack_t stack;
    stack.ss_sp = signalstack;
    stack.ss_flags = 0;
    stack.ss_size = sizeof(signalstack);
    sigaltstack(&stack, 0);
}
#endif

#ifndef WINDOWS
void Coroutine__fault(int signo, siginfo_t* info, void* context) {
    if (signo != SIGBUS && signo != SIGSEGV) {
        abort();
    }
    U64 stack_start = (U64)Coroutine__current->stack;
    U64 stack_end = Coroutine__current->stack_end;
    U64 addr = (U64)info->si_addr;
    if (addr < stack_start || addr >= stack_end) {
        if (signo == SIGBUS) {
            struct sigaction self;
            sigaction(SIGBUS, &sigbus, &self);
            raise(SIGBUS);
        } else if (signo == SIGSEGV) {
            struct sigaction self;
            sigaction(SIGSEGV, &sigsegv, &self);
            raise(SIGSEGV);
        }
    } else {
        Coroutine__commit_page(Coroutine__current, addr);
    }
}
#endif

