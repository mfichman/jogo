/*
 * Copyright (c) 2013 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  


#include "UsageFinder.hpp"

UsageFinder::UsageFinder(Environment* env, String* qn) :
    search_(env->feature(qn)) {

    if (!search_) {
        return;
    }
    for (File::Itr f = env->files(); f; ++f) {
        if (f->is_input_file()) {
            this->operator()(f);;
        }
    } 
}

void UsageFinder::operator()(File* file) {
    for (Feature::Itr f = file->features(); f; ++f) {
        f(this); 
    }
}

void UsageFinder::operator()(Module* feature) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Class* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    // FIXME: Find range for class name
}

void UsageFinder::operator()(Formal* formal) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Cast* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Box* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Binary* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Unary* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Call* call) {
    Expression::Ptr expr = call->expression();
    expr(this);
    for (Expression::Ptr arg = call->arguments(); arg; arg = arg->next()) {
        arg(this);
    }
}

void UsageFinder::operator()(Construct* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(IdentifierRef* expr) {
    if (Call::Ptr call = dynamic_cast<Call*>(expr->parent())) {
        if (call->function() == search_) {
            usage_.push_back(Usage(expr));
        }
    }
}

void UsageFinder::operator()(Member* expr) {
    if (Call::Ptr call = dynamic_cast<Call*>(expr->parent())) {
        if (call->function() == search_) {
            //usage_.push_back(Usage(expr->identifier()));
            //Location l1 = expr->expression()->location();
            //Location l2 = expr->location();
            usage_.push_back(Usage(expr)); 
            //FIXME: Adjust size of the expression to cover only the
            //identifier.
        }
    }
}


void UsageFinder::operator()(ConstantRef* expr) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Let* statement) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Block* stmt) {
    for (Statement::Ptr c = stmt->children(); c; c = c->next()) {
        c(this);
    }
}

void UsageFinder::operator()(Simple* statement) {
    Expression::Ptr expr = statement->expression();
    expr(this);
}

void UsageFinder::operator()(While* statement) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Conditional* statement) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Assignment* statement) {
    ///assert(!"Not implemented");
}

void UsageFinder::operator()(Return* statement) {
}

void UsageFinder::operator()(Match* statement) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Case* statement) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Function* feature) {
    Statement::Ptr block = feature->block();
    block(this);
    // FIXME: Check parameter types
    // FIXME: Find range for the function name in the definition
}

void UsageFinder::operator()(Attribute* feature) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Constant* constant) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Import* feature) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(HashLiteral* literal) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(ArrayLiteral* literal) {
    assert(!"Not implemented");
}

void UsageFinder::operator()(Is* expr) {
    assert(!"Not implemented");
}

