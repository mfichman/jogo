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

using namespace std;

Class::Class(Location loc, Environment* env, Type* type, Type* mixins, 
    String* comment, Feature* feature) :

    Feature(loc, env, type->name()),
    type_(type),
    mixins_(mixins),
    comment_(comment),
    features_(feature),
    is_object_(false),
    is_value_(false),
    is_interface_(false),
    is_mixin_(false),
    size_(0) {

    for (Feature* feat = features_; feat; feat = feat->next()) {
        feat->parent(this);
        if (Attribute* attr = dynamic_cast<Attribute*>(feat)) {
            attributes_[attr->name()] = attr;
        } else if (Function* func = static_cast<Function*>(feat)) {
            functions_[func->name()] = func;
        } else if (Constant* cons = static_cast<Constant*>(feat)) {
            constants_[cons->name()] = cons;
        }   
    }

    for (Type* mixin = mixins; mixin; mixin = mixin->next()) {
        if ("Object" == mixin->name()->string()) {
            is_object_ = true;
            break;
        }
        if ("Interface" == mixin->name()->string()) {
            is_interface_ = true;
            break;
        }
        if ("Value" == mixin->name()->string()) {
            is_value_ = true;
            break;
        }
        if ("Mixin" == mixin->name()->string()) {
            is_mixin_ = true;
            break;
        }
    }
}

/* Constructor for union types */
Class::Class(Location loc, Environment* env, Type* type, Type* alt) :
    Feature(loc, env, type->name()),
    type_(type),
    alternates_(alt),
    is_object_(true),
    is_value_(false),
    is_interface_(false),
    is_mixin_(false), 
    size_(0) {
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

    if (Attribute* attr = dynamic_cast<Attribute*>(feature)) {
        attributes_[attr->name()] = attr;
        return;
    }
    if (Function* func = dynamic_cast<Function*>(feature)) {
        functions_[func->name()] = func;
        return;
    }
    if (Constant* cons = dynamic_cast<Constant*>(feature)) {
        constants_[cons->name()] = cons;
        return;
    } 
}

Attribute* Class::attribute(String* name) const {
    std::map<String::Ptr, Attribute::Ptr>::const_iterator i;
    i = attributes_.find(name);
    return (i == attributes_.end()) ? 0 : i->second;
}

Constant* Class::constant(String* name) const {
    std::map<String::Ptr, Constant::Ptr>::const_iterator i;
    i = constants_.find(name);
    return (i == constants_.end()) ? 0 : i->second;
}

Function* Class::function(String* name) const {
    // Searches for a function with name 'name' in this lass or in a mixin
    // that was added to this class.
    std::map<String::Ptr, Function::Ptr>::const_iterator i;
    i = functions_.find(name);
    if (i != functions_.end()) {
        return i->second;
    }
    if (type()->is_proto()) {
        return 0;
    }

    for (Type* mixin = mixins_; mixin; mixin = mixin->next()) {
        Class* clazz = mixin->clazz();
        if (clazz) {
            Function* func = clazz->function(name);
            if (func) {
                return func;
            } 
        }
    }
    return 0;
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

    // If 'this' is a union type, then it will be a subtype of 'other' if
    // 'other' is listed as an alternate.  Likewise, 'this' will be a subtype
    // of 'other' if 'other' is an alternate and 'this' is listed as an
    // alternate of 'other'.
    if (alternates()) {
        for (Type::Ptr alt = alternates(); alt; alt = alt->next()) {
            if (alt->clazz() == other) {
                return true;
            }  
        }
        return false;
    } else if (other->alternates()) {
        for (Type::Ptr alt = other->alternates(); alt; alt = alt->next()) {
            if (alt->clazz() == this) {
                return true;
            }
        }
        return false;
    } else if (!other->is_interface()) {
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

    feature->parent(this);
    features_ = append(features_, feature);

    if (Class* clazz = dynamic_cast<Class*>(feature)) {
        classes_[clazz->name()] = clazz;
        return;
    }
    if (Function* func = dynamic_cast<Function*>(feature)) {
        functions_[func->name()] = func;
        return;
    }
    if (Constant* cons = dynamic_cast<Constant*>(feature)) {
        constants_[cons->name()] = cons;
    }
}

String* Feature::label() const {
    // Builds the label for a feature if it hasn't already been calculated.
    if (label_) { return label_; }

    std::string parent = parent_? parent_->label()->string() : "";
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

std::string Import::file_name(const std::string& scope) {
    // Converts a module name to the name of the file that contains the 
    // module.
    std::string out;
    for (size_t i = 1; i <= scope.length(); i++) {    
        if (scope[i-1] == ':' && scope[i] == ':') {
            out += FILE_SEPARATOR;
            i++;
        } else {
            out += scope[i-1];
        }
    }

    return out;
}

std::string Import::scope_name(const std::string& file) {
    // Given the file name, returns the module's scope name.  This is equal
    // to the directory name with the '/' replaced by '::'.  For example,
    // 'Foo/Bar.ap' would have scope name 'Foo'.

    size_t pos = file.find_last_of(FILE_SEPARATOR);
    if (pos == string::npos) {
        return "";
    }
    std::string dir = file.substr(0, pos);
    std::string name;
    for (size_t i = 0; i < dir.size(); i++) {
        if (!isalnum(dir[i])) {
            if (dir[i] == FILE_SEPARATOR) {
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
