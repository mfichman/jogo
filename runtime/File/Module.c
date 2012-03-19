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

#include "File/Module.h"
#include "String.h"
#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <assert.h>

Io_Stream File_open(String path, String mode) {
#ifdef WINDOWS
    DWORD share = FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE;
    DWORD flags = FILE_FLAG_OVERLAPPED;
    DWORD access = 0;
    DWORD create = 0;
    HANDLE ret = 0;
    if (mode->length < 1) {
        return 0;
    } else if (mode->data[0] == 'r') {
        access = GENERIC_READ;
        create = OPEN_EXISTING;
    } else if (mode->data[0] == 'w') {
        access = GENERIC_WRITE;
        create = CREATE_ALWAYS;
    } else if (mode->data[0] == 'a') {
        access = GENERIC_WRITE;
        create = OPEN_ALWAYS;
    } else if (mode->length < 2) {
        return 0;
    } else if (mode->data[0] == 'r' && mode->data[1] == '+') {
        access = GENERIC_READ|GENERIC_WRITE;
        create = OPEN_EXISTING;
    } else if (mode->data[0] == 'w' && mode->data[1] == '+') {
        access = GENERIC_READ|GENERIC_WRITE;
        create = CREATE_ALWAYS;
    } else if (mode->data[0] == 'a' && mode->data[1] == '+') {
        access = GENERIC_READ|GENERIC_WRITE;
        create = OPEN_ALWAYS;
    } else {
        return 0;
    }
    ret = CreateFile(path->data, access, share, 0, create, flags, 0);
    if (ret == INVALID_HANDLE_VALUE) {
        // ERROR
        return 0;
    } else {
        return Io_Stream__init((Int)ret, Io_StreamType_FILE);
    }
#else
    Int flags = 0;
    if (mode->length < 1) {
        return 0;
    } else if (mode->data[0] == 'r') {
        flags = O_RDONLY;
    } else if (mode->data[0] ==  'w') {
        flags = O_WRONLY|O_CREAT|O_TRUNC;
    } else if (mode->data[0] == 'a') {
        flags = O_WRONLY|O_CREAT|O_APPEND; 
    } else if (mode->length < 2) {
        return 0;
    } else if (mode->data[0] == 'r' && mode->data[1] == '+') {
        flags = O_RDWR;
    } else if (mode->data[0] == 'w' && mode->data[1] == '+') {
        flags = O_RDWR|O_CREAT|O_TRUNC;
    } else if (mode->data[0] == 'a' && mode->data[1] == '+') {
        flags = O_RDWR|O_CREAT|O_APPEND;
    } else {
        return 0;
    }

    Int ret = open(path->data, flags, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (ret == -1) {
        // ERROR
        return 0;
    } else {
        return Io_Stream__init(ret, Io_StreamType_FILE);
    }
#endif
    return 0;
}
