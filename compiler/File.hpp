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
#include <Iterator.hpp>
#include <string>
#include <vector>

#ifdef WINDOWS
#define FILE_SEPARATOR '\\'
#define FILE_SEPARATOR_STR "\\"
#define PATH_SEPARATOR ';'
#else
#define FILE_SEPARATOR '/'
#define FILE_SEPARATOR_STR "/"
#define PATH_SEPARATOR ':'
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
    static std::string const JGI;
    static std::string const ASM;
    static std::string const C;
    static std::string const JGO;
    static std::string const JGC;
    static std::string const O; 
    
    Feature* feature(String* scope, String* name) const;
    Function* function(String* scope, String* name) const;
    Constant* constant(String* scope, String* name) const;
    Class* clazz(String* scope, String* name) const;
    Import* import(String* name) const { return query(import_, name); }
    String* name() const { return name_; }
    String* path() const { return path_; }
    String* full_path() const;
    Import::Itr imports() const { return Import::Itr(import_); }
    Feature::Itr features() const { return Feature::Itr(feature_); }
    Constant::Itr constants() const { return Constant::Itr(constant_); }
    bool is_input_file() const { return is_input_file_; }
    bool is_output_file() const;
    bool is_interface_file() const { return ext(name_->string())==JGI; }
    std::string input(const std::string& ext) const;
    std::string output(const std::string& ext = JGO) const;
    std::string asm_file() const { return output(ASM); }
    std::string c_file() const { return output(C); }
    std::string o_file() const { return output(O); }
    std::string jgo_file() const { return output(JGO); }
    std::string jgc_file() const { return output(JGC); }
    std::string native_file() const { return input(C); }
    void import(Import* import) { import_[import->scope()] = import; }
    void feature(Feature* feature) { feature_.push_back(feature); }
    void constant(Constant* constant) { constant_.push_back(constant); }
    void is_input_file(bool input) { is_input_file_ = input; }
    void is_output_file(bool output) { is_output_file_ = output; }
    typedef Pointer<File> Ptr;
    typedef ::Iterator<File> Itr;

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
    static std::string cwd();

private:
    String::Ptr name_;
    String::Ptr path_;
    Module::Ptr module_;
    Environment* env_;
    bool is_input_file_;
    bool is_output_file_;
    mutable std::map<String::Ptr, Import::Ptr> import_; 
    mutable std::vector<Feature::Ptr> feature_;
    mutable std::vector<Constant::Ptr> constant_;
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

