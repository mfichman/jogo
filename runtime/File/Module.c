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

#include "Module.h"
#include "../String.h"
#include <string.h>
#ifndef WINDOWS
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

File_Stream File_open(String path, String mode) {

#ifndef WINDOWS
    Int flags = 0;
    if (!strncmp(mode->data, "r", 1)) {
        flags = O_RDONLY;
    } else if (!strncmp(mode->data, "w", 1)) {
        flags = O_WRONLY|O_CREAT;
    } else if (!strncmp(mode->data, "a", 1)) {
        flags = O_WRONLY|O_APPEND|O_CREAT; 
    } else if (!strncmp(mode->data, "r+", 2)) {
        flags = O_RDWR;
    } else if (!strncmp(mode->data, "w+", 2)) {
        flags = O_RDWR|O_CREAT;
    } else if (!strncmp(mode->data, "a+", 2)) {
        flags = O_RDWR|O_APPEND|O_CREAT;
    }

    Int ret = open(path->data, flags, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (ret == -1) {
        return 0;
    } else {
        return File_Stream__init(ret);
    }
#endif
}
