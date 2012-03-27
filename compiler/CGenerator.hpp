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
#include "Environment.hpp"
#include "Object.hpp"
#include "Scope.hpp"

/* C code generator */
class CGenerator : public TreeNode::Functor {
public:
    CGenerator(Environment* env);
    typedef Pointer<CGenerator> Ptr;

    Stream* out() const { return out_; }
    void out(Stream* out) { out_ = out; }
    void operator()(File* file);

private:
    void operator()(Constant* feature);
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Is* expression);
    void operator()(Member* expression);
    void operator()(Construct* expression);
    void operator()(Identifier* expression);
    void operator()(ConstantIdentifier* expression);
    void operator()(Empty* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(Let* let);
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
    void operator()(Import* feature);
    void operator()(Closure* expression);
    void operator()(Type* type);

private:
    void dispatch_table(Class* feature);
    void function_signature(Function* feature); 
    void enter_scope();
    void exit_scope();
    void ctor_preamble(Class* feature);
    void scope_cleanup(Variable* variable);
    void func_return();
    void dtor_epilog(Function* function);
    void refcount_inc(Operand var);
    void refcount_dec(Operand var);

    Environment::Ptr env_;
    File::Ptr file_;
    Stream::Ptr out_;
    Class::Ptr class_;
    Function::Ptr function_;
    std::vector<Scope::Ptr> scope_;
};
