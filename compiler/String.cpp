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

#include "String.hpp"

char octal(std::string::const_iterator& i, std::string::const_iterator const& end) {
    // Parses an octal escape sequence in the form \oXXX where X is an octal
    // digit, [0-7] inclusive.
    char out = 0;
    if (i == end || *i != '\\') { goto error; }
    ++i;
    if (i == end || *i != 'o') { goto error; }
    ++i;
    for (int j = 0; j < 3; ++j) {
        if (i == end || *i < '0' || *i > '7') { goto error; }
        out = (out*8) + (*i) - '0';
        ++i;
    }
    return out; 
error:
    throw StringEscapeError("Invalid octal sequence in literal");
}

char hex(std::string::const_iterator& i, std::string::const_iterator const& end) {
    // Parses a hex escape sequence in the form \xXX where X is a hexadecimal
    // digit, [0-9a-fA-F].
    char out = 0;
    if (i == end || *i != '\\') { goto error; }
    ++i;
    if (i == end || *i != 'x') { goto error; }
    ++i; 
    for (int j = 0; j < 2; ++j) {
        char byte = 0;
        if (i == end) { goto error; }
        if (*i >= 'A' && *i <= 'F') {
            byte = (*i) - 'A' + 10;
        } else if (*i >= 'a' && *i <= 'f') {
            byte = (*i) - 'a' + 10;
        } else if (isdigit(*i)) {
            byte = (*i) - '0';
        } else {
            goto error;
        }
        out = (out*16) + (byte);
        ++i;
    }
    return out;
error:
    throw StringEscapeError("Invalid hexadecimal sequence in literal");
}

char escape(std::string::const_iterator& i, std::string::const_iterator const& end) {
    // Parses an escape sequence
    if (i == end || *i != '\\') { goto error; }
    ++i;
    if (i == end) { goto error; }
    switch (*i) {
    case 'o': --i; return octal(i, end); 
    case 'x': --i; return hex(i, end); 
    case 'a': ++i; return '\a'; // alarm
    case 'b': ++i; return '\b'; // backspace
    case 't': ++i; return '\t'; // horizontal tab
    case 'n': ++i; return '\n'; // newline
    case 'v': ++i; return '\v'; // vertical tab
    case 'f': ++i; return '\f'; // form feed
    case 'r': ++i; return '\r'; // carriage return
    case '"': ++i; return '\"'; // quote
    case '\'': ++i; return '\''; // quote
    default: return *i++; // passthrough
    }
error:
    throw StringEscapeError("Invalid escape sequence in literal");
}

std::string const String::unescaped() const {
    // Removes the escape sequences from this string and returns the actual
    // string, as intended by the programmer
    std::string const& in = string();
    std::string out;
    for (std::string::const_iterator i = in.begin(); i != in.end();) {
        if (*i == '\\') {
            // Output escape sequences.  For NASM, the actual hex codes must
            // be output for non-visible characters; there is no escape
            // character.
            out += escape(i, in.end());
        } else {
            out += *i++;
        }
    }
    return out;
}
