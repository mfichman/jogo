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

#include "Io/Module.h"
#include "Io/Stream.h"
#include "Io/Manager.h"
#include "String.h"
#include <stdio.h>
#ifdef WINDOWS
#include <windows.h>
#endif

Io_Manager Io_manager() {
    static Io_Manager manager = 0;
    if (!manager) {
        manager = Io_Manager__init();
    }
    manager->_refcount++;
    return manager;
}

Io_Stream Io_stderr() {
    static Io_Stream err = 0;
    if (!err) {
#ifdef WINDOWS
        HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
        err = Io_Stream__init((Int)handle, Io_StreamType_CONSOLE);
#else
        err = Io_Stream__init(2, Io_StreamType_CONSOLE);
#endif
    }
    err->_refcount++;
    return err;
}

Io_Stream Io_stdout() {
    static Io_Stream out = 0;
    if (!out) {
#ifdef WINDOWS
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        out = Io_Stream__init((Int)handle, Io_StreamType_CONSOLE);
#else
        out = Io_Stream__init(1, Io_StreamType_CONSOLE);
#endif
    }
    out->_refcount++;
    return out;
}

Io_Stream Io_stdin() {
    static Io_Stream in = 0;
    if (!in) {
#ifdef WINDOWS
        HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
        in = Io_Stream__init((Int)handle, Io_StreamType_CONSOLE);
#else
        in = Io_Stream__init(0, Io_StreamType_CONSOLE);
#endif
    }
    in->_refcount++;
    return in;
}
