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

#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

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
    virtual void operator()(Functor *functor) {}

private:
    Location location_;
};

class TreeNode::Functor : public Object {
public:
    virtual ~Functor() {} 
    virtual void operator()(Class* unit) {}
    virtual void operator()(Interface* unit) {}
    virtual void operator()(Structure* unit) {}
    virtual void operator()(Module* unit) {}
    virtual void operator()(StringLiteral* expression) {}
    virtual void operator()(IntegerLiteral* expression) {}
    virtual void operator()(BooleanLiteral* expression) {}
    virtual void operator()(Binary* expression) {}
    virtual void operator()(Assignment* expression) {}
    virtual void operator()(Unary* expression) {}
    virtual void operator()(Call* expression) {}
    virtual void operator()(Dispatch* expression) {}
    virtual void operator()(Identifier* expression) {}
    virtual void operator()(Member* expression) {}
    virtual void operator()(Empty* empty) {}
    virtual void operator()(Block* statment) {}
    virtual void operator()(Simple* statment) {}
    virtual void operator()(While* statment) {}
    virtual void operator()(For* statment) {}
    virtual void operator()(Conditional* statment) {}
    virtual void operator()(Variable* statment) {}
    virtual void operator()(Return* statment) {}
    virtual void operator()(When* statment) {}
    virtual void operator()(Case* statment) {}
    virtual void operator()(Function* feature) {}
    virtual void operator()(Define* feature) {}
    virtual void operator()(Attribute* feature) {}
    virtual void operator()(Import* feature) {}
};


#endif
