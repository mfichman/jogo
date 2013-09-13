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

#include "WinDebugger.hpp"

#ifdef WINDOWS

WinDebugger::WinDebugger() : 
    process_(INVALID_HANDLE_VALUE),
    thread_(INVALID_HANDLE_VALUE) {
}

WinDebugger::~WinDebugger() {
}

DebugAddr WinDebugger::symbol(String* name) {
    return 0;
}

void WinDebugger::read(DebugAddr addr, char* buf, int len) {
    BOOL ret = ReadProcessMemory(process_, (LPCVOID)addr, buf, len, 0);
    if (!ret) {
        std::cerr << "error: read: " << GetLastError() << std::endl;
        abort();
    }
}
 
void WinDebugger::write(DebugAddr addr, char const* buf, int len) {
    BOOL ret = WriteProcessMemory(process_, (LPVOID)addr, buf, len, 0);
    if (!ret) {
        std::cerr << "error: read: " << GetLastError() << "\n" << std::endl;
        abort();
    }
}

void WinDebugger::breakpoint(DebugAddr addr) {
}

void WinDebugger::exec(const char** argv, int argc) {
    if (process_ != INVALID_HANDLE_VALUE) {
        kill();
    }
    std::string cmd;
    for (int i = 0; i < argc; ++i) {
        cmd += argv[i];
        if (i < (argc-1)) {
            cmd += ' ';
        }
    }
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));
    DWORD flags = DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS;
    BOOL ret = CreateProcess(0, (LPSTR)cmd.c_str(), 0, 0, FALSE, flags, 0, 0, &sinfo, &pinfo);
    if (!ret) {
        std::cerr << "error: exec: " << GetLastError() << "\n" << std::endl;
        abort();
    }
    process_ = pinfo.hProcess;
    thread_ = pinfo.hThread;
}

void WinDebugger::kill() {
    // Kill the process
    BOOL ret = TerminateProcess(process_, 1);
    if (!ret) {
        std::cerr << "error: kill: " << GetLastError() << "\n" << std::endl;
        abort();
    }
    CloseHandle(process_);
    process_ = INVALID_HANDLE_VALUE;
}

void WinDebugger::cont() {
    DWORD status = DBG_EXCEPTION_NOT_HANDLED;
    BOOL ret = ContinueDebugEvent(event_.dwProcessId, event_.dwThreadId, status);
    if (!ret) {
        std::cerr << "error: cont: " << GetLastError() << std::endl;
        abort();
    }
}

void WinDebugger::attach(ProcessId pid) {
    if (process_ != INVALID_HANDLE_VALUE) {
        kill();
    }
    process_ = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    BOOL ret = DebugActiveProcess(pid);
    if (!ret) {
        std::cerr << "error: attach: " << GetLastError() << std::endl;
        abort();
    }
}

WinDebugger::Event WinDebugger::wait() {
    // Waits for a debugger event (exception, breakpoint, etc)
    while (true) {
        memset(&event_, 0, sizeof(event_));
        BOOL ret = WaitForDebugEvent(&event_, INFINITE);
        if (!ret) {
            std::cerr << "error: wait: " << GetLastError() << std::endl;
            abort();
        }
        switch (event_.dwDebugEventCode) {
        case EXCEPTION_DEBUG_EVENT:
            switch (event_.u.Exception.ExceptionRecord.ExceptionCode) {
            case EXCEPTION_ACCESS_VIOLATION: return ACCESS_VIOLATION;
            case EXCEPTION_BREAKPOINT: return BREAKPOINT;
            case EXCEPTION_SINGLE_STEP: return STEP;
            case DBG_CONTROL_C: return INTERRUPT;
            default: break;
            }
        case CREATE_THREAD_DEBUG_EVENT: break;
        case CREATE_PROCESS_DEBUG_EVENT: break;
        case EXIT_THREAD_DEBUG_EVENT: break;
        case EXIT_PROCESS_DEBUG_EVENT: return EXIT;
        case LOAD_DLL_DEBUG_EVENT: 
            CloseHandle(event_.u.LoadDll.hFile);
            break;
        case UNLOAD_DLL_DEBUG_EVENT: break;
        case OUTPUT_DEBUG_STRING_EVENT: break;
        case RIP_EVENT: break;
        }
        cont();
    }
    return NONE;
}

#endif
