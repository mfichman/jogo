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

#include <Jogo.hpp>
#include <String.hpp>
#include <Stream.hpp>
#include <cassert>

class Location {
public:
    Location() :
        file(0),
        first_line(0),
        first_column(0),
        first_offset(0),
        last_line(0),
        last_column(0),
        last_offset(0) {
    }
    Location(Location const& copy) {
        file = copy.file; 
        first_line = copy.first_line;
        first_column = copy.first_column;
        first_offset = copy.first_offset;
        last_line = copy.last_line;
        last_column = copy.last_column;
        last_offset = copy.last_offset;
    }
    File* file;
    int first_line;
    int first_column;
    int first_offset;
    int last_line;
    int last_column;
    int last_offset;
};

Stream::Ptr operator<<(Stream::Ptr out, const Location& location);
