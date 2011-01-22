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

#ifndef FEATURE_HPP
#define FEATURE_HPP

#include "Apollo.hpp"
#include "TreeNode.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Formal.hpp"
#include "Type.hpp"

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
    typedef Pointer<Attribute> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
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
        block_(blk) {
    }

    Name* name() const { return name_; }
	Formal* formals() const { return formals_; }
	Type* type() const { return type_; }
    Statement* block() const { return block_; }
    typedef Pointer<Function> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr name_;
	Formal::Ptr formals_;
	Type::Ptr type_;
    Statement::Ptr block_;
};

/* Class for imports */
class Import : public Feature {
public:
    Import(Location loc, Type* type) :
        Feature(loc),
        type_(type) {
    }

    const std::string& file() const { return file_; }
    Type* type() const { return type_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    std::string file_;
    Type::Ptr type_;
};

/* Type definition */
class Define : public Feature {
public:
    Define(Location loc, Name* name, Type* type) :
        Feature(loc),
        name_(name),
        type_(type) {
    }

    Name* name() const { return name_; }
    Type* type() const { return type_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr name_;
    Type::Ptr type_;
};

#endif
