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

#pragma once

#include <Object.hpp>
#include <Feature.hpp>
#include <string>
#include <vector>

/* Basic file operations and utilites */
class File : public Object {
public:
    File(String* name, Module* module, Environment* env) :
        name_(name),
        module_(module),
        environment_(env) {  
    }

    Feature* features() const { return features_; }
    Function* function(String* scope, String* name);
    Class* clazz(String* scope, String* name);
    String* name() const { return name_; }
    void feature(Feature* feature);
    typedef Pointer<File> Ptr;

    class Iterator;
    static bool is_dir(const std::string& file);
    static bool is_reg(const std::string& file);
    static std::string base_name(const std::string& file);

private:
    Feature::Ptr features_;
    String::Ptr name_;
    Module::Ptr module_;
    Environment* environment_;
    std::vector<Import::Ptr> imports_; 
};

/* Directory iterator object */
class File::Iterator {
public:
    Iterator(const std::string& file);
    ~Iterator();
    const Iterator& operator++();
    std::string operator*() const;
    operator bool() const;
    
    
private:
    void* handle_;
    void* entry_;
};

