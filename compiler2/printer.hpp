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

#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "apollo.hpp"
#include "tree_node.hpp"
#include "environment.hpp"

class Printer : public TreeNode::Functor {
public:
    Printer(Environment* environment);
    typedef Pointer<Printer> Ptr;

private:
    void operator()(Class* unit);
    void operator()(Interface* unit);
    void operator()(Structure* unit);
    void operator()(Module* unit);
    void operator()(StringLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Assignment* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Dispatch* expression);
    void operator()(Index* expression);
    void operator()(Identifier* expression);
    void operator()(Member* expression);
    void operator()(Block* statment);
    void operator()(Simple* statment);
    void operator()(While* statment);
    void operator()(For* statment);
    void operator()(Conditional* statment);
    void operator()(Variable* statment);
    void operator()(Return* statment);
    void operator()(When* statment);
    void operator()(Case* statment);
    void operator()(Function* feature);
    void operator()(Define* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);

    void print_tabs();

    Environment::Ptr environment_;
    int indent_level_;
};

#endif
