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
#include "Statement.hpp"
#include "Expression.hpp"
#include "Formal.hpp"
#include "Type.hpp"
#include "IrBlock.hpp"
#include "Import.hpp"
#include "OrderedMap.hpp"
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
        parent_(0) {
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
    bool is_component() const { return flags_ & COMPONENT; }
    void next(Feature* next) { next_ = next; }
    void last(Feature* last) { last_ = last; }
    void flags(Flags flags) { flags_ = flags; }
    void parent(Feature* parent) { parent_ = parent; }
    typedef Pointer<Feature> Ptr;
    typedef Iterator<Feature> Itr;

    static const int PRIVATE = 0x1;
    static const int NATIVE = 0x2;
    static const int WEAK = 0x4;
    static const int IMMUTABLE = 0x8;
    static const int CLOSURE = 0x20;
    static const int EMBEDDED = 0x40; 
    static const int COMPONENT = 0x80;

private:
    String::Ptr name_;
    Feature::Ptr next_;
    Feature::Ptr last_;
    Environment* env_;
    Flags flags_;
    mutable String::Ptr label_;
    Feature* parent_;
};

/* Represents a Constant initializer */
class Constant : public Feature {
public:
    Constant(Location loc, Environment* env, String* name, Flags flags, 
        Type* type, Expression* init);

    Type* declared_type() const { return declared_type_; }
    Type* type() const { return type_; }
    Expression* initializer() const { return initializer_; }
    void type(Type* type) { type_ = type; assert(type_); } 
    void initializer(Expression* init) { initializer_ = init; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Constant> Ptr;
    typedef Iterator<Constant> Itr;

private:
    Type::Ptr declared_type_;
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
    void initializer(Expression* expr) { initializer_ = expr; }
    void slot(int slot) { slot_ = slot; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Attribute> Ptr;
    typedef Iterator<Attribute> Itr;

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
    IrBlock* ir_block(int index) { return ir_block_[index]; }
	Function* called_func(int index) { return called_func_[index]; }
	ThrowSpec throw_spec() const;
    int ir_blocks() const { return ir_block_.size(); }
    int local_slots() const { return local_slots_; }
    int arg_slots() const { return arg_slots_; }
    int stack_slots() const;
	int called_funcs() const { return called_func_.size(); }
    int temp_regs() const { return temp_regs_; }
    bool covariant(Function* other) const;
    bool is_constructor() const { return name()->string() == "@init"; }
    bool is_destructor() const { return name()->string() == "@destroy"; }
    bool is_copier() const { return name()->string() == "@copy"; }
    bool is_virtual() const;
	bool is_primitive_op() const;
    void local_slots_inc() { local_slots_++; }
    void local_slots(int value) { local_slots_ = value; }
    void arg_slots_inc() { arg_slots_++; }
    void arg_slots(int value) { arg_slots_ = value; }
    void temp_regs(int value) { temp_regs_ = value; }
    void ir_block(IrBlock* block) { ir_block_.push_back(block); }
    void ir_block_del_all() { ir_block_.clear(); }
    void formals(Formal* formals) { formals_ = formals; }
	void throw_spec(ThrowSpec spec) { throw_spec_ = spec; }
    void block(Block* block) { block_ = block; }
	void called_func(Function* func);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Function> Ptr;
    typedef Iterator<Function> Itr;

private:
	Formal::Ptr formals_;
	Type::Ptr type_;
    Block::Ptr block_;
    std::vector<IrBlock::Ptr> ir_block_;
	std::vector<Function*> called_func_;
    int local_slots_;
    int arg_slots_;
    int temp_regs_;
	mutable ThrowSpec throw_spec_;
};

/* Represents a class object */
class Class : public Feature {
public:
    Class(Location loc, Environment* env, Type* type, Type* proto, 
       String* comment, Feature* feat);
    Class(Location loc, Environment* env, Type* type, Feature* feat);
    Class(Location loc, Environment* env, Type* type, Type* alt);
    Function* constructor() const;
    Function* destructor() const;
    Function* copier() const;
    Feature* features() const { return features_; }    
    Feature* feature(String* name) const;
    String* comment() const { return comment_; }
    Type* type() const { return type_; }
    Type* alternates() const { return alternates_; }
    Type* proto() const { return proto_; }
    String* default_enum_value() const;
    bool is_object() const { return type_->is_object(); }
    bool is_functor() const { return type_->is_functor(); }
    bool is_any() const { return type_->is_any(); }
    bool is_union() const { return type_->is_union(); }
    bool is_interface() const { return type_->is_interface(); }
    bool is_enum() const { return type_->is_enum(); }
    bool is_value() const { return type_->is_value(); }
    bool is_alt() const { return type_->is_alt(); }
    bool is_primitive() const { return type_->is_primitive(); }
    bool is_ref() const { return type_->is_ref(); }
    bool is_compound() const { return type_->is_compound(); }
    int jump1(int index) const { return jump1_[index]; }
    Function* jump2(int index) const { return jump2_[index]; } 
    int slots() const { return slots_; }
    int jump1s() const { return jump1_.size(); }
    int jump2s() const { return jump2_.size(); }
    void feature(Feature* feature);
    void jump1(int index, int d);
    void jump2(int index, Function* func);
    void slots_inc(int words) { slots_ += words; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Class> Ptr;
    typedef Iterator<Class> Itr;

private:
    void gen_equal_method();
    void gen_functor_method();
    std::vector<int> jump1_;
    std::vector<Function::Ptr> jump2_;
    Type::Ptr type_;
    Type::Ptr alternates_;
    Type::Ptr proto_;
    String::Ptr comment_;
    Feature::Ptr features_;
    Import::Ptr imports_;
    std::map<String::Ptr, Feature::Ptr> feature_;
    int slots_;
};

/* Module, contains classes, functions, etc. */
class Module : public Feature {
public:
    Module(Location loc, Environment* env, String* name); 

    Feature* feature(String* name) const { return feature_.value(name); }
    File* file(int index) const { return file_[index]; }
    Feature::Itr features() const { return feature_.iterator(); }
    int files() const { return file_.size(); }
    // FIXME: Convert to a File::Itr
    bool is_up_to_date() const;
    std::string lib_file() const;
    std::string exe_file() const;
    std::string jgi_file() const;
    void feature(Feature* feature);
    void location(Location location) { location_ = location; }
    void file(File* file) { file_.push_back(file); }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Module> Ptr; 
    typedef Iterator<Module> Itr;

private:
    OrderedMap<String::Ptr, Feature> feature_;
    std::vector<File*> file_;
};

