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

#ifndef NODE_H
#define NODE_H

#include "apollo.hpp"
#include "object.hpp"
#include "location.hpp"

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

class TreeNode::Functor : public Object{
public:
    virtual ~Functor() {} 
    virtual void operator()(Literal* expr) {}
    virtual void operator()(Binary* expr) {}
    virtual void operator()(Unary* expr) {}
    virtual void operator()(Call* expr) {}
    virtual void operator()(Dispatch* expr) {}
    virtual void operator()(Index* expr) {}
    virtual void operator()(Identifier* expr) {}
    virtual void operator()(Member* expr) {}
    virtual void operator()(Block* stmt) {}
    virtual void operator()(Simple* stmt) {}
    virtual void operator()(While* stmt) {}
    virtual void operator()(For* stmt) {}
    virtual void operator()(Conditional* stmt) {}
    virtual void operator()(Variable* stmt) {}
    virtual void operator()(Return* stmt) {}
    virtual void operator()(When* stmt) {}
    virtual void operator()(Case* stmt) {}
    virtual void operator()(Function* feat) {}
    virtual void operator()(Define* feat) {}
    virtual void operator()(Attribute* feat) {}
    virtual void operator()(Import* feat) {}
};


#endif
