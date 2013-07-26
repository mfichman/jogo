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
    typedef Iterator<Expression> Itr;

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
    typedef Iterator<StringLiteral> Itr;

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
    typedef Iterator<IntegerLiteral> Itr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};

/* Literal expression (integers, strings, booleans, hashes, etc.) */
class NilLiteral : public Expression {
public:
    NilLiteral(Location loc) : Expression(loc) {}
    typedef Pointer<NilLiteral> Ptr;
    typedef Iterator<NilLiteral> Itr;

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
    typedef Iterator<FloatLiteral> Itr;

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
    typedef Pointer<BooleanLiteral> Ptr;
    typedef Iterator<BooleanLiteral> Itr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr value_;
};    

/* Hash literal */
class HashLiteral : public Expression {
public:
    HashLiteral(Location loc, Expression* args);
    typedef Pointer<HashLiteral> Ptr;
    typedef Iterator<HashLiteral> Itr;

    Expression* arguments() const { return arguments_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Expression::Ptr arguments_;
};

/* Array literal */
class ArrayLiteral : public Expression {
public:
    ArrayLiteral(Location loc, Expression* args);
    typedef Pointer<ArrayLiteral> Ptr;
    typedef Iterator<ArrayLiteral> Itr;

    Expression* arguments() const { return arguments_; }

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    Expression::Ptr arguments_;
};

/* Simple binary expression */
class Binary : public Expression {
public:
    Binary(Location loc, String* op, Expression* left, Expression* right);
    typedef Pointer<Binary> Ptr;
    typedef Iterator<Binary> Itr;

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
    Unary(Location loc, String* op, Expression* child);
    typedef Pointer<Unary> Ptr;
    typedef Iterator<Unary> Itr;

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
    Call(Location loc, Expression* expr, Expression* args);
    Expression* expression() const { return expression_; }
    Expression* arguments() const { return arguments_; }
    Expression* receiver() const { return receiver_; }
    Function* function() const { return function_; }
    void arguments(Expression* args) { arguments_ = args; }
    void function(Function* function) { function_ = function; }
    void receiver(Expression* receiver) { receiver_ = receiver; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Call> Ptr;
    typedef Iterator<Call> Itr;

private:
    Expression::Ptr expression_;
    Expression::Ptr arguments_;
    Expression::Ptr receiver_;
    Function* function_;
};

/* Member access (e.g., object.attribute) */
class Member : public Expression {
public:
    Member(Location loc, Expression* expr, String* ident);
    String* identifier() const { return identifier_; }
    Expression* expression() const { return expression_; }
    Function* function() const { return function_; }
    void function(Function* function) { function_ = function; }
    typedef Pointer<Member> Ptr;
    typedef Iterator<Member> Itr;

private:
    void operator()(Functor* functor) { functor->operator()(this); }
    String::Ptr identifier_;
    Expression::Ptr expression_;
    Function* function_;
};

/* Constructor call */
class Construct : public Expression {
public:
    Construct(Location loc, Type* type, Expression* args);
    Expression* arguments() const { return arguments_; }
    void arguments(Expression* args) { arguments_ = args; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Construct> Ptr;
    typedef Iterator<Construct> Itr;

private:
    Type::Ptr clazz_;
    Expression::Ptr arguments_;
};

/* IdentifierRef expression (variable access) */
class IdentifierRef : public Expression {
public:
    IdentifierRef(Location loc, String* scope, String* ident);
    String* scope() const { return scope_; }
    String* identifier() const { return identifier_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<IdentifierRef> Ptr;
    typedef Iterator<IdentifierRef> Itr;

private:
    String::Ptr scope_;
    String::Ptr identifier_;
};

/* Constant identifier access expression */
class ConstantRef : public Expression {
public:
    ConstantRef(Location loc, String* scope, String* ident);
    String* scope() const { return scope_; }
    String* identifier() const { return identifier_; }
    Constant* constant() const { return constant_; }
    void constant(Constant* constant) { constant_ = constant; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<ConstantRef> Ptr;
    
private:
    String::Ptr scope_;
    String::Ptr identifier_;
    Constant* constant_;
};

/* Variable declaration */
class Assignment : public Expression {
public:
    typedef int Flags;
    Assignment(Location loc, String* ident, Type* type, Expression* expr, Flags flags=0);
    String* identifier() const { return identifier_; }
    Type* declared_type() { return declared_type_; }
    Expression* initializer() const { return initializer_; }
    Flags flags() const { return flags_; }
    bool is_let() const { return flags_ & LET; }
    bool is_mutable() const { return flags_ & MUTABLE; }
    void is_let(bool let) { flags_ = let ? (flags_ | LET) : (flags_ & ~LET); }
    void is_mutable(bool mut) { flags_ = mut ? (flags_ | MUTABLE) : (flags_ & ~MUTABLE); }
    void flags(Flags flags) { flags_ = flags; }
    void initializer(Expression* expr) { initializer_ = expr; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Assignment> Ptr;

    static const int MUTABLE = 0x1;
    static const int LET = 0x2;
private:
    String::Ptr identifier_;
	Type::Ptr declared_type_;
    Expression::Ptr initializer_;
    Flags flags_;
};

/* Closure */
class Closure : public Expression {
public:
    Closure(Location loc, Function* function, Class* clazz);
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
    Empty(Location loc);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Empty> Ptr;
};

/* Cast expression */
class Cast : public Expression {
public:
    Cast(Location loc, Type* type, Expression* expr);
    Expression* child() const { return child_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Cast> Ptr;

private:
    Expression::Ptr child_;
};

/* An 'is' type check expression */
class Is : public Expression {
public:
    Is(Location loc, Expression* expr, Type* type);
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
    Box(Location loc, Type* type, Expression* expr);
    Expression* child() const { return child_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    
private:
    Expression::Ptr child_;
};

/* Case statement */
class Case : public Expression {
public:
    Case(Location loc, Expression* guard, Expression* block);
    Expression* guard() const { return guard_; }
    Expression* block() const { return block_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Case> Ptr;

private:
    Expression::Ptr guard_;
    Expression::Ptr block_;
};

 
/* Match expression */
class Match : public Expression {
public:
    Match(Location loc, Expression* guard, Case* cases);
    Expression* guard() const { return guard_; }
    Case* cases() const { return cases_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Match> Ptr;

private:
    Expression::Ptr guard_;
    Case::Ptr cases_;
};

/* While loop */
class While : public Expression {
public:
    While(Location loc, Expression* guard, Expression* block);
    Expression* guard() const { return guard_; }
    Expression* block() const { return block_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<While> Ptr;

private:
    Expression::Ptr guard_;
    Expression::Ptr block_;
};

/* Conditional */
class Conditional : public Expression {
public:
    Conditional(Location loc, Expression* ex, Expression* yes, Expression* no);
    Expression* guard() const { return guard_; }
    Expression* true_branch() const { return true_branch_; }
    Expression* false_branch() const { return false_branch_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Conditional> Ptr;

private:
    Expression::Ptr guard_;
    Expression::Ptr true_branch_;
    Expression::Ptr false_branch_;
};

/* Let statement */
class Let : public Expression {
public:
    Let(Location loc, Assignment* variables, Expression* block);
    Assignment* variables() const { return variables_; }
    Expression* block() const { return block_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Let> Ptr;

private:
    Assignment::Ptr variables_;
    Expression::Ptr block_;
};

/* Block statement */
class Block : public Expression {
public:
    Block(Location loc, String* comment, Expression* children);
    String* comment() const { return comment_; }
    Expression* children() const { return children_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Block> Ptr;

private:
    String::Ptr comment_;
    Expression::Ptr children_;
};

/* Return statement */
class Return : public Expression {
public:
    Return(Location loc, Expression* expr);
    Expression* expression() const { return expression_; }
    void expression(Expression* expr) { expression_ = expr; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Return> Ptr;

private:
    Expression::Ptr expression_;
};

/* Fork statement */
class Fork : public Expression {
public:
    Fork(Location loc, Expression* expression);
    Expression* expression() const { return expression_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Fork> Ptr;

private:
    Expression::Ptr expression_;
};

#undef Yield

/* Yield statement */
class Yield : public Expression {
public:
    Yield(Location loc, Expression* expression);
    Expression* expression() const { return expression_; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Yield> Ptr;

private:
    Expression::Ptr expression_;
};


/* Parse error placeholder expression */
class ParseError : public Expression {
public:
    ParseError(Location loc);
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<ParseError> Ptr;
};

