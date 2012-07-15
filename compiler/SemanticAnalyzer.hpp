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
#include "Scope.hpp"
#include <vector>
#include <map>

class SemanticAnalyzer : public TreeNode::Functor {
public:
	SemanticAnalyzer(Environment* environment);
    typedef Pointer<SemanticAnalyzer> Ptr;

private:
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(HashLiteral* expression);
    void operator()(ArrayLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Construct* expression);
    void operator()(Member* expression);
    void operator()(Identifier* expression);
    void operator()(ConstantIdentifier* expression);
    void operator()(Empty* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Is* expression);
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
    void operator()(Constant* feature);
    void operator()(Import* feature);
    void operator()(Type* type);
    void operator()(Closure* expression);

    Scope* scope() const { return scope_.back(); }
    Variable* variable(String* name);
    void variable(Variable* var);
    void enter_scope();
    void exit_scope();
    Expression::Ptr args(Expression* expr, Function* func, Type* receiver);
    void initial_assignment(Assignment* expr);
    void secondary_assignment(Assignment* expr); 
    void mutator(Attribute* feature);
    void accessor(Attribute* feature);
    void constructor();
    void destructor();
    Type* fix_generics(Type* parent, Type* type);
    void assign_enums(Class* clazz);

    Environment::Ptr env_;
    Stream::Ptr err_;
    Class::Ptr class_;
    Function::Ptr function_;
    Type::Ptr return_; // Return value of the current block
    std::vector<Scope::Ptr> scope_;

    friend class ContextAnchor;
};

/* This class saves the state of the semantic analyzer */
class ContextAnchor {
public:
    ContextAnchor(SemanticAnalyzer* semant) : semant_(semant){
        scope_.swap(semant->scope_);
        class_ = semant->class_;
        return_ = semant->return_;
        function_ = semant->function_;
        semant->enter_scope();
    }

    ~ContextAnchor() {
        semant_->exit_scope();
        semant_->function_ = function_;
        semant_->return_ = return_;
        semant_->class_ = class_;
        scope_.swap(semant_->scope_);
    }

private:
    SemanticAnalyzer* semant_;
    std::vector<Scope::Ptr> scope_;
    Class::Ptr class_;
    Type::Ptr return_;
    Function::Ptr function_;
    File::Ptr file_;
};
