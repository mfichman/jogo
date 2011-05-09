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
    //virtual TreeNode* clone() const=0;

private:
    Location location_;
};

class TreeNode::Functor : public Object {
public:
    virtual ~Functor() {} 
    virtual void operator()(Class* feature)=0;
    virtual void operator()(Module* feature)=0;
    virtual void operator()(Formal* formal)=0;
    virtual void operator()(StringLiteral* expression)=0;
    virtual void operator()(IntegerLiteral* expression)=0;
    virtual void operator()(BooleanLiteral* expression)=0;
    virtual void operator()(Binary* expression)=0;
    virtual void operator()(Assignment* expression)=0;
    virtual void operator()(Unary* expression)=0;
    virtual void operator()(Call* expression)=0;
    virtual void operator()(Dispatch* expression)=0;
    virtual void operator()(Construct* expression)=0;
    virtual void operator()(Identifier* expression)=0;
    virtual void operator()(Member* expression)=0;
    virtual void operator()(Empty* empty)=0;
    virtual void operator()(Block* statement)=0;
    virtual void operator()(Simple* statement)=0;
    virtual void operator()(While* statement)=0;
    virtual void operator()(For* statement)=0;
    virtual void operator()(Let* statement)=0;
    virtual void operator()(Conditional* statement)=0;
    virtual void operator()(Variable* statement)=0;
    virtual void operator()(Return* statement)=0;
    virtual void operator()(When* statement)=0;
    virtual void operator()(Yield* statement)=0;
    virtual void operator()(Fork* statement)=0;
    virtual void operator()(Case* statement)=0;
    virtual void operator()(Function* feature)=0;
    virtual void operator()(Attribute* feature)=0;
    virtual void operator()(Import* feature)=0;
    virtual void operator()(Type* type)=0;
};


