/*
 * Copyright (c) 2013 Matt Fichman
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

#include "Expression.hpp"

HashLiteral::HashLiteral(Location loc, Expression* args) :
    Expression(loc),
    arguments_(args) {

    for(Expression* arg = args; arg; arg = arg->next()) {
        arg->parent(this);
    }
}

ArrayLiteral::ArrayLiteral(Location loc, Expression* args) :
    Expression(loc),
    arguments_(args) {

    for(Expression* arg = args; arg; arg = arg->next()) {
        arg->parent(this);
    }
}

Binary::Binary(Location loc, String* op, Expression* left, Expression* right) :
    Expression(loc),
    operation_(op),
    left_(left),
    right_(right) {

    left->parent(this);
    right->parent(this);
}

Unary::Unary(Location loc, String* op, Expression* child) :
    Expression(loc),
    operation_(op),
    child_(child) {

    child->parent(this);
}

Call::Call(Location loc, Expression* expr, Expression* args) :
    Expression(loc),
    expression_(expr),
    arguments_(args),
    function_(0) {

    expr->parent(this);
    for (Expression* arg = args; arg; arg = arg->next()) {
        arg->parent(this);
    }
}

Member::Member(Location loc, Expression* expr, String* ident) :
    Expression(loc), 
    identifier_(ident),
    expression_(expr),
    function_(0) {

    expr->parent(this);
}

Construct::Construct(Location loc, Type* type, Expression* args) :
    Expression(loc),
    arguments_(args) {
    
    Expression::type(type);
    for (Expression* arg = args; arg; arg = arg->next()) {
        arg->parent(this);
    }
}

IdentifierRef::IdentifierRef(Location loc, String* scope, String* ident) :
    Expression(loc),
    scope_(scope),
    identifier_(ident) {

}

ConstantRef::ConstantRef(Location loc, String* scope, String* ident) :
    Expression(loc),
    scope_(scope),
    identifier_(ident) {
}

Assignment::Assignment(Location loc, String* ident, Type* type, Expression* expr, Flags flags) :
    Expression(loc),
    identifier_(ident),
	declared_type_(type),
    initializer_(expr),
    flags_(flags) {

    expr->parent(this);
    assert(declared_type_);
}

Closure::Closure(Location loc, Function* function, Class* clazz) :
    Expression(loc),
    function_(function),
    class_(clazz) {
}

Empty::Empty(Location loc) :
    Expression(loc) {
}

Cast::Cast(Location loc, Type* type, Expression* expr) :
    Expression(loc),
    child_(expr) {
    
    Expression::type(type);
    expr->parent(this);
}

Is::Is(Location loc, Expression* expr, Type* type) :
    Expression(loc),
    child_(expr),
    check_type_(type) {
    
    expr->parent(this);
}

Box::Box(Location loc, Type* type, Expression* expr) :
    Expression(loc),
    child_(expr) {
    
    Expression::type(type);
    expr->parent(this);
}

Case::Case(Location loc, Expression* guard, Expression* block) :
    Expression(loc),
    guard_(guard),
    block_(block) {

    guard->parent(this);
    block->parent(this);
}

Match::Match(Location loc, Expression* guard, Case* cases) : 
    Expression(loc),
    guard_(guard),
    cases_(cases) {

    guard->parent(this);

    for (Expression* cas = cases; cas; cas = cas->next()) {
        cas->parent(this);
    } 
}

While::While(Location loc, Expression* guard, Expression* block) :
    Expression(loc), 
    guard_(guard),
    block_(block) {

    guard->parent(this);
    block->parent(this);
}

Conditional::Conditional(Location loc, Expression* ex, Expression* yes, Expression* no) :
    Expression(loc),
    guard_(ex),
    true_branch_(yes),
    false_branch_(no) {

    yes->parent(this);
    if (no) {
        no->parent(this);
    }
}

Let::Let(Location loc, Assignment* variables, Expression* block) :
    Expression(loc),
    variables_(variables),
    block_(block) {

    for (Expression* expr = variables; expr; expr = expr->next()) {
        Assignment* var = static_cast<Assignment*>(expr);
        var->is_let(true);
        expr->parent(this);
    }
    block->parent(this);
}

Block::Block(Location loc, String* comment, Expression* children) :
    Expression(loc),
    comment_(comment),
    children_(children) {

    for(Expression* expr = children; expr; expr = expr->next()) {
        expr->parent(this);
    }
}

Return::Return(Location loc, Expression* expr) :
    Expression(loc),
    expression_(expr) {

    expr->parent(this);
}

Fork::Fork(Location loc, Expression* expression) : 
    Expression(loc),
    expression_(expression) {

    expression->parent(this);
}

Yield::Yield(Location loc, Expression* expression) :
    Expression(loc),
    expression_(expression) {

    if (expression) {
        expression->parent(this);
    }
}

ParseError::ParseError(Location loc) :
    Expression(loc) {
}
