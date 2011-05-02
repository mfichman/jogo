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

#ifndef TYPE_HPP
#define TYPE_HPP

#include "Apollo.hpp"
#include "Object.hpp"
#include "Name.hpp"
#include <vector>
#include <map>
#include <iostream>

/* Type object */
class Type : public Object {
public:
    Type(Type* enclosing_type, Name* name, Generic* gen, Environment* env);
    Type* enclosing_type() const { return enclosing_type_; }
    Name* name() const { return name_; }
    Generic* generics() const { return generics_; }
    Name* qualified_name() const { return qualified_name_; }
    bool equals(Type* other);
    bool supertype(Type* other);
    bool subtype(Type* other);
    bool collection(Type* other);
    Type* least_upper_bound(Type* other);
    
    typedef Pointer<Type> Ptr;
    
private:
    Pointer<Environment> environment_;
    Type::Ptr enclosing_type_;
    Name::Ptr name_;
    Pointer<Generic> generics_;
    Name::Ptr qualified_name_;
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

std::ostream& operator<<(std::ostream&out, const Type& type);


#endif
