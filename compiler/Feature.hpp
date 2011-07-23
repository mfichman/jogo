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
#include "Statement.hpp"
#include "Expression.hpp"
#include "Formal.hpp"
#include "Type.hpp"
#include "BasicBlock.hpp"


/* This file holds interfaces for class features */
class Feature : public TreeNode {
public:
    typedef int Flags;

    Feature(Location loc, Flags flags = 0) :
        TreeNode(loc),
        flags_(flags) {
    }

    Feature* next() const { return next_; }
    Flags flags() const { return flags_; }
    String* label() const { return label_; }
    bool is_private() const { return flags_ & PRIVATE; }
    bool is_native() const { return flags_& NATIVE; }
    void next(Feature* next) { next_ = next; }
    void flags(Flags flags) { flags_ = flags; }
    void label(String* label) { label_ = label; }
    typedef Pointer<Feature> Ptr;

    static const int PRIVATE = 0x1;
    static const int NATIVE = 0x2;

private:
    Feature::Ptr next_;
    Flags flags_;
    String::Ptr label_;
};

/* Class for instance variables (attributes) of a class or module */
class Attribute : public Feature {
public:
    Attribute(Location loc, String* name, Type* type, Expression* init) :
        Feature(loc),
        name_(name),
		type_(type),
        initializer_(init) {
    }

    String* name() const { return name_; }
	Type* type() const { return type_; }
    Expression* initializer() const { return initializer_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Attribute> Ptr;

private:
	String::Ptr name_;
    Type::Ptr type_;
    Expression::Ptr initializer_;
};

/* Class for functions belonging to a class or module */
class Function : public Feature {
public:
    Function(Location l, String* nm, Formal* fm, Flags f, Type* r, Block* b) :
        Feature(l, f),
        name_(nm),
		formals_(fm),
		type_(r),
        block_(b) {
    
        basic_block();
    }

    String* name() const { return name_; }
	Formal* formals() const { return formals_; }
	Type* type() const { return type_; }
    Block* block() const { return block_; }
    BasicBlock* code() const { return basic_block_[0]; }
    BasicBlock* basic_block();
    BasicBlock* basic_block(int index) { return basic_block_[index]; }
    int basic_blocks() { return basic_block_.size(); }
    bool covariant(Function* other) const;
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Function> Ptr;

private:
    String::Ptr name_;
	Formal::Ptr formals_;
	Type::Ptr type_;
    Block::Ptr block_;
    std::vector<BasicBlock::Ptr> basic_block_;
};

/* Class for imports */
class Import : public Feature {
public:
    Import(Location loc, String* scope, bool is_qualified) :
        Feature(loc),
        file_name_(file_name(scope->string())),
        scope_(scope),
        is_qualified_(is_qualified) {
    }
        
    const std::string& file_name() const { return file_name_; }
    String* scope() const { return scope_; }
    bool is_qualified() const { return is_qualified_; }
    static std::string file_name(const std::string& scope);
    static std::string scope_name(const std::string& file);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Import> Ptr;

private:
    std::string file_name_;
    String::Ptr scope_;
    bool is_qualified_;
};

/* Represents a class object */
class Class : public Feature {
public:
    Class(Location loc, Type* type, Type* mixins, String* cmt, Feature* feat);
    Feature* features() const { return features_; }    
    Attribute* attribute(String* name) const;
    Function* function(String* name) const;
    String* comment() const { return comment_; }
    Type* type() const { return type_; }
    Type* mixins() const { return mixins_; }
    String* name() const { return type_->name(); }
    bool is_object() const { return is_object_; }
    bool is_value() const { return is_value_; }
    bool is_interface() const { return is_interface_; }
    bool subtype(Class* other) const;
    void feature(Feature* feature);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Class> Ptr;

private:
    std::map<String::Ptr, Attribute::Ptr> attributes_;
    std::map<String::Ptr, Function::Ptr> functions_;
    mutable std::map<Class*, bool> subtype_;
    Type::Ptr type_;
    Type::Ptr mixins_;
    String::Ptr comment_;
    Feature::Ptr features_;
    bool is_object_;
    bool is_value_;
    bool is_interface_;
    mutable int visited_;
};

/* Module, contains classes, functions, etc. */
class Module : public Feature {
public:
    Module(Location loc, String* name, Environment* env) :
        Feature(loc),
        name_(name),
        environment_(env) {
    }

    Feature* features() const { return features_; }
    Function* function(String* name) { return query(functions_, name); }
    Class* clazz(String* name) { return query(classes_, name); }
    String* name() const { return name_; }
    void feature(Feature* feature);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Module> Ptr; 

private:
    std::map<String::Ptr, Function::Ptr> functions_;
    std::map<String::Ptr, Class::Ptr> classes_;
    String::Ptr name_; 
    Feature::Ptr features_;
    Environment* environment_;
};

