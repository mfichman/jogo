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

#ifndef UNIT_H
#define UNIT_H

#include "TreeNode.h"
#include "Feature.h"
#include <typeinfo>
#include <map>

/* This file includes interfaces for compilation units */
class Unit : public TreeNode {
public:
    Unit(Location loc, Name* name, Feature* features) :
        TreeNode(loc),
        name_(name),
        features_(features) {

        for (Feature* feat = features; feat; feat = feat->next()) {
            if (typeid(feat) == typeid(Attribute*)) {
                attributes_[feat->name()] = static_cast<Attribute*>(feat);
                break;
            }
            if (typeid(feat) == typeid(Function*)) {
                functions_[feat->name()] = static_cast<Function*>(feat);
                break;
            }
        }
    }
    
    Feature* features() const { return features_; };    
    Attribute* attribute(Name* name) { return attributes_[name]; }
    Function* function(Name* name) { return functions_[name]; }
    Name* name() const { return name_; }
    Unit* next() const { return next_; }
    void next(Unit* unit) { next_ = unit; }
    typedef Pointer<Unit> Ptr;

private:
    std::map<Name::Ptr, Attribute::Ptr> attributes_;
    std::map<Name::Ptr, Function::Ptr> functions_;
    Name::Ptr name_;
    Feature::Ptr features_;
    Unit::Ptr next_;
};

/* Represents a class object */
class Class : public Unit {
public:
    Class(Location loc, Name* name, Feature* features) :
        Unit(loc, name, features) {
    }
};

/* Represents a class object */
class Interface : public Unit {
public:
    Interface(Location loc, Name* name, Feature* features) :
        Unit(loc, name, features) {
    }
};


class Structure : public Unit {
public:
    Structure(Location loc, Name* name, Feature* features) :
        Unit(loc, name, features) {
    }
};

class Module : public Unit {
public:
    Module(Location loc, Name* name, Feature* features) :
        Unit(loc, name, features) {
    }
};


#endif
