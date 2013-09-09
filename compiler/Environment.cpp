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
    output_("out"),
    build_dir_(".build"),
    entry_point_("main"),
    dump_ast_(false),
    dump_lex_(false),
    dump_ir_(false),
    dump_liveness_(false),
    dump_regalloc_(false),
    dump_reggraph_(false),
    make_(false),
    optimize_(false),
    link_(true),
    assemble_(true),
    execute_(false),
    verbose_(false),
    monolithic_build_(true),
    no_default_libs_(false),
    generator_("Intel64"),
    errors_(0) {

    Location loc;
    root_ = new Module(loc, this, name(""));
    builtin_file_ = new File(name(""), name(""), root_, this);
    loc.file = builtin_file_;
    void_type_ = new Type(loc, name("Void"), 0, this);
    bool_type_ = new Type(loc, name("Bool"), 0, this);
    int_type_ = new Type(loc, name("Int"), 0, this);
    string_type_ = new Type(loc, name("String"), 0, this);
    nil_type_ = new Type(loc, name("Nil"), 0, this);
    top_type_ = new Type(loc, name("<<top>>"), 0, this);
    bottom_type_ = new Type(loc, name("<<bottom>>"), 0, this);
    float_type_ = new Type(loc, name("Float"), 0, this);
    char_type_ = new Type(loc, name("Char"), 0, this);
    byte_type_ = new Type(loc, name("Byte"), 0, this);
    pair_type_ = new Type(loc, name("Pair"), 0, this);
    any_type_ = new Type(loc, name("Any"), 0, this);
    enum_type_ = new Type(loc, name("Enum"), 0, this);
    object_type_ = new Type(loc, name("Object"), 0, this);
    functor_type_ = new Type(loc, name("Functor"), 0, this);
    value_type_ = new Type(loc, name("Value"), 0, this);
    interface_type_ = new Type(loc, name("Interface"), 0, this);
    union_type_ = new Type(loc, name("Union"), 0, this);
    appendable_type_ = new Type(loc, name("Appendable"), 0, this);

    module(root_);
}

String* Environment::name(const std::string& str) const {
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

String* Environment::integer(const std::string& str) const {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = integer_.find(str);
	if (i == integer_.end()) {
		String* name = new String(str);
		integer_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

String* Environment::floating(const std::string& str) const {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = floating_.find(str);
	if (i == floating_.end()) {
		String* name = new String(str);
		floating_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

String* Environment::string(const std::string& str) const {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, String::Ptr>::iterator i = string_.find(str);
	if (i == string_.end()) {
        String::Ptr name(new String(str));
		string_.insert(std::make_pair(str, name));
		return name;
	} else {
		return i->second;
	}
}

bool Environment::gen_library() const { 
    // If the loaded source does not have a "main" function, then generate a 
    // library instead of a program.
    Module::Ptr top = module(name("")); 
    Function::Ptr main = top->function(name("main")); 
    return !main; 
}

bool Environment::is_input(const std::string& import) const {
    // Returns true if the module name given by 'import' was an input given on
    // the command line.
    for (int i = 0; i < inputs(); i++) {
        if (input(i) == import) {
            return true;
        }
    }
    return false;
}

bool SubtypeKey::operator==(SubtypeKey const& other) const {
    return t1_->equals(other.t1_) && t2_->equals(other.t2_);
}

bool SubtypeKey::operator<(SubtypeKey const& other) const {
    if (t1_->equals(other.t1_)) {
        return t2_->operator<(*other.t2_);
    } else {
        return t1_->operator<(*other.t1_);
    }
}

SubtypeResult Environment::subtype(Type const* t1, Type const* t2) const {
    typedef std::map<SubtypeKey, SubtypeResult>::const_iterator Iter;
    Iter i = subtype_.find(SubtypeKey(t1, t2));
    if (i == subtype_.end()) {
        return UNCHECKED; 
    } else {
        return i->second;
    }
}

void Environment::subtype(Type const* t1, Type const* t2, SubtypeResult res) {
    subtype_[SubtypeKey(t1, t2)] = res;
}

void Environment::workspace_search(std::string prefix, std::string name) {
    // Searches for modules in directory "dir"
    std::string dir = prefix + FILE_SEPARATOR + name; 
    if (!File::is_dir(dir) || name[0] == '.') { return; }
    input(Import::module_name(name));
    for (File::Iterator i(dir); i; ++i) {
        if ((*i)[0] != '.') {
            workspace_search(prefix, name + FILE_SEPARATOR + *i);
        }
    }
}

void Environment::workspace_load() {
    // Sets up the environment for a default workspace.
    if (!inputs()) {
        for (File::Iterator i("src"); i; ++i) {
            std::string fn = std::string("src") + FILE_SEPARATOR + *i;
            workspace_search("src", *i);
        }
    }
    include("lib");
    include("src");
    include("runtime");
    build_dir("build");
}

Feature* Environment::feature(String* qn) const {
    // Attempts to find the function, attribute, class, or module with the
    // given fully-qualified name.
    std::string parent = Import::parent_scope(qn->string());
    std::string sub = Import::sub_scope(qn->string()); 

    Module::Ptr ret = module(qn);
    if (ret) { return ret; }

    if (parent.empty()) {
        // No parent, so the requested node is a top-level function, module,
        // attribute or class.
        return root()->feature(name(sub));
    } else {
        Module::Ptr mod = module(name(parent));
        Feature::Ptr ret = mod ? mod->feature(name(sub)) : 0;
        if (ret) { return ret; }
        // Found a module-level identifier.
        
        std::string mn = Import::parent_scope(parent);
        std::string cn = Import::sub_scope(parent);
        mod = module(name(mn));
        Class::Ptr clazz = mod ? mod->clazz(name(cn)) : 0; 
        return clazz ? clazz->feature(name(sub)) : 0;
        // Found an identifier that was nested within a class.
    }
}

//void Environment::feature(String* scope) {
//}
