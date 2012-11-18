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

#include "CodeExpander.hpp"

CodeExpander::CodeExpander(Environment* environment) :
    env_(environment) {

    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void
CodeExpander::operator()(Module* feature) {
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void
CodeExpander::operator()(Class* feature) {
    class_ = feature;
    for (Type::Ptr m = feature->mixins(); m; m = m->next()) {
        mixin_ = m->clazz();
        mixin(mixin_);
    }
}

void
CodeExpander::operator()(Function* func) {
}

void
CodeExpander::stub(Function* func) { 
    // Create a stub which calls through to the nested class function.
    std::string name = func->name()->string();
    if (name == "@init" || name == "@destroy" || name == "@copy") {
        return;
    }
    if (class_->feature(func->name()) || func->is_private()) {
        return;
    }

    String::Ptr nm = func->name();
    Feature::Flags flags = func->flags();
    Formal::Ptr f = func->formals();
    Type::Ptr type = func->type();
    Location loc = class_->location();
    Expression::Ptr args;
    for (Formal::Ptr f = func->formals(); f; f = f->next()) {
        if (f->name()->string() != "self") {
            Expression::Ptr arg(new Identifier(loc, env_->name(""), f->name()));
            args = append(args.pointer(), arg.pointer());
        }
    }

    Expression::Ptr self(new Identifier(loc, env_->name(""), mixin_->name())); 
    Expression::Ptr mem(new Member(loc, self, func->name()));
    Call::Ptr call(new Call(loc, mem, args));
    Statement::Ptr stmt;
    if (func->type()->is_void()) {
        stmt = new Simple(loc, call); 
    } else {    
        stmt = new Return(loc, call);
    }
    Block::Ptr block(new Block(loc, 0, stmt));
    Function::Ptr stub(new Function(loc, env_, nm, f, flags, type, block));
    class_->feature(stub);
}

void
CodeExpander::mixin(Class* feature) {
    if (feature->name()->string() == "Object") {
        return;
    }
    for (Type::Ptr m = feature->mixins(); m; m = m->next()) {
        mixin(m->clazz());
    }
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f.pointer())) {
            stub(func);
        } 
    }
}
