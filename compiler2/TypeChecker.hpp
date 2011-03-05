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

#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Apollo.hpp"
#include "TreeNode.hpp"
#include "Environment.hpp"
#include "Feature.hpp"
#include <vector>
#include <map>

class TypeChecker : public TreeNode::Functor {
public:
	TypeChecker(Environment* environment);
    typedef Pointer<TypeChecker> Ptr;

private:
    void operator()(Class* unit);
    void operator()(Interface* unit);
    void operator()(Structure* unit);
    void operator()(Module* unit);
    void operator()(StringLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Assignment* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Dispatch* expression);
    void operator()(Identifier* expression);
    void operator()(Member* expression);
    void operator()(Empty* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(Let* let);
    void operator()(While* statement);
    void operator()(For* statement);
    void operator()(Conditional* statement);
    void operator()(Variable* statement);
    void operator()(Return* statement);
    void operator()(When* statement);
    void operator()(Case* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);

    Type* variable(Name* name);
    void variable(Name* name, Type* type);
    Function* function(Name* name);
    void function(Name* name, Function* function);
    void enter_scope();
    void exit_scope();

    Environment::Ptr environment_;
    std::vector<std::map<Name::Ptr, Type::Ptr> > variable_;
    std::map<Name::Ptr, Function::Ptr> function_;
    Unit::Ptr current_unit_; 
    Function::Ptr current_function_;
};

#endif
