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
    root_(new Module(Location(), name(""), this)),
    void_type_(new Type(Location(), name("Void"), 0, root_, this)),
    boolean_type_(new Type(Location(), name("Bool"), 0, root_, this)),
    integer_type_(new Type(Location(), name("Int"), 0, root_, this)),
    string_type_(new Type(Location(), name("String"), 0, root_, this)),
    no_type_(new Type(Location(), name("<<notype>>"), 0, root_, this)),
    float_type_(new Type(Location(), name("Float"), 0, root_, this)),
    self_type_(new Type(Location(), name("Self"), 0, root_, this)),
    errors_(0) {

    init_boolean();
    init_integer();
    init_string();
    init_float();
}

void Environment::init_boolean() {
    root_->feature(new Class(Location(), boolean_type_, 0, 0));
}

void Environment::init_integer() {
    Location loc;
    Formal::Ptr formals;

    Class::Ptr clazz = new Class(loc, integer_type_, 0, 0);

    formals = new Formal(loc, name("_"), integer_type_);
    formals->next(new Formal(loc, name("_"), integer_type_));
    clazz->feature(new Function(loc, name("@add"), formals, integer_type_, 0));
    
    formals = new Formal(loc, name("_"), integer_type_);
    formals->next(new Formal(loc, name("_"), integer_type_));
    clazz->feature(new Function(loc, name("@subtract"), formals, integer_type_, 0));

    formals = new Formal(loc, name("_"), integer_type_);
    formals->next(new Formal(loc, name("_"), integer_type_));
    clazz->feature(new Function(loc, name("@multiply"), formals, integer_type_, 0));

    formals = new Formal(loc, name("_"), integer_type_);
    formals->next(new Formal(loc, name("_"), integer_type_));
    clazz->feature(new Function(loc, name("@divide"), formals, integer_type_, 0));

    formals = new Formal(loc, name("_"), integer_type_);
    formals->next(new Formal(loc, name("_"), integer_type_));
    clazz->feature(new Function(loc, name("@modulus"), formals, integer_type_, 0));

    root_->feature(clazz);
}

void Environment::init_string() {
    root_->feature(new Class(Location(), string_type_, 0, 0));
}

void Environment::init_float() {
    root_->feature(new Class(Location(), float_type_, 0, 0));
}

Name* Environment::name(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, Name::Ptr>::iterator i = name_.find(str);
	if (i == name_.end()) {
		Name* name = new Name(str);
		name_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

Module* Environment::module(Name* scope) {
    // Look up a module by file_name path
    std::map<Name::Ptr, Module::Ptr>::iterator i = module_.find(scope);
    if (i == module_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

void Environment::module(Module* module) {
    module_[module->name()] = module;
    module->next(modules_);
    modules_ = module;
}
