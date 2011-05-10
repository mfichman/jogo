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

#pragma once

#include "Apollo.hpp"
#include "TreeNode.hpp"
#include "Name.hpp"
#include <vector>
#include <map>
#include <iostream>

/* Type object */
class Type : public TreeNode {
public:
    Type(Location loc, Name* qn, Generic* gen, Module* mod, Environment* env);
    Generic* generics() const { return generics_; }
    Name* name() const { return name_; }
    Name* scope() const { return scope_; }
    Type* next() const { return next_; }
    Class* clazz() const;
    Module* module() const { return module_; }
    bool equals(Type* other) const;
    bool subtype(Type* other) const;
    bool interface() const;
    bool object() const;
    bool value() const;
    void next(Type* next) { next_ = next; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Type> Ptr;

    
private:
    Pointer<Generic> generics_;
    Module* module_;
    Environment* environment_;
    Name::Ptr scope_;
    Name::Ptr name_;
    Type::Ptr next_;
    Class* class_;
};

/* Holder for a generics type parameter */
class Generic : public Object {
public: 
    Generic(Type* type) :
        type_(type) {
    }

    Type* type() const { return type_; }
    Generic* next() const { return next_; }
    void next(Generic* next) { next_ = next; }
    typedef Pointer<Generic> Ptr;

private:
    Type::Ptr type_;
    Generic::Ptr next_;
};

std::ostream& operator<<(std::ostream&out, const Type* type);


