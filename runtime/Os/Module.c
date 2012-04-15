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

#include "Primitives.h"
#include "String.h"
#include "Boot/Module.h"
#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#endif

String Os_strerror(Int error) {
    // Returns a message describing the OS-level error.
    String ret = String_alloc(1024);
#ifdef WINDOWS
    DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM;
    LPTSTR buffer = ret->data;
    ret->length = FormatMessage(flags, 0, error, 0, buffer, 1024, 0);
    if (ret->length) {
        return ret;
    }
#else
    if (!strerror_r(error, ret->data, 1024)) {
        return ret;
    }
    const char* msg = gai_strerror(error);
    if (msg) {
        strncpy(ret->data, msg, 1024);
        ret->length = strlen(ret->data);
        return ret;
   }
#endif
    Boot_abort();
    return 0;
}
