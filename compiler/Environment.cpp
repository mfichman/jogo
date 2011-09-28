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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Environment.hpp"
#include <cassert>
#include <stack>

Environment::Environment() :
    output_("-"),
    output_dir_("build"),
    dump_ast_(false),
    dump_ir_(false),
    dump_liveness_(false),
    make_(false),
    optimize_(false),
    link_(true),
    assemble_(true),
    errors_(0) {

    root_ = new Module(Location(), name(""), this);
    builtins_ = new Module(Location(), name(""), this);
    builtin_file_ = new File(0, 0, builtins_, this);
    void_type_ = new Type(Location(), name("Void"), 0, builtin_file_, this);
    bool_type_ = new Type(Location(), name("Bool"), 0, builtin_file_, this);
    int_type_ = new Type(Location(), name("Int"), 0, builtin_file_, this);
    string_type_ = new Type(Location(), name("String"), 0, builtin_file_, this);
    nil_type_ = new Type(Location(), name("Nil"), 0, builtin_file_, this);
    no_type_ = new Type(Location(), name("<<notype>>"), 0, builtin_file_, this);
    float_type_ = new Type(Location(), name("Float"), 0, builtin_file_, this);
    char_type_ = new Type(Location(), name("Char"), 0, builtin_file_, this);
    byte_type_ = new Type(Location(), name("Byte"), 0, builtin_file_, this);
    pair_type_ = new Type(Location(), name("Pair"), 0, builtin_file_, this);
    self_type_ = new Type(Location(), name("Self"), 0, builtin_file_, this);

    module(root_);

    include(".");
    lib("apollo");
}

String* Environment::name(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = name_.find(str);
	if (i == name_.end()) {
		String* name = new String(str);
		name_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

String* Environment::integer(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = integer_.find(str);
	if (i == integer_.end()) {
		String* name = new String(str);
        name->next(integers_);
        integers_ = name;
		integer_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

String* Environment::floating(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = floating_.find(str);
	if (i == floating_.end()) {
		String* name = new String(str);
        name->next(floats_);
        floats_ = name;
		floating_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

String* Environment::string(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = string_.find(str);
	if (i == string_.end()) {
		String* name = new String(str);
        name->next(strings_);
        strings_ = name; 
		string_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

Module* Environment::module(String* scope) {
    // Look up a module by file_name path
    std::map<String::Ptr, Module::Ptr>::iterator i = module_.find(scope);
    if (i == module_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

File* Environment::file(String* scope) {
    // Look up a module by file_name path
    std::map<String::Ptr, File::Ptr>::iterator i = file_.find(scope);
    if (i == file_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

void Environment::file(File* file) {
    file_[file->name()] = file;
    file->next(files_);
    files_ = file;
}

void Environment::module(Module* module) {
    module_[module->name()] = module;
    module->next(modules_);
    modules_ = module;
}
