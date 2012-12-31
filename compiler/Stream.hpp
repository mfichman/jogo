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

#pragma once
#include "Jogo.hpp"
#include <cstring>
#include <vector>
#ifdef WINDOWS
#include <windows.h>
#endif

// Adapter for low-level I/O.  This class is used instead of C++ iostreams
// because it can support pipe output.
class Stream : public Object {
public:
    Stream(const std::string& file);
#ifdef WINDOWS
    Stream(HANDLE fd);
#else
    Stream(int fd);
#endif
    ~Stream();

    typedef Pointer<Stream> Ptr;    

    static Stream::Ptr stout();
    static Stream::Ptr sterr();

    const std::string& message() const { return message_; }
    bool error() const { return error_; }
    Machine* machine() const { return machine_; }
    void write(const char* data, int len);
    void machine(Machine* machine) { machine_ = machine; }
    void close();
    void flush();

private:
    void write_direct(const char* data, int len);

    std::vector<char> buffer_;
    std::string message_;
    Machine* machine_;
#ifdef WINDOWS
    HANDLE fd_;
#else
    int fd_;
#endif
    bool error_;
    
};

inline Stream::Ptr operator<<(Stream::Ptr s, const char* str) {
    s->write(str, strlen(str));
    return s;
}

inline Stream::Ptr operator<<(Stream::Ptr s, const std::string& str) {
    s->write(str.c_str(), str.length());
    return s;
}

inline Stream::Ptr operator<<(Stream::Ptr s, const void* ptr) {
    std::stringstream ss;
    ss << ptr;
    s << ss.str();
    return s;
}

inline Stream::Ptr operator<<(Stream::Ptr s, char val) {
    s->write(&val, 1);
    return s;
}

inline Stream::Ptr operator<<(Stream::Ptr s, int val) {
    std::stringstream ss;
    ss << val;
    s << ss.str();
    return s;
}
