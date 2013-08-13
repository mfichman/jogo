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

#include "DepScanner.hpp"

DepScanner::DepScanner(Environment* env) : env_(env) {
}

void DepScanner::operator()(File* file) {
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this);
    }
}

void DepScanner::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void DepScanner::operator()(Constant* feature) {
    dependency_.insert(feature);
}

void DepScanner::operator()(Formal* formal) {
     
}

void DepScanner::operator()(StringLiteral* expr) {
    string_.insert(expr->value());
}

void DepScanner::operator()(NilLiteral* expr) {
}

void DepScanner::operator()(IntegerLiteral* expr) {
     
}

void DepScanner::operator()(FloatLiteral* expr) {
}

void DepScanner::operator()(BooleanLiteral* expr) {
}

void DepScanner::operator()(Binary* expr) {
}

void DepScanner::operator()(Unary* expr) {
}

void DepScanner::operator()(Call* expr) {
    dependency_.insert(expr->function());

    Expression::Ptr child = expr->expression();
    child(this);
    for (Expression::Ptr arg = expr->arguments(); arg; arg = arg->next()) {
        arg(this);
    } 
}

void DepScanner::operator()(Is* expr) {
}

void DepScanner::operator()(Member* expr) {
}

void DepScanner::operator()(Construct* expr) {
    String::Ptr id = env_->name("@init");
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->function(id);
    dependency_.insert(func.pointer());

    for (Expression::Ptr arg = expr->arguments(); arg; arg = arg->next()) {
        arg(this);
    } 
}

void DepScanner::operator()(IdentifierRef* expr) {
}

void DepScanner::operator()(ConstantRef* expr) {
}

void DepScanner::operator()(Empty* expr) {
}

void DepScanner::operator()(Cast* expr) {
}

void DepScanner::operator()(Box* expr) {
}

void DepScanner::operator()(Block* expr) {
	for (Expression::Ptr s = expr->children(); s; s = s->next()) {
		s(this);
	}
}

void DepScanner::operator()(Let* let) {
}

void DepScanner::operator()(While* expr) {
}

void DepScanner::operator()(Conditional* expr) {
}

void DepScanner::operator()(Assignment* expr) {
    Expression::Ptr init = expr->initializer();
    init(this);
}

void DepScanner::operator()(Return* expr) {
    Expression::Ptr child = expr->expression();
    child(this); 
}

void DepScanner::operator()(Match* expr) {
}

void DepScanner::operator()(Case* expr) {
}

void DepScanner::operator()(Fork* expr) {
}

void DepScanner::operator()(Yield* expr) {
}

void DepScanner::operator()(Function* feature) {
    Block::Ptr block = feature->block();
    block(this);
}

void DepScanner::operator()(Attribute* feature) {
}

void DepScanner::operator()(Import* feature) {
}

void DepScanner::operator()(Closure* expr) {
}

void DepScanner::operator()(Type* type) {
}

