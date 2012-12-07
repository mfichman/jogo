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
#include "String.hpp"
#include "Type.hpp"

/* This file includes interfaces for expression nodes */
class Expression : public TreeNode {
public:
    Expression(Location loc) :
        TreeNode(loc),
        parent_(0) {
    }
	
	Expression* next() const { return next_; }
    Expression* last() const { return last_; }
    Expression* parent() const { return parent_; }
    Type* type() const { return type_; }
	void next(Expression* next) { assert(next != this); next_ = next; }
    void last(Expression* last) { last_ = last; }
    void parent(Expression* parent) { parent_ = parent; }
    void type(Type* type) { type_ = type; assert(type_); }
    typedef Pointer<Expression> Ptr;

private:
	Expression::Ptr next_;
    Expression::Ptr last_;
    Type::Ptr type_;
    Expression* parent_;
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class StringLiteral : public Expression {
public:
    StringLiteral(Location loc, String* value) :
        Expression(loc),
        value_(value) {
    }
    String* value() const { return value_; } 
    typedef Pointer<StringLiteral> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class IntegerLiteral : public Expression {
public:
    IntegerLiteral(Location loc, String* value) :
        Expression(loc),
        value_(value) {
    }
    String* value() const { return value_; } 
    typedef Pointer<IntegerLiteral> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class NilLiteral : public Expression {
public:
    NilLiteral(Location loc) : Expression(loc) {}
    typedef Pointer<NilLiteral> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class FloatLiteral : public Expression {
public:
    FloatLiteral(Location loc, String* value) :
        Expression(loc),
        value_(value) {
    }
    String* value() const { return value_; } 
    typedef Pointer<FloatLiteral> Ptr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};

/* Boolean literal expression */
class BooleanLiteral : public Expression {
public:
    BooleanLiteral(Location loc, String* value) :
        Expression(loc),
        value_(value) {
    }
    String* value() const { return value_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};    

/* Hash literal */
class HashLiteral : public Expression {
public:
    HashLiteral(Location loc, Expression* args) :
        Expression(loc),
        arguments_(args) {
    }

    Expression* arguments() const { return arguments_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Expression::Ptr arguments_;
};

/* Array literal */
class ArrayLiteral : public Expression {
public:
    ArrayLiteral(Location loc, Expression* args) :
        Expression(loc),
        arguments_(args) {
    }

    Expression* arguments() const { return arguments_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Expression::Ptr arguments_;
};

/* Simple binary expression */
class Binary : public Expression {
public:
    Binary(Location loc, String* op, Expression* left, Expression* right) :
        Expression(loc),
        operation_(op),
        left_(left),
        right_(right) {

        left->parent(this);
        right->parent(this);
    }

    String* operation() const { return operation_; }
    Expression* left() const { return left_; }
    Expression* right() const { return right_; } 

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr operation_;
    Expression::Ptr left_;
    Expression::Ptr right_;
};

/* Simple unary expression */
class Unary : public Expression {
public:
    Unary(Location loc, String* op, Expression* child) :
        Expression(loc),
        operation_(op),
        child_(child) {

        child->parent(this);
    }

    String* operation() const { return operation_; }
    Expression* child() { return child_; }
    void operator()(Functor* functor) { functor->operator()(this); }

private:
    String::Ptr operation_;
    Expression::Ptr child_;
};

/* Normal function all */
class Call : public Expression {
public:
    Call(Location loc, Expression* expr, Expression* args) :
        Expression(loc),
        expression_(expr),
        arguments_(args),
        function_(0) {

        expr->parent(this);
    }

    Expression* expression() const { return expression_; }
    Expression* arguments() const { return arguments_; }
    Expression* receiver() const { return receiver_; }
    Function* function() const { return function_; }
    void arguments(Expression* args) { arguments_ = args; }
    void function(Function* function) { function_ = function; }
    void receiver(Expression* receiver) { receiver_ = receiver; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Call> Ptr;

private:
    Expression::Ptr expression_;
    Expression::Ptr arguments_;
    Expression::Ptr receiver_;
    Function* function_;
};

/* Member access (e.g., object.attribute) */
class Member : public Expression {
public:
    Member(Location loc, Expression* expr, String* ident) :
        Expression(loc), 
        identifier_(ident),
        expression_(expr),
        function_(0) {

        expr->parent(this);
    }

    String* identifier() const { return identifier_; }
    Expression* expression() const { return expression_; }
    Function* function() const { return function_; }
    void function(Function* function) { function_ = function; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr identifier_;
    Expression::Ptr expression_;
    Function* function_;
};

/* Constructor call */
class Construct : public Expression {
public:
    Construct(Location loc, Type* type, Expression* args) :
        Expression(loc),
        arguments_(args) {
        
        Expression::type(type);
        for (Expression* arg = args; arg; arg = arg->next()) {
            arg->parent(this);
        }
    }

    Expression* arguments() const { return arguments_; }
    void arguments(Expression* args) { arguments_ = args; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Construct> Ptr;

private:
    Type::Ptr clazz_;
    Expression::Ptr arguments_;
};

/* Identifier expression (variable access) */
class Identifier : public Expression {
public:
    Identifier(Location loc, String* scope, String* ident) :
        Expression(loc),
        scope_(scope),
        identifier_(ident) {

    }

    String* scope() const { return scope_; }
    String* identifier() const { return identifier_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Identifier> Ptr;

private:
    String::Ptr scope_;
    String::Ptr identifier_;
};

/* Constant identifier access expression */
class ConstantIdentifier : public Expression {
public:
    ConstantIdentifier(Location loc, String* scope, String* ident) :
        Expression(loc),
        scope_(scope),
        identifier_(ident) {
    }

    String* scope() const { return scope_; }
    String* identifier() const { return identifier_; }
    Constant* constant() const { return constant_; }
    void constant(Constant* constant) { constant_ = constant; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<ConstantIdentifier> Ptr;
    
private:
    String::Ptr scope_;
    String::Ptr identifier_;
    Constant* constant_;
};

/* Variable declaration */
class Assignment : public Expression {
public:
    typedef int Flags;
    Assignment(Location loc, String* ident, Type* type, Expression* expr) :
        Expression(loc),
        identifier_(ident),
		declared_type_(type),
        initializer_(expr),
        is_let_(false) {

        expr->parent(this);
        assert(declared_type_);
    }

    String* identifier() const { return identifier_; }
    Type* declared_type() { return declared_type_; }
    Expression* initializer() const { return initializer_; }
    bool is_let() const { return is_let_; }
    void initializer(Expression* expr) { initializer_ = expr; }
    void is_let(bool let) { is_let_ = let; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Assignment> Ptr;

private:
    String::Ptr identifier_;
	Type::Ptr declared_type_;
    Expression::Ptr initializer_;
    bool is_let_;
};

/* Closure */
class Closure : public Expression {
public:
    Closure(Location loc, Function* function, Class* clazz) :
        Expression(loc),
        function_(function),
        class_(clazz) {
    }

    Function* function() const { return function_; } 
    Class* clazz() const { return class_; }
    Construct* construct() const { return construct_; }
    void construct(Construct* construct) { construct_ = construct; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Assignment> Ptr;

private:
    Function* function_;
    Class* class_;
    Construct::Ptr construct_;
};

/* Empty expression */
class Empty : public Expression {
public:
    Empty(Location loc) :
        Expression(loc) {
    }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Empty> Ptr;
};

/* Cast expression */
class Cast : public Expression {
public:
    Cast(Location loc, Type* type, Expression* expr) :
        Expression(loc),
        child_(expr) {
        
        Expression::type(type);
    }

    Expression* child() const { return child_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Cast> Ptr;

private:
    Expression::Ptr child_;
};

/* An 'is' type check expression */
class Is : public Expression {
public:
    Is(Location loc, Expression* expr, Type* type) :
        Expression(loc),
        child_(expr),
        check_type_(type) {
    }

    Expression* child() const { return child_; }
    Type* check_type() const { return check_type_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Is> Ptr;

private:
    Expression::Ptr child_;
    Type::Ptr check_type_;
};

/* Boxes a primitive */
class Box : public Expression {
public:
    Box(Location loc, Type* type, Expression* expr) :
        Expression(loc),
        child_(expr) {
        
        Expression::type(type);
    }

    Expression* child() const { return child_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    
private:
    Expression::Ptr child_;
};
 

/* Parse error placeholder expression */
class ParseError : public Expression {
public:
    ParseError(Location loc) :
        Expression(loc) {
    }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<ParseError> Ptr;
};

