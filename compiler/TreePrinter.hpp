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

#include "Jogo.hpp"
#include "TreeNode.hpp"
#include "Environment.hpp"
#include "Stream.hpp"

class TreePrinter : public TreeNode::Functor {
public:
    TreePrinter(Environment* environment, Stream* out=Stream::stout());
    typedef Pointer<TreePrinter> Ptr;

private:
    void operator()(Module* feature);
    void operator()(Class* feature);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Construct* expression);
    void operator()(IdentifierRef* expression);
    void operator()(ConstantRef* expression);
    void operator()(Empty* empty);
    void operator()(Let* statement);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(Match* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Constant* constant);
    void operator()(Import* feature);
    void operator()(Type* type);
    void operator()(HashLiteral* literal);
    void operator()(ArrayLiteral* literal);
    void operator()(Member* expression);
    void operator()(ParseError* expression);
    void operator()(Is* expression);

    void print_tabs();
    void print_comment(String* comment);

    Environment::Ptr env_;
    Stream::Ptr out_;
    int indent_level_;
};

void print(TreeNode* node);
