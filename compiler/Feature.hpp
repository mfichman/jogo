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
#include "TreeNode.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Formal.hpp"
#include "Type.hpp"
#include "BasicBlock.hpp"


/* This file holds interfaces for class features */
class Feature : public TreeNode {
public:
    Feature(Location loc) :
        TreeNode(loc) {
    }

    Feature* next() const { return next_; }
    void next(Feature* next) { next_ = next; }
    typedef Pointer<Feature> Ptr;

private:
    Feature::Ptr next_;
};

/* Class for instance variables (attributes) of a class or module */
class Attribute : public Feature {
public:
    Attribute(Location loc, Name* name, Type* type, Expression* init) :
        Feature(loc),
        name_(name),
		type_(type),
        initializer_(init) {
    }

    Name* name() const { return name_; }
	Type* type() const { return type_; }
    Expression* initializer() const { return initializer_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Attribute> Ptr;

private:
	Name::Ptr name_;
    Type::Ptr type_;
    Expression::Ptr initializer_;
};

/* Class for functions belonging to a class or module */
class Function : public Feature {
public:
    Function(Location loc, Name* nm, Formal* fm, Type* ret, Statement* blk) :
        Feature(loc),
        name_(nm),
		formals_(fm),
		type_(ret),
        block_(blk),
        code_(new BasicBlock) {
    }

    Name* name() const { return name_; }
	Formal* formals() const { return formals_; }
	Type* type() const { return type_; }
    Statement* block() const { return block_; }
    BasicBlock* code() const { return code_; }
    bool covariant(Function* other) const;
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Function> Ptr;

private:
    Name::Ptr name_;
	Formal::Ptr formals_;
	Type::Ptr type_;
    Statement::Ptr block_;
    BasicBlock::Ptr code_;
};

/* Class for imports */
class Import : public Feature {
public:
    Import(Location loc, Name* scope) :
        Feature(loc),
        file_name_(file_name(scope)),
        scope_(scope) {
    }
        
    const std::string& file_name() const { return file_name_; }
    Name* scope() const { return scope_; }
    static std::string file_name(Name* name);
    static std::string base_name(const std::string& file);
    static std::string scope_name(const std::string& file);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Import> Ptr;

private:
    std::string file_name_;
    Name::Ptr scope_;
};

/* Represents a class object */
class Class : public Feature {
public:
    Class(Location loc, Type* type, Type* mixins, Feature* features);
    Feature* features() const { return features_; }    
    Attribute* attribute(Name* name) const;
    Function* function(Name* name) const;
    Type* type() const { return type_; }
    Type* mixins() const { return mixins_; }
    Name* name() const { return type_->name(); }
    bool is_object() const { return is_object_; }
    bool is_value() const { return is_value_; }
    bool is_interface() const { return is_interface_; }
    bool subtype(Class* other) const;
    void feature(Feature* feature);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Class> Ptr;

private:
    std::map<Name::Ptr, Attribute::Ptr> attributes_;
    std::map<Name::Ptr, Function::Ptr> functions_;
    mutable std::map<Class*, bool> subtype_;
    Type::Ptr type_;
    Type::Ptr mixins_;
    Feature::Ptr features_;
    bool is_object_;
    bool is_value_;
    bool is_interface_;
    mutable int visited_;
};

/* Module, contains classes, functions, imports, etc. */
class Module : public Feature {
public:
    Module(Location loc, Name* name, Environment* env) :
        Feature(loc),
        name_(name),
        environment_(env) {
    }

    Feature* features() const { return features_; }
    Function* function(Name* name) { return functions_[name]; }
    Function* function(Name* scope, Name* name);
    Class* clazz(Name* name) { return classes_[name]; }
    Class* clazz(Name* scope, Name* name);
    Module* module(Name* scope);
    Import* import(Name* name) { return imports_[name]; }
    Name* name() const { return name_; }
    void feature(Feature* feature);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Module> Ptr; 

private:
    std::map<Name::Ptr, Function::Ptr> functions_;
    std::map<Name::Ptr, Class::Ptr> classes_;
    std::map<Name::Ptr, Import::Ptr> imports_;
    Name::Ptr name_; 
    Feature::Ptr features_;
    Environment* environment_;
};

