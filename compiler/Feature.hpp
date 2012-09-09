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
#include "Import.hpp"
#include <vector>


/* This file holds interfaces for class features */
class Feature : public TreeNode {
public:
    typedef int Flags;

    Feature(Location loc, Environment* env, String* name, Flags flags = 0) :
        TreeNode(loc),
        name_(name),
        env_(env),
        flags_(flags),
        parent_(0),
        next_slot_(2) { // 2: 1 for dispatch table, 1 for refcount
    }

    virtual Feature* feature(String* name) const { return 0; }
    Class* clazz(String* name) const;
    Attribute* attribute(String* name) const;
    Function* function(String* name) const;
    Constant* constant(String* name) const;
    String* name() const { return name_; }
    Feature* next() const { return next_; }
    Feature* last() const { return last_; }
    Flags flags() const { return flags_; }
    String* label() const;
    String* qualified_name() const;
    Feature* parent() const { return parent_; }
    Environment* env() const { return env_; }
    bool is_private() const { return flags_ & PRIVATE; }
    bool is_native() const { return flags_ & NATIVE; }
    bool is_weak() const { return flags_ & WEAK; }
    bool is_immutable() const { return flags_ & IMMUTABLE; }
    bool is_closure() const { return flags_ & CLOSURE; }
    bool is_embedded() const { return flags_ & EMBEDDED; }
    void next(Feature* next) { next_ = next; }
    void last(Feature* last) { last_ = last; }
    void flags(Flags flags) { flags_ = flags; }
    void parent(Feature* parent) { parent_ = parent; }
    int next_slot() { return next_slot_++; }
    typedef Pointer<Feature> Ptr;

    static const int PRIVATE = 0x1;
    static const int NATIVE = 0x2;
    static const int WEAK = 0x4;
    static const int IMMUTABLE = 0x8;
    static const int CLOSURE = 0x20;
    static const int EMBEDDED = 0x40; 

private:
    String::Ptr name_;
    Feature::Ptr next_;
    Feature::Ptr last_;
    Environment* env_;
    Flags flags_;
    mutable String::Ptr label_;
    Feature* parent_;
    int next_slot_;
};

/* Represents a Constant initializer */
class Constant : public Feature {
public:
    Constant(Location loc, Environment* env, String* name, Flags flags, 
        Expression* init);

    Type* type() const { return type_; }
    Expression* initializer() const { return initializer_; }
    void type(Type* type) { type_ = type; assert(type_); } 
    void initializer(Expression* init) { initializer_ = init; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Constant> Ptr;

private:
    Type::Ptr type_;
    Expression::Ptr initializer_;
};

/* Class for instance variables (attributes) of a class or module */
class Attribute : public Feature {
public:
    Attribute(Location loc, Environment* env, String* name, Flags flags, 
        Type* type, Expression* init) :
        Feature(loc, env, name, flags),
		declared_type_(type),
        initializer_(init),
        slot_(0) {
    
        assert(declared_type_);
    }

    Type* declared_type() const { return declared_type_; }
	Type* type() const { return type_; }
    Expression* initializer() const { return initializer_; }
    int slot() const { return slot_; }
    void type(Type* type) { type_ = type; assert(type_); }
    void slot(int slot) { slot_ = slot; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Attribute> Ptr;

private:
    Type::Ptr declared_type_;
    Type::Ptr type_;
    Expression::Ptr initializer_;
    int slot_;
};

/* Class for functions belonging to a class or module */
class Function : public Feature {
public:
    Function(Location loc, Environment* env, String* name, Formal* formal,
        Flags flags, Type* ret, Block* block);

	enum ThrowSpec { UNKNOWN, THROW, NOTHROW };

	Formal* formals() const { return formals_; }
	Type* type() const { return type_; }
    Block* block() const { return block_; }
    BasicBlock* basic_block(int index) { return basic_block_[index]; }
	Function* called_func(int index) { return called_func_[index]; }
	ThrowSpec throw_spec() const;
    int basic_blocks() const { return basic_block_.size(); }
    int stack_vars() const { return stack_vars_; }
	int called_funcs() const { return called_func_.size(); }
    bool covariant(Function* other) const;
    bool is_constructor() const { return name()->string() == "@init"; }
    bool is_destructor() const { return name()->string() == "@destroy"; }
    bool is_virtual() const;
	bool is_primitive_op() const;
    void stack_vars_inc() { stack_vars_++; }
    void stack_vars(int value) { stack_vars_ = value; }
    void basic_block(BasicBlock* block) { basic_block_.push_back(block); }
    void basic_block_del_all() { basic_block_.clear(); }
    void formals(Formal* formals) { formals_ = formals; }
	void throw_spec(ThrowSpec spec) { throw_spec_ = spec; }
	void called_func(Function* func);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Function> Ptr;

private:
	Formal::Ptr formals_;
	Type::Ptr type_;
    Block::Ptr block_;
    std::vector<BasicBlock::Ptr> basic_block_;
	std::vector<Function*> called_func_;
    int stack_vars_;
	mutable ThrowSpec throw_spec_;
};

/* Represents a class object */
class Class : public Feature {
public:
    Class(Location loc, Environment* env, Type* type, Type* proto, 
       String* comment, Feature* feat);
    Class(Location loc, Environment* env, Type* type, Feature* feat);
    Class(Location loc, Environment* env, Type* type, Type* alt);
    Feature* features() const { return features_; }    
    Feature* feature(String* name) const;
    String* comment() const { return comment_; }
    Type* type() const { return type_; }
    Type* alternates() const { return alternates_; }
    Type* mixins() const { return mixins_; }
    Type* proto() const { return proto_; }
    String* default_enum_value() const;
    bool is_object() const { return is_alt() || proto_->is_object_proto(); }
    bool is_alt() const { return type_->is_any() || proto_->is_alt_proto(); }
    bool is_enum() const { return proto_->is_enum_proto(); }
    bool is_value() const { return is_enum() || proto_->is_value_proto(); }
    bool is_interface() const { return proto_->is_interface_proto(); }
    bool is_primitive() const { return type_->is_primitive(); }
    bool subtype(Class* other) const;
    int jump1(int index) const { return jump1_[index]; }
    Function* jump2(int index) const { return jump2_[index]; } 
    int size() const { return size_; }
    int jump1s() const { return jump1_.size(); }
    int jump2s() const { return jump2_.size(); }
    void feature(Feature* feature);
    void mixin(Type* mixin) { mixins_ = append(mixins_, mixin); }
    void jump1(int index, int d);
    void jump2(int index, Function* func);
    void size(int size) { size_ = size; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Class> Ptr;

private:
    void gen_equal_method();
    std::vector<int> jump1_;
    std::vector<Function::Ptr> jump2_;
    mutable std::map<Class*, bool> subtype_;
    Type::Ptr type_;
    Type::Ptr alternates_;
    Type::Ptr mixins_;
    Type::Ptr proto_;
    String::Ptr comment_;
    Feature::Ptr features_;
    Import::Ptr imports_;
    std::map<String::Ptr, Feature::Ptr> feature_;
    int size_;
};

/* Module, contains classes, functions, etc. */
class Module : public Feature {
public:
    Module(Location loc, Environment* env, String* name); 

    Feature* features() const { return features_; }
    Feature* feature(String* name) const { return query(feature_, name); }
    Import* imports() const { return imports_; }
    Import* import(String* name) const { return query(import_, name); }
    File* file(int index) const { return file_[index]; }
    int files() const { return file_.size(); }
    bool is_up_to_date() const;
    std::string lib_file() const;
    std::string exe_file() const;
    std::string api_file() const;
    void feature(Feature* feature);
    void import(Import* import);
    void location(Location location) { location_ = location; }
    void file(File* file) { file_.push_back(file); }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Module> Ptr; 

private:
    Feature::Ptr features_;
    Import::Ptr imports_;
    std::map<String::Ptr, Feature::Ptr> feature_;
    std::map<String::Ptr, Import::Ptr> import_;
    std::vector<File*> file_;
};

