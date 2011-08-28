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

#include "String.h"
#include <stdio.h>
#include <stdlib.h>

void boot_print_str(String* string) {
    // Write string to stdout.  This is function is here for convenience's 
    // sake.  Once a full-fledged IO framework has been written, this function
    // will really only be useful for simple output.

    fwrite(string->data, 1, string->length, stdout);
    fflush(stdout);
}

void boot_print_int(Int integer) {
    // Print an integer to stdout.  This function is here only to run initial
    // tests on the compiler, and isn't part of the public API.

#ifdef DARWIN
    fprintf(stdout, "%lld", integer);
#else
    fprintf(stdout, "%ld", integer);    
#endif
    fflush(stdout);
}

void boot_print_char(Char character) {
    // Print a character to stdout.  This function is not part of the public 
    // API for the Apollo library.

    fputc(character, stdout);
    fflush(stdout);
}

void boot_dummy(int a, int b, int c) {
    boot_print_int(a);
    boot_print_int(b);
    boot_print_int(c);
}
