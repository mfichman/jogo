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

char String::escape(const std::string& str) {
    // Returns the escaped character for 'in', e.g., if '\n' is given to this
    // function, it returns the newline character.

    const char* c = &str[0];
    if (*c != '\\') {
        return *c;
    }
    c++;

    if (isdigit(c[0]) && isdigit(c[1]) && isdigit(c[2])) {
        return 64*(c[0]-'0') + 8*(c[1]-'0') + (c[2]-'0');
    }
    if (*c == 'x' && isxdigit(c[1]) && isxdigit(c[2])) {
        char ret = 0;
        if (isupper(c[1])) {
            ret += 10 + c[1] - 'A';
        } else if (islower(c[1])) {
            ret += 10 + c[1] - 'a';
        } else {
            ret += c[1] - '0';
        }
        ret = ret << 4;
        if (isupper(c[2])) {
            ret += 10 + c[2] - 'A';
        } else if (islower(c[2])) {
            ret += 10 + c[2] - 'a';
        } else {
            ret += c[2] - '0';
        }
        return ret;
    }
    switch (*c) {
    case 'a': return 0x7; // alarm
    case 'b': return 0x8; // backspace
    case 't': return 0x9; // horizontal tab
    case 'n': return 0xa; // newline
    case 'v': return 0xb; // vertical tab
    case 'f': return 0xc; // form feed
    case 'r': return 0xd; // carriage return
    case '"': return 0x22; // quote
    case '\'': return 0x27; // quote
    default: return c[0];
    }
}

std::string const String::unescaped() const {
    // Removes the escape sequences from this string and returns the actual
    // string, as intended by the programmer
    std::string const& in = string();
    std::string out;
    int length = 0;
    for (int i = 0; i < in.length(); i++) {
        char c = in[i];
        if (c == '\\') {
            // Output escape sequences.  For NASM, the actual hex codes must
            // be output for non-visible characters; there is no escape
            // character.
            c = in[++i];
            if (isdigit(c)) { // Octal code
                char c2 = in[++i];
                char c3 = in[++i]; 
                out += (c-'0')*64 + (c2-'0')*8 + (c3-'0');
            } else if (c == 'x') { // Hexadecimal code
                char c1 = in[++i];
                char c2 = in[++i];
                char byte = 0;
                if (isupper(c1)) {
                    byte += c1-'A'+10;
                } else if (islower(c1)) {
                    byte += c1-'a'+10;
                } else {
                    byte += c1-'0';
                }
                byte *= 16;
                if (isupper(c2)) {
                    byte += c2-'A'+10;
                } else if (islower(c2)) {
                    byte += c2-'a'+10;
                } else {
                    byte += c2-'0';
                }
                out += byte;
            } else {
                switch (c) {
                case 'a': out += "\a"; break; // alarm
                case 'b': out += "\b"; break; // backspace
                case 't': out += "\t"; break; // horizontal tab
                case 'n': out += "\n"; break; // newline
                case 'v': out += "\v"; break; // vertical tab
                case 'f': out += "\f"; break; // form feed
                case 'r': out += "\r"; break; // carriage return
                case '"': out += "\""; break; // quote
                case '\'': out += "\'"; break; // quote
                default: out += c; break;
                }
            }
        } else {
            out += c;
        }
    }
    return out;
}
