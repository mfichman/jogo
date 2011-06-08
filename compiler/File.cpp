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
#include <String.hpp>
#include <Feature.hpp>
#include <Environment.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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

    feature->next(features_);
    features_ = feature;

    if (Import* import = dynamic_cast<Import*>(feature)) {
        if (import->scope()->string().empty()) {
            return;
        }

        imports_.push_back(import);
        return;
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
