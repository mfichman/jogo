/*
 * Copyright (c) 2013 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "TreeNode.hpp"
#include "Environment.hpp"

/* Records a usage of a name */
class Usage {
public:
    Usage(TreeNode::Ptr node) : node_(node) {}
    TreeNode::Ptr node() const { return node_; }
    Location const& location() const { return location_; }
private:
    TreeNode::Ptr node_;
    Location location_;
};

/* Records all locations that use a qualified name. */
class UsageFinder : public TreeNode::Functor {
public: 
    UsageFinder(Environment* env, String* qn); 
    Usage const& usage(int index) const { return usage_[index]; }
    int usages() const { return usage_.size(); }
    void operator()(File* file);
    void operator()(Module* feature);
    void operator()(Class* feature);
    void operator()(Formal* formal);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(ConstantIdentifier* expression);
    void operator()(Let* statement);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(Match* statement);
    void operator()(Case* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Constant* constant);
    void operator()(Import* feature);
    void operator()(HashLiteral* literal);
    void operator()(ArrayLiteral* literal);
    void operator()(Member* expression);
    void operator()(Is* expression);
    typedef Pointer<UsageFinder> Ptr;

private:
    std::vector<Usage> usage_;
    TreeNode::Ptr search_;
};


