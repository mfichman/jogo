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

#include <Object.hpp>
#include <Feature.hpp>
#include <string>
#include <vector>

#ifdef WINDOWS
#define FILE_SEPARATOR '\\'
#define FILE_SEPARATOR_STR "\\"
#else
#define FILE_SEPARATOR '/'
#define FILE_SEPARATOR_STR "/"
#endif

/* Basic file operations and utilites */
class File : public Object {
public:
    File(String* name, String* path, Module* module, Environment* env) :
        name_(name),
        path_(path),
        module_(module),
        env_(env),
        is_input_file_(false),
        is_output_file_(true) {  
    }
    static std::string const API;
    static std::string const ASM;
    static std::string const C;
    static std::string const APO;
    static std::string const O; 
    
    Feature* dependency(int id) { return dependency_[id]; }
    Feature* feature(String* scope, String* name) const;
    Function* function(String* scope, String* name) const;
    Constant* constant(String* scope, String* name) const;
    Class* clazz(String* scope, String* name) const;
    String* name() const { return name_; }
    String* path() const { return path_; }
    File* next() const { return next_; }
    Constant* constant(int index) { return constant_[index]; }
    String* integer(const std::string& str);
    String* floating(const std::string& str);
    String* string(const std::string& str);
    String* integers() const { return integers_; }
    String* floats() const { return floats_; }
    String* strings() const { return strings_; }
    Import* import(int index) { return import_[index]; }
    Feature* feature(int index) { return feature_[index]; }
    int imports() const { return import_.size(); }
    int features() const { return feature_.size(); }
    int dependencies() { return dependency_.size(); }
    int constants() { return constant_.size(); }
    bool is_input_file() const { return is_input_file_; }
    bool is_output_file() const;
    bool is_interface_file() const { return ext(name_->string())==API; }
    bool is_up_to_date(const std::string& ext) const;
    std::string input(const std::string& ext) const;
    std::string output(const std::string& ext = APO) const;
    std::string asm_file() const { return output(ASM); }
    std::string c_file() const { return output(C); }
    std::string o_file() const { return output(O); }
    std::string apo_file() const { return output(APO); }
    std::string native_file() const { return input(C); }
    void dependency(Feature* name);
    void import(Import* import) { import_.push_back(import); }
    void feature(Feature* feature) { feature_.push_back(feature); }
    void next(File* next) { next_ = next; }
    void constant(Constant* constant) { constant_.push_back(constant); }
    void is_input_file(bool input) { is_input_file_ = input; }
    void is_output_file(bool output) { is_output_file_ = output; }
    typedef Pointer<File> Ptr;

    class Iterator;
    static bool is_dir(const std::string& file);
    static bool is_reg(const std::string& file);
    static bool is_native_lib(const std::string& file);
    static bool is_up_to_date(const std::string& in, const std::string& out);
    static std::string base_name(const std::string& file);
    static std::string ext(const std::string& file);
    static std::string no_ext_name(const std::string& file);
    static std::string dir_name(const std::string& file);
    static bool mkdir(const std::string& file);
    static time_t mtime(const std::string& file);

private:
    String::Ptr name_;
    String::Ptr path_;
    Module::Ptr module_;
    Environment* env_;
    File::Ptr next_;
    bool is_input_file_;
    bool is_output_file_;
    std::vector<Import::Ptr> import_; 
    std::vector<Feature::Ptr> feature_;
    std::vector<Feature::Ptr> dependency_;
    std::vector<Constant::Ptr> constant_;
    std::map<std::string, String::Ptr> integer_;
    std::map<std::string, String::Ptr> floating_;
    std::map<std::string, String::Ptr> string_;
    String::Ptr strings_;
    String::Ptr integers_;
    String::Ptr floats_;
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
#ifdef WINDOWS
    HANDLE handle_;
    WIN32_FIND_DATA data_;
#else
    void* handle_;
    void* entry_;
#endif
};

