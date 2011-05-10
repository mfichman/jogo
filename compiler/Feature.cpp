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

#include "Feature.hpp"
#include "Environment.hpp"
#include <iostream>

using namespace std;

Class::Class(Location loc, Type* type, Type* mixins, Feature* features) :
    Feature(loc),
    type_(type),
    mixins_(mixins),
    features_(features),
    is_object_(false),
    is_value_(false),
    is_interface_(false) {

    for (Feature* feat = features; feat; feat = feat->next()) {
        if (Attribute* attr = dynamic_cast<Attribute*>(feat)) {
            attributes_[attr->name()] = attr;
            continue;
        }
        if (Function* func = static_cast<Function*>(feat)) {
            functions_[func->name()] = func;
            continue;
        }
    }

    for (Type* mixin = mixins; mixin; mixin = mixin->next()) {
        if ("Object" == mixin->name()->string()) {
            is_object_ = true;
            continue;
        }
        if ("Interface" == mixin->name()->string()) {
            is_interface_ = true;
            continue;
        }
        if ("Value" == mixin->name()->string()) {
            is_value_ = true;
            continue;
        }
    }
}

void Class::feature(Feature* feature) {
    if (!feature) {
        return;
    }

    feature->next(features_);
    features_ = feature;

    if (Attribute* attr = dynamic_cast<Attribute*>(feature)) {
        attributes_[attr->name()] = attr;
        return;
    }
    if (Function* func = dynamic_cast<Function*>(feature)) {
        functions_[func->name()] = func;
        return;
    }
}

Attribute* Class::attribute(Name* name) const {
    std::map<Name::Ptr, Attribute::Ptr>::const_iterator i = attributes_.find(name);
    return (i == attributes_.end()) ? 0 : i->second;
}

Function* Class::function(Name* name) const {
    std::map<Name::Ptr, Function::Ptr>::const_iterator i = functions_.find(name);
    return (i == functions_.end()) ? 0 : i->second;
}

bool Class::subtype(Class* other) const {
    // Returns true if this class is a subtype of 'other.'  A class is a subtype
    // of another class if it implements all methods found in the class.
    
    // Check if this comparison has been done before.  If so, then we don't need
    // to do it again.
    std::map<Class*, bool>::iterator i = subtype_.find(other);
    if (i != subtype_.end()) {
        return i->second;
    }
    if (!other->is_interface() && !is_interface()) {
        return this == other; 
    }

    // Assume that this type is a subtype of 'other.'  This will break cycles
    // caused when checking function return values for covariance.
    subtype_[other] = true;

    for (Feature* f = other->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f)) {
            Function* mine = function(func->name());   
            if (!mine || !mine->covariant(func)) {
                // Now we know that the types are not compatible, so mark
                // 'other' as a disjoint type.
                subtype_[other] = false;
                return false;
            }
        }
    }
    return true;
}


bool Function::covariant(Function* other) const {
    // Returns true if this function is covariant, i.e., its return type is the
    // same as or a subtype of 'other.'  The arguments must match exactly.
    Formal* f1 = formals_;
    Formal* f2 = other->formals_;

    while (f1 && f2) {
        if (f1->type()->name()->string() == "Self" 
            && f2->type()->name()->string() == "Self") {
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

void Module::feature(Feature* feature) {
    if (!feature) {
        return;
    }

    feature->next(features_);
    features_ = feature;

    if (Class* clazz = dynamic_cast<Class*>(feature)) {
        classes_[clazz->name()] = clazz;
        return;
    }
    if (Function* func = dynamic_cast<Function*>(feature)) {
        functions_[func->name()] = func;
        return;
    }
}

Function* Module::function(Name* scope, Name* name) {
    // Returns the function with scope "scope" and name "name."  Searches
    // through imports included in this module to attempt to find the function.

    if (!scope || scope->string() == "") {
        Function* fn = function(name);
        if (fn) {
            return fn;
        }
        return environment_->root()->function(name);
    }

    Module* module = this->module(scope);
    if (!module) {
        return 0;
    }
    return module->function(name);
}

Class* Module::clazz(Name* scope, Name* name) {
    // Returns the class with the scope "scope" and name "name."  Searches
    // through imports included in this module to attempt to find the class.

    if (!scope || scope->string() == "") {
        Class* cs = clazz(name);
        if (cs) {
            return cs;
        }
        return environment_->root()->clazz(name);
    }

    Module* module = this->module(scope);
    if (!module) {
        return 0;
    }
    return module->clazz(name);
}

Module* Module::module(Name* scope) {
    // Returns the module with the name "scope."  Searches through imports
    // included in this module to attempt to find the module.

    Module* module = environment_->module(scope);

    std::map<Name::Ptr, Import::Ptr>::iterator i = imports_.begin();
    for (; i != imports_.end(); i++) {
        Name* is = i->second->scope();
        Name* mn = environment_->name(scope->string() + is->string());
        Module* m = environment_->module(mn);
        if (module && m) {
            // Ambiguous: more than one module that matches this name
            return 0;
        }
    }
    return module;
}

std::string Import::file_name(Name* scope) {
    // Converts a module name to the name of the file that contains the 
    // module.
    const std::string& name = scope->string();
    std::string out;
    for (size_t i = 1; i <= name.length(); i++) {    
        if (name[i-1] == ':' && name[i] == ':') {
            out += '/';
            i++;
        } else {
            out += name[i-1];
        }
    }

    return out + ".ap";
}

std::string Import::base_name(const std::string& file) {
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

std::string Import::scope_name(const std::string& file) {
    // Given the file name, returns the module's scope name

    std::string name;
    for (size_t i = 0; i < file.size(); i++) {
        if (!isalnum(file[i])) {
            if (file[i] == '/') {
                name += "::";
            } else {
                return name;
            }
        } else {
            name += file[i];
        }
    }
    return name;
}
