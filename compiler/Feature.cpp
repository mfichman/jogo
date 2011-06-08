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

Class::Class(Location loc, Type* t, Type* mixins, String* cmt, Feature* f) :
    Feature(loc),
    type_(t),
    mixins_(mixins),
    comment_(cmt),
    features_(f),
    is_object_(false),
    is_value_(false),
    is_interface_(false) {

    for (Feature* feat = features_; feat; feat = feat->next()) {
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

Attribute* Class::attribute(String* name) const {
    std::map<String::Ptr, Attribute::Ptr>::const_iterator i = attributes_.find(name);
    return (i == attributes_.end()) ? 0 : i->second;
}

Function* Class::function(String* name) const {
    std::map<String::Ptr, Function::Ptr>::const_iterator i = functions_.find(name);
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
    if (!other->is_interface()) {
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

std::string Import::file_name(String* scope) {
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

    return out;
}

std::string Import::scope_name(const std::string& file) {
    // Given the file name, returns the module's scope name.  This is equal
    // to the directory name with the '/' replaced by '::'.  For example,
    // 'Foo/Bar.ap' would have scope name 'Foo'.

    size_t pos = file.find_last_of('/');
    if (pos == string::npos) {
        return "";
    }
    std::string dir = file.substr(0, pos);
    std::string name;
    for (size_t i = 0; i < dir.size(); i++) {
        if (!isalnum(dir[i])) {
            if (dir[i] == '/') {
                name += "::";
            } else {
                return name;
            }
        } else {
            name += dir[i];
        }
    }
    return name;
}
