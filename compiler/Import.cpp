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

#include "Import.hpp"
#include "File.hpp"

std::string Import::file_name(const std::string& scope) {
    // Converts a module name to the name of the file that contains the 
    // module.
    std::string out;
    for (size_t i = 1; i <= scope.length(); i++) {    
        if (scope[i-1] == ':' && scope[i] == ':') {
            out += FILE_SEPARATOR;
            i++;
        } else {
            out += scope[i-1];
        }
    }

    return out;
}

std::string Import::scope_name(const std::string& file) {
    // Given the file name, returns the module's scope name.  This is equal
    // to the directory name with the '/' replaced by '::'.  For example,
    // 'Foo/Bar.ap' would have scope name 'Foo'.

    size_t pos = file.find_last_of(FILE_SEPARATOR);
    if (pos == std::string::npos) {
        return "";
    }
    std::string dir = file.substr(0, pos);
    std::string name;
    for (size_t i = 0; i < dir.size(); i++) {
        if (!isalnum(dir[i])) {
            if (dir[i] == FILE_SEPARATOR) {
                name += "::";
            } else {
                return name;
            }
        } else {
            name += dir[i];
        }
    }
    return name;
}

std::string Import::parent_scope(const std::string& scope) {
    // Given the import (a::b) return the parent scope (a).

    size_t pos = scope.find_last_of(':');
    if (pos == std::string::npos) {
        return "";
    }    
    if (pos > 0 && scope[pos-1] == ':') {
        return scope.substr(0, pos-1);
    } else {
        return scope;
    }
}
