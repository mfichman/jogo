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

#include "Io/Stream.h"
#include "String.h"
#include <stdio.h>
#ifdef WINDOWS
#include <windows.h>
#endif

Io_Stream Io_stderr() {
    static Io_Stream err = 0;
    if (!err) {
#ifdef WINDOWS
        err = Io_Stream__init((Int)GetStdHandle(STD_ERROR_HANDLE));
#else
        err = Io_Stream__init(2);
#endif
    }
    return err;
}

Io_Stream Io_stdout() {
    static Io_Stream out = 0;
    if (!out) {
#ifdef WINDOWS
        out = Io_Stream__init((Int)GetStdHandle(STD_OUTPUT_HANDLE));
#else
        out = Io_Stream__init(1);
#endif
    }
    return out;
}

Io_Stream Io_stdin() {
    static Io_Stream in = 0;
    if (!in) {
#ifdef WINDOWS
        in = Io_Stream__init((Int)GetStdHandle(STD_INPUT_HANDLE));
#else
        in = Io_Stream__init(0);
#endif
    }
    return in;
}

void Io_println(String str) {
    // FIXME: Use Apollo streams
    if (str) {
        fwrite(str->data, str->length, 1, stdout);
    } else {
        fwrite("nil", 3, 1, stdout);
    }
    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
}

void Io_print(String str) {
    // FIXME: Use Apollo streams
    if (str) {
        fwrite(str->data, str->length, 1, stdout);
    } else {
        fwrite("nil", 3, 1, stdout);
    }
    fflush(stdout);
}
