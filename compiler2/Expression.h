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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "TreeNode.h"
#include "Name.h"

/* This file includes interfaces for expression nodes */
class Expression : public TreeNode {
public:
    Expression(Location loc) :
        TreeNode(loc) {
    }
	
	Expression* next() const { return next_; }
	void next(Expression* next) { next_ = next; }
    typedef Pointer<Expression> Ptr;

private:
	Expression* next_;
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class Literal : public Expression {
public:
    Literal(Location loc, Name* value, Name* type) :
        Expression(loc),
        value_(value),
        type_(type) {
    }

    Name* value() const { return value_; } 
    Name* type() const { return type_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr value_;
    Name::Ptr type_;
};

/* Simple binary expression */
class Binary : public Expression {
public:
    Binary(Location loc, Name* op, Expression* left, Expression* right) :
        Expression(loc),
        operation_(op),
        left_(left),
        right_(right) {
    }

    Name* operation() const { return operation_; }
    Expression* left() const { return left_; }
    Expression* right() const { return right_; } 

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr operation_;
    Expression::Ptr left_;
    Expression::Ptr right_;
};

/* Simple unary expression */
class Unary : public Expression {
public:
    Unary(Location loc, Name* op, Expression* child) :
        Expression(loc),
        operation_(op),
        child_(child) {
    }

    Name* operation() const { return operation_; }
    Expression* child() { return child_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr operation_;
    Expression::Ptr child_;
};

/* Normal function all */
class Call : public Expression {
public:
    Call(Location loc, Name* ident, Expression* args) :
        Expression(loc),
        identifier_(ident),
        args_(args) {
    }

    Name* identifier() const { return identifier_; }
    Expression* args() const { return args_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr identifier_;
    Expression::Ptr args_;
};

/* Object-oriented function call dispatch */
class Dispatch : public Expression {
public:
    Dispatch(Location loc, Name* ident, Expression* args) :
        Expression(loc),
        identifier_(ident),
        arguments_(args) {
    }

    Name* identifier() const { return identifier_; }
    Expression* arguments() const { return arguments_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr identifier_;
    Expression::Ptr arguments_;
};

/* Index operator call */
class Index : public Expression {
public:
    Index(Location loc, Expression* obj, Expression* index) :
        Expression(loc),
        object_(obj),
        index_(index) {
    }

    Expression* object() const { return object_; }
    Expression* index() const { return index_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Expression::Ptr object_;
    Expression::Ptr index_;
};

/* Identifier expression (variable access) */
class Identifier : public Expression {
public:
    Identifier(Location loc, Name* ident) :
        Expression(loc),
        identifier_(ident) {
    }

    Name* identifier() const { return identifier_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr identifier_;
};

/* Member identifier access */
class Member : public Expression {
public:
    Member(Location loc, Expression* child, Name* ident) :
        Expression(loc),
        child_(child),
        identifier_(ident) {
    }

    Name* identifier() const { return identifier_; }
    Expression* child() const { return child_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Name::Ptr identifier_;
    Expression::Ptr child_;

};

#endif
