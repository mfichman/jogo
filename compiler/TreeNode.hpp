/*
 * Copyright (c) 2010 Matt Fichman
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

#pragma once

#include "Apollo.hpp"
#include "Object.hpp"
#include "Location.hpp"

/* Class for abstract syntax tree nodes */
class TreeNode : public Object {
public:
    TreeNode(Location loc) : 
        location_(loc) {
    }

    const Location& location() const { return location_; }

    class Functor;
    virtual void operator()(Functor *functor)=0;

private:
    Location location_;
};

class TreeNode::Functor : public Object {
public:
    virtual ~Functor() {} 
    virtual void operator()(Class* feature) {}
    virtual void operator()(Module* feature) {}
    virtual void operator()(Formal* formal) {}
    virtual void operator()(StringLiteral* expression) {}
    virtual void operator()(IntegerLiteral* expression) {}
    virtual void operator()(FloatLiteral* expression) {}
    virtual void operator()(BooleanLiteral* expression) {}
    virtual void operator()(Binary* expression) {}
    virtual void operator()(Unary* expression) {}
    virtual void operator()(Call* expression) {}
    virtual void operator()(Dispatch* expression) {}
    virtual void operator()(Construct* expression) {}
    virtual void operator()(Identifier* expression) {}
    virtual void operator()(Empty* empty) {}
    virtual void operator()(Block* statement) {}
    virtual void operator()(Simple* statement) {}
    virtual void operator()(While* statement) {}
    virtual void operator()(Let* statement) {}
    virtual void operator()(Conditional* statement) {}
    virtual void operator()(Variable* statement) {}
    virtual void operator()(Return* statement) {}
    virtual void operator()(When* statement) {}
    virtual void operator()(Yield* statement) {}
    virtual void operator()(Fork* statement) {}
    virtual void operator()(Case* statement) {}
    virtual void operator()(Function* feature) {}
    virtual void operator()(Attribute* feature) {}
    virtual void operator()(Import* feature) {}
    virtual void operator()(Type* type) {}
};


