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
#include "TreeNode.hpp"
#include "Environment.hpp"
#include "Feature.hpp"

/* 
 * This class recursively iterates through a closure to find unbound vars.
 * Unbound vars are variables reference within the closure that are not bound
 * to a variable in the scope when they are referenced.
 */
class ClosureAnalyzer : public TreeNode::Functor {
public:
    ClosureAnalyzer(Environment* env);
    typedef Pointer<ClosureAnalyzer> Ptr;

    int unbound_vars() const { return unbound_var_.size(); } 
    String::Ptr unbound_var(int i) const { return unbound_var_[i]; }

    void operator()(Function* func);

private:
    void operator()(HashLiteral* expression);
    void operator()(ArrayLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(Let* let);
	void operator()(Member* member);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(Match* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);

    void variable(String* var, bool set);
    bool variable(String* var);
    void enter_scope();
    void exit_scope();

    Environment::Ptr env_;
    std::vector<String::Ptr> unbound_var_;
    std::set<String::Ptr> unbound_var_set_;
    std::vector<std::set<String::Ptr> > variable_;
};
