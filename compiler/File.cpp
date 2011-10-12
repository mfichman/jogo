/*
 * Copyright (c) 2011 Matt Fichman
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

#include <File.hpp>
#include <String.hpp>
#include <Feature.hpp>
#include <Environment.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WINDOWS
#define stat _stat
#define S_ISDIR(x) ((x) & _S_IFDIR)
#define S_ISREG(x) ((x) & _S_IFREG)
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#endif

Function* File::function(String* scope, String* name) {
    // Returns the function with the scope "scope" and name "name."  Searches
    // through imports included in this file to attempt to find the function.

    if (scope && scope->string() != "") {
        // The scope was specified, so load the function using the full scope
        // specifier.
        Module* module = environment_->module(scope);
        return module ? module->function(name) : 0;
    }

    // Attempt to load the function from the current module
    Function* fn = module_->function(name);
    if (fn) {
        return fn;
    }

    // Search the imports for the function in question 
    std::vector<Import::Ptr>::iterator i = imports_.begin();
    for (; i != imports_.end(); i++) {
        if ((*i)->is_qualified()) {
            continue;
        }
        Module* m = environment_->module((*i)->scope());
        if (!m) {
            continue;
        }
        fn = m->function(name);
        if (fn) {
            return fn;
        }
    }

    // Load from the global scope
    fn = environment_->root()->function(name);
    if (fn) {
        return fn;
    }

    // Load from the builtin scope 
    return environment_->builtins()->function(name);
}

Class* File::clazz(String* scope, String* name) {
    // Returns the class with the scope "scope" and name "name."  Searches
    // through imports included in this module to attempt to find the class.

    if (scope && scope->string() != "") {
        // The scope was specified, so load the class using the full scope
        // specifier.
        Module* module = environment_->module(scope);
        return module ? module->clazz(name) : 0;
    }

    // Attempt to load the class from the current module
    Class* cs = module_->clazz(name);
    if (cs) {
        return cs;
    }

    // Search the imports for the class in question 
    std::vector<Import::Ptr>::iterator i = imports_.begin();
    for (; i != imports_.end(); i++) {
        if ((*i)->is_qualified()) {
            continue;
        }
        Module* m = environment_->module((*i)->scope());
        if (!m) {
            continue;
        }
        cs = m->clazz(name);
        if (cs) {
            return cs;
        }
    }

    // Load from the global scope
    cs = environment_->root()->clazz(name);
    if (cs) {
        return cs;
    }

    // Load from the builtin scope 
    return environment_->builtins()->clazz(name);
}

void File::feature(Feature* feature) {
    if (!feature) {
        return;
    }

    features_ = append(features_, feature);

    if (Import* import = dynamic_cast<Import*>(feature)) {
        if (import->scope()->string().empty()) {
            return;
        }

        imports_.push_back(import);
        return;
    }
}

time_t File::mtime(const std::string& name) {
    struct stat info;
    if (stat(name.c_str(), &info)) {
        return 0;
    } else {
        return info.st_mtime;
    }

}

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

bool File::is_native_lib(const std::string& name) {
#ifdef WINDOWS
    if (!name.compare(name.length()-2, std::string::npos, ".lib")) {
        return true;
    }
    if (!name.compare(name.length()-2, std::string::npos, ".dll")) {
        return true;
    }
#else
    if (!name.compare(name.length()-2, std::string::npos, ".a")) {
        return true;
    }
    if (!name.compare(name.length()-3, std::string::npos, ".so")) {
        return true;
    }
#endif

    return false;
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

std::string File::no_ext_name(const std::string& file) {
    size_t dot = file.find_last_of('.');
    if (dot == std::string::npos) {
        return file;
    } else {
        return file.substr(0, dot);
    }
}

std::string File::ext(const std::string& file) {
    // Returns the component after the last '.'

    size_t dot = file.find_last_of('.');
    if (dot == std::string::npos) {
        return "";
    } else {    
        return file.substr(dot);
    }
}

std::string File::dir_name(const std::string& file) {
    size_t slash = file.find_last_of('/');
    if (slash == std::string::npos) {
        return ".";
    } else {
        return file.substr(0, slash);
    }
}

bool File::mkdir(const std::string& file) {
    // Recursively makes all directories in the path specified by 'file'.

    size_t pos = file.find_last_of('/');
    if (pos != std::string::npos) {
        File::mkdir(file.substr(0, pos));
    }

#ifdef WINDOWS
    if (CreateDirectory(file.c_str(), 0)) {
        return GetLastError() == ERROR_ALREADY_EXISTS;
    }
#else
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    if (::mkdir(file.c_str(), mode)) {
        return errno == EEXIST;
    }
#endif
    return true;
}

File::Iterator::Iterator(const std::string& file) :
#ifdef WINDOWS
    handle_(FindFirstFile(file.c_str(), &data_)) {
#else
    handle_(opendir(file.c_str())),
    entry_(handle_ ? readdir((DIR*)handle_) : 0) {
#endif
}

File::Iterator::~Iterator() {
#ifdef WINDOWS
    FindClose(handle_);
#else
    if (handle_) { closedir((DIR*)handle_); }
#endif
}

const File::Iterator& File::Iterator::operator++() {
#ifdef WINDOWS
    if (!FindNextFile(handle_, &data_)) {
        FindClose(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }
#else
    entry_ = handle_ ? readdir((DIR*)handle_) : 0;
#endif
    return *this;
}

std::string File::Iterator::operator*() const {
#ifdef WINDOWS
    return std::string(data_.cFileName);
#else
    return std::string(((dirent*)entry_)->d_name);
#endif
}

File::Iterator::operator bool() const {
#ifdef WINDOWS
    return handle_ != INVALID_HANDLE_VALUE;
#else
    return entry_ != 0;
#endif
}

