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

#include "Feature.hpp"
#include "Environment.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

Class::Class(Location loc, Environment* env, Type* type, Type* proto, 
    String* comment, Feature* feat) :
    Feature(loc, env, type->name()),
    type_(type),
    proto_(proto),
    comment_(comment),
    features_(feat),
    slots_(0) {

    assert(type_);
    assert(proto_);

    for (Feature* feat = features_; feat; feat = feat->next()) {
        feat->parent(this);
        if (!feature_[feat->name()]) {
            feature_[feat->name()] = feat;
        }        
    }
    if (proto->is_enum_proto()) {
        gen_equal_method();
    }
    if (proto->is_functor_proto()) {
        gen_functor_method();
    }
}

/* Constructor for union types */
Class::Class(Location loc, Environment* env, Type* type, Type* alt) :
    Feature(loc, env, type->name()),
    type_(type),
    alternates_(alt),
    proto_(env->union_type()),
    slots_(0) {

    assert(type_);
    assert(proto_);
}

/* Constructor for enum types */
Class::Class(Location loc, Environment* env, Type* type, Feature* feat) :
    Feature(loc, env, type->name()),
    type_(type),
    proto_(env->enum_type()),
    features_(feat),
    slots_(0) {

    assert(type_);
    assert(proto_);

    for (Feature* feat = features_; feat; feat = feat->next()) {
        feat->parent(this);
        if (!feature_[feat->name()]) {
            feature_[feat->name()] = feat;
        }        
    }
    gen_equal_method();
}

/* Constructor for anonymous interfaces */
Class::Class(Location loc, Environment* env, Type* type) :
    Feature(loc, env, type->name()),
    type_(type),
    proto_(env->interface_type()),
    comment_(0),
    features_(0),
    slots_(0) {
}

void Class::gen_functor_method() {
    // Gen the functor @call method
    String::Ptr name = env()->name("@call");
    if(!feature(name)) {
        Type::Ptr ret = env()->void_type();
        Type::Ptr at = env()->any_type();
        Formal::Ptr self(new Formal(location(), env()->name("__self"), type()));
        self->next(new Formal(location(), env()->name("obj"), at));
        Block::Ptr block(new Block(location(), env()->string(""), 0));
        feature(new Function(location(), env(), name, self, 0, ret, block));
    }
}

void Class::gen_equal_method() {
    // Generates an automatic @equal method when necessary.
    String::Ptr name = env()->name("@equal");
    Feature::Ptr feat = feature(name);
    if(!feat||feat->parent()!=this) {
        Type::Ptr ret = env()->bool_type();
        Formal::Ptr self(new Formal(location(), env()->name("__self"), type())); 
        self->next(new Formal(location(), env()->name("other"), type())); 
        Feature::Flags flags = Feature::NATIVE;
        Function::Ptr func = new Function(location(), env(), name, self, flags, ret, 0);
        feature(func);
    }
}

Function* Class::destructor() const {
    return function(env()->name("@destroy")); 
}

Function* Class::constructor() const {
    return function(env()->name("@init"));
}

Function* Class::copier() const {
    return function(env()->name("@copy"));
}

String* Class::default_enum_value() const {
    // Returns the first (default) value in the enumeration.
    for (Feature* feat = features_; feat; feat = feat->next()) {
        if (Constant* cons = dynamic_cast<Constant*>(feat)) {
            Expression* init = cons->initializer();
            if (IntegerLiteral* lit = dynamic_cast<IntegerLiteral*>(init)) {
                return lit->value();
            }
        }
    }
    return env()->integer("0");
}

void Class::jump1(int index, int d) {
    if (index >= jump1_.size()) {
        jump1_.resize(index+1);
    }
    jump1_[index] = d; 
}

void Class::jump2(int index, Function* func) {
    if (index >= jump2_.size()) {
        jump2_.resize(index+1);
    }
    jump2_[index] = func; 
}

void Class::feature(Feature* feature) {
    if (!feature) {
        return;
    }
    feature->parent(this);
    features_ = append(features_, feature);
    if (!feature_[feature->name()]) {
        feature_[feature->name()] = feature;
    }
}

Feature* Class::feature(String* name) const {
    // Searches for a feature with name 'name' in this lass or in an embedded
    // attribute that was added to this class.

    std::map<String::Ptr, Feature::Ptr>::const_iterator i = feature_.find(name);
    if (i != feature_.end()) {
        return i->second;
    }

    if (type()->is_proto()) {
        return 0;
    }

    for (Feature::Ptr f = features_; f; f = f->next()) {
        if (!f->is_embedded()) { continue; }
        Attribute* attr = dynamic_cast<Attribute*>(f.pointer());
        Type* type = attr->type();
        if (!type) {
            type = attr->declared_type();
        }
        Class* clazz = type->clazz();
        if (clazz && clazz != this) {
            Feature* func = clazz->feature(name);
            if (func) { return func; }    
        } 
    }
    Class* clazz = env()->object_type()->clazz();
    if(clazz && clazz != this) {
        return clazz->feature(name);
    }
    return 0;
}

Constant::Constant(Location loc, Environment* env, String* name, Flags flags, 
    Type* type, Expression* init) :
    Feature(loc, env, name, flags),
    declared_type_(type),
    initializer_(init) {
}

Function::Function(Location loc, Environment* env, String* name, Formal* formal, 
    Flags flags, Type* ret, Block* block) :

    Feature(loc, env, name, flags),
	formals_(formal),
	type_(ret),
    block_(block),
    local_slots_(0),
    arg_slots_(0),
    temp_regs_(0),
	throw_spec_(UNKNOWN),
    is_checked_(false) {

    assert(type_);
}

bool Function::covariant(Function* other) const {
    // Returns true if this function is covariant, i.e., its return type is the
    // same as or a subtype of 'other.'  The arguments must match exactly.
    Formal* f1 = formals_;
    Formal* f2 = other->formals_;

    while (f1 && f2) {
        if (f1->is_self() && f2->is_self()) {
            /* pass */
        } else if (!f1->type()->equals(f2->type())) {
            return false;
        }
        f1 = f1->next();
        f2 = f2->next();
    }
    if (f1 || f2) {
        return false;
    }
    if (!type_->subtype(other->type())) {
        return false;
    } 

    return true;
}

bool Function::is_primitive_op() const {
	// Returns true if this function is a primitive operand, which is output
	// directly into machine code.
	Class* clazz = dynamic_cast<Class*>(parent());
	return clazz && clazz->is_primitive() && name()->string()[0] == '@';
}

bool Function::is_virtual() const {
    // Returns true if the function is virtual.
    Class* clazz = dynamic_cast<Class*>(parent());
    return clazz && clazz->is_interface();
}

int Function::stack_slots() const {
#ifdef WINDOWS
    // Windows requires that at least 4 argument slots are allocated,
    // regardless of whether or not they are used.  This space can be used
    // by the called function, so if it is not allocated properly then the
    // called function can smash the calling function's stack.
    int const win_min_arg_slots = 4;
    return std::max(arg_slots_, win_min_arg_slots) + local_slots_;
#else
    return arg_slots_ + local_slots_; 
#endif
}

Function::ThrowSpec Function::throw_spec() const {
	// Returns THROW if this function can throw an exception; otherwise
	// returns NOTHROW.  The result is computed lazily, but all of the 
	// children must be added to the called_func array or the computation
	// will be reset.
	if (throw_spec_ != UNKNOWN) {
		return throw_spec_;
	}
	if (is_virtual()) {
		throw_spec_ = THROW;
		return throw_spec_;
	}
    throw_spec_ = NOTHROW;
	for (int i = 0; i < called_func_.size(); i++) {
		if (called_func_[i]->throw_spec() == THROW) {
			throw_spec_ = THROW;
			return throw_spec_;
		}
	}
	
	return throw_spec_;
}

void Function::called_func(Function* func) {
	// Reset the throw_spec, because adding a new function that throws
	// could cause this function to throw.
	called_func_.push_back(func);
}

Module::Module(Location loc, Environment* env, String* name) :
    Feature(loc, env, name) { 
}

void Module::feature(Feature* feature) {
    if (!feature) {
        return;
    }
    feature->parent(this);
    feature_.insert(feature); 
}

bool Module::is_exe() const {
    // Returns true if this module is an executable module (false if it is a
    // static library).
    return function(env()->name("main"));
}

bool Module::is_up_to_date() const {
    // Return true if the module is up to date, that is, if all dependent
    // source files have a later modification time than the output file.  Also,
    // check dependent packages as well to see if they need to be relinked.
    std::string out = function(env()->name("main")) ? exe_file() : lib_file();
    File::Ptr libjogo = env()->file(env()->name(Module::jgi_file("jogo")));
    for (int i = 0; i < files(); i++) {
        File::Ptr file = Module::file(i);
        std::string const& source = file->path()->string();
        if (!File::is_up_to_date(source, out)) {
            return false;
        }
        std::string const& native = file->native_file();
        if (File::is_reg(native) && !File::is_up_to_date(native, out)) {
            return false;
        }
        for (Import::Itr im = file->imports(); im; ++im) {
            String::Ptr scope = im->scope();
            Module::Ptr mod = env()->module(scope);
            if (mod) {
               std::string const& lib = mod->lib_file();
               if (File::is_reg(lib) && !File::is_up_to_date(lib, out)) {
                   return false;
               }
            }
        }
        if (!File::is_up_to_date(libjogo->path()->string(), out)) {
            return false;
        }
    }
    return true;
}

std::string Module::file_base() const { 
    return file_base(name()->string()); 
}

std::string Module::lib_file() const { 
    std::string dir = env()->output() + FILE_SEPARATOR + "lib";
    return dir + FILE_SEPARATOR + lib_file(name()->string()); 
}

std::string Module::exe_file() const { 
    std::string dir = env()->output() + FILE_SEPARATOR + "bin";
    return dir + FILE_SEPARATOR + exe_file(name()->string()); 
}

std::string Module::jgi_file() const { 
    std::string dir = env()->output() + FILE_SEPARATOR + "lib";
    return dir + FILE_SEPARATOR + jgi_file(name()->string()); 
}

std::string Module::file_base(std::string const& name) {
    // The file name for a module is the module name, lower-cased, with all
    // "::" substrings replaced by "-" (":" is not a valid file name character
    // on Windows platforms)
    std::string nm;
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == ':' && name[i+1] == ':') {
            nm += '-';
            ++i;
        } else {
            nm += tolower(name[i]);
        }
    }
    return nm;
}

std::string Module::lib_file(std::string const& name) {
    std::string nm = file_base(name);
#ifdef WINDOWS
    return nm + ".lib";
#else
    return nm + ".a";
#endif
}

std::string Module::exe_file(std::string const& name) {
    std::string nm = file_base(name);
#ifdef WINDOWS
    return nm + ".exe";
#else
    return nm;
#endif
} 

std::string Module::jgi_file(std::string const& name) {
    std::string nm = file_base(name);
    return nm + ".jgi";
}   

Class* Feature::clazz(String* name) const {
    return dynamic_cast<Class*>(feature(name));
}

Attribute* Feature::attribute(String* name) const {
    return dynamic_cast<Attribute*>(feature(name));
}

Constant* Feature::constant(String* name) const {
    return dynamic_cast<Constant*>(feature(name));
}

Function* Feature::function(String* name) const {
    return dynamic_cast<Function*>(feature(name));
}

String* Feature::qualified_name() const {
    // Returns the qualified name for this feature

    std::string parent = parent_ ? parent_->qualified_name()->string() : "";
    std::string name = parent.empty() ? "" : parent + "::";
    name += Feature::name()->string(); 
    return env_->name(name);
}

String* Feature::label() const {
    // Builds the label for a feature if it hasn't already been calculated.
    if (label_) { return label_; }

    std::string parent = parent_ ? parent_->label()->string() : "";
    std::string name = parent.empty() ? "" : parent + "_";
    name += Feature::name()->string();    

    std::string label;
    label.reserve(name.length());
    for (size_t i = 0; i < name.length(); i++) {
        if (name[i] == '@') {
            label += '_';
        } else if (name[i] == '?') {
            label += "__g";
        } else if (name[i] == '!') {
            label += "__w";
        } else if (name[i] == '=') {
            label += "__s";
        } else if (name[i] == ':') {
            label += "_";
            i++;
        } else {
            label += name[i];
        }
    }

    label_ = env_->name(label);
    return label_;
}

