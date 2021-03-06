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

#ifndef JOGO_SOCKET_ADDR_H
#define JOGO_SOCKET_ADDR_H

#include "Primitives.h"

typedef struct Socket_Addr* Socket_Addr;
struct Socket_Addr {
    String host;
    Int ip;
    Int port;
    Int error; 
};

Socket_Addr Socket_Addr__init(Socket_Addr ret, String str, Int port);
String Socket_Addr_host__g(Socket_Addr self);
Bool Socket_Addr__equals(Socket_Addr self, Socket_Addr other);
void Socket_Addr__copy(Socket_Addr self, Socket_Addr other);
extern void Socket_Addr__vtable();

#endif
