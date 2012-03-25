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


#include "Socket/Module.h"
#include "Boot/Module.h"
#include "String.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WINDOWS
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif

Socket_Addr Socket_Addr__init(String str) {
    // Initialies a new socket address (IP address, port port ID pair).
    Socket_Addr ret = Boot_calloc(sizeof(struct Socket_Addr));
    Char* c = 0;
    String host_str = 0;
    String port_str = 0;
    struct in_addr in; 
    memset(&in, 0, sizeof(in));

    ret->_vtable = Socket_Addr__vtable;
    ret->_refcount = 1;
    ret->host = 0;
    ret->ip = 0;
    ret->port = 0;
    ret->valid = 0;

    // Split the address string on the ':' character.  The substring before the
    // first ':' is the host name, and the substring after the ':' is the port
    // number.
    for (c = str->data; (*c != ':') && (*c != '\0'); ++c) {}
    if (*c != ':') { 
        ret->valid = 0;
        return ret;
    }
    host_str = String_prefix(str, c - str->data);

    // Attempt to translate the hostname as a dotted quad first.  Then, try to
    // translate the hostname as a DNS name.
    if (inet_pton(AF_INET, host_str->data, &in) != 1) { 
        struct addrinfo* res = 0;
        struct sockaddr_in* sin = 0;
        if (getaddrinfo(host_str->data, 0, 0, &res) != 0) {
            Object__refcount_dec(host_str);
            return ret;
        }
        Object__refcount_dec(host_str);

        for(; res; res = res->ai_next) {
            sin = (struct sockaddr_in*)res->ai_addr;
            if (sin->sin_addr.s_addr) {
                in = sin->sin_addr; 
            }
        }
        freeaddrinfo(res);
    }

    // Parse the port number, and make sure that it is in range.
    port_str = String_suffix(str, c - str->data + 1);
    ret->port = atoi(port_str->data); 
    Object__refcount_dec(port_str);
    if (!ret->port || ret->port > 0xFFFF) {
        return ret;
    }

    ret->ip = ntohl(in.s_addr);
    ret->valid = 1;
    return ret;
}

String Socket_Addr_host__g(Socket_Addr self) {
    return 0;
}

