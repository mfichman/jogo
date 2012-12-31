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
#include "Object.hpp"
#include "Stream.hpp"
#include <string>
#include <sstream>

class String : public Object {
public:
    String(const std::string& string) :
        string_(string) {
    }

    const std::string& string() const { return string_; }
    String* next() const { return next_; }
    bool is_empty() const { return string_.empty(); }
    void next(String* next) { next_ = next; }
    typedef Pointer<String> Ptr;

    static char escape(const std::string& code);

private:
    std::string string_;
    String::Ptr next_;
};

inline Stream::Ptr operator<<(Stream::Ptr out, const String* name) {
    out << name->string();
    return out;
}

