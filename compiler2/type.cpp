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

#include "type.hpp"
#include "environment.hpp"
#include <cassert>

Type::Type(Type* enclosing_type, Name* name, Generic* gen, Environment* env) :
    environment_(env),
    enclosing_type_(enclosing_type),
    name_(name),
    generics_(gen) {

    std::string qualified_name = name->string();
    for (Type* tp = enclosing_type; tp; tp = tp->enclosing_type()) {
        qualified_name = tp->name()->string() + "::" + qualified_name;
    }
    qualified_name_ = env->name(qualified_name);
}

bool Type::equals(Type* other) {
    /* Make sure the base name is equal */
    if (name() != other->name()) {
        return false;
    }

    /* Make sure the enclosing types are all equal */
    Type* t1 = enclosing_type();
    Type* t2 = other->enclosing_type();
    while (t1 && t2) {
        if (t1->name() != t2->name()) {
            return false;
        }         

        t1 = t1->enclosing_type();
        t2 = t2->enclosing_type();
    }
    if (t1 != 0 && t2 != 0) {
        return false;
    }

    /* Make sure the generic parameters are the same */
    Generic* g1 = generics();
    Generic* g2 = other->generics();
    while (g1 && g2) {
        if (g1->type()->equals(g2->type())) {
            return false;
        }
        
        g1 = g1->next();
        g2 = g2->next();
    }
    return true;
}

bool Type::subtype(Type* other) {
    assert("Not implemented");
    return false;
}

bool Type::supertype(Type* other) {
    assert("Not implemented");
    return false;
}

bool Type::collection(Type* other) {
    assert("Not implemented");
    return false;
}

Type* Type::least_upper_bound(Type* other) {
    assert("Not implemented");
    return false;
}








