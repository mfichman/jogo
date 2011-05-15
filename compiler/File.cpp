/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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

#include <File.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

bool File::is_dir(const std::string& name) {
    struct stat info;
    if (stat(name.c_str(), &info)) {
        return false;
    } else {
        return S_ISDIR(info.st_mode);
    } 
}

bool File::is_reg(const std::string& name) {
    struct stat info;
    if (stat(name.c_str(), &info)) {
        return false;
    } else {
        return S_ISREG(info.st_mode);
    } 
}

std::string File::base_name(const std::string& file) {
    // Returns the last component of a file path, without the .ap extension.

    size_t slash = file.find_last_of('/');
    size_t dot = file.find_last_of('.');

    if (dot == std::string::npos) {
        dot = file.size();
    }

    if (slash == std::string::npos) {
        slash = 0;
    } else {
        slash++;
    }
    
    if (dot < slash) {
        dot = slash;
    }
    
    return file.substr(slash, dot - slash); // slashdot!!
}

File::Iterator::Iterator(const std::string& file) :
    handle_(opendir(file.c_str())),
    entry_(handle_ ? readdir((DIR*)handle_) : 0) {
}

File::Iterator::~Iterator() {
    closedir((DIR*)handle_);
}

const File::Iterator& File::Iterator::operator++() {
    entry_ = handle_ ? readdir((DIR*)handle_) : 0;
    return *this;
}

std::string File::Iterator::operator*() const {
    return std::string(((dirent*)entry_)->d_name);
}

File::Iterator::operator bool() const {
    return entry_ != 0;
}

