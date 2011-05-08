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

#include <typeinfo>

Class::Class(Location loc, Type* type, Feature* features) :
    Feature(loc),
    type_(type),
    features_(features) {

    for (Feature* feat = features; feat; feat = feat->next()) {
        if (typeid(feat) == typeid(Attribute*)) {
            Attribute* attr = static_cast<Attribute*>(feat);
            attributes_[attr->name()] = attr;
            continue;
        }
        if (typeid(feat) == typeid(Function*)) {
            Function* func = static_cast<Function*>(feat);
            functions_[func->name()] = func;
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

    if (typeid(feature) == typeid(Attribute*)) {
        Attribute* attr = static_cast<Attribute*>(feature);
        attributes_[attr->name()] = attr;
        return;
    }
    if (typeid(feature) == typeid(Function*)) {
        Function* func = static_cast<Function*>(feature);
        functions_[func->name()] = func;
        return;
    }
}

void Module::feature(Feature* feature) {
    if (!feature) {
        return;
    }

    feature->next(features_);
    features_ = feature;

    if (typeid(feature) == typeid(Class*)) {
        Class* clazz = static_cast<Class*>(feature);
        classes_[clazz->name()] = clazz;
        return;
    }
    if (typeid(feature) == typeid(Function*)) {
        Function* func = static_cast<Function*>(feature);
        functions_[func->name()] = func;
        return;
    }
    if (typeid(feature) == typeid(Module*)) {
        Module* module = static_cast<Module*>(feature);
        modules_[module->name()] = module;
        return;
    }
}

std::string Import::basename(const std::string& file) {

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
