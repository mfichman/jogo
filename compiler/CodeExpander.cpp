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

    for (Module::Itr m = env_->modules(); m; ++m) {
        m(this);
    }    
}

void
CodeExpander::operator()(Module* feature) {
    for (Feature::Itr f = feature->features(); f; ++f) {
        f(this);
    }
}

void
CodeExpander::operator()(Class* feature) {
    class_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (f->is_embedded()) {
            Attribute* attr = dynamic_cast<Attribute*>(f.pointer());
            component(attr);
        }
    }
    if (class_->is_functor()) {
        functor(class_);
    }
}

void
CodeExpander::operator()(Function* func) {
}

void
CodeExpander::stub(Function* func, Attribute* attr) { 
    // Create a stub which calls through to the nested class function.
    std::string name = func->name()->string();
    if (name == "@init" || name == "@destroy" || name == "@copy") {
        return;
    }
    if ((class_->feature(func->name()) != func) || func->is_private()) {
        return;
    }
    String::Ptr nm = func->name();
    Feature::Flags flags = func->flags() & ~(Feature::NATIVE);
    Formal::Ptr f = func->formals();
    Type::Ptr type = func->type();
    Location loc = class_->location();
    Expression::Ptr args;

    Expression::Ptr self(new IdentifierRef(loc, env_->name(""), attr->name()));
    for (Formal::Ptr f = func->formals(); f; f = f->next()) {
        Expression::Ptr arg;
        if (f->name()->string() == "self") {
            arg = self; 
        } else {
            arg = new IdentifierRef(loc, env_->name(""), f->name());
        }
        arg->type(f->type());
        args = append(args.pointer(), arg.pointer());
    }

    Expression::Ptr mem(new Member(loc, self, func->name()));
    Call::Ptr call(new Call(loc, mem, args));
    call->function(func); 
    
    Statement::Ptr stmt;
    if (func->type()->is_void()) {
        stmt = new Simple(loc, call); 
    } else {    
        stmt = new Return(loc, call);
        call->type(func->type());
    }
    Block::Ptr block(new Block(loc, 0, stmt));
    Function::Ptr stub(new Function(loc, env_, nm, f, flags, type, block));
    class_->feature(stub);
}

void
CodeExpander::component(Attribute* attr) {
    // Generates thunk functions for each attr in the emedded type.
    Class::Ptr clazz = attr->type()->clazz();
    if (clazz->name()->string() == "Object") {
        return;
    }
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        if (f->is_embedded()) {
            Attribute* attr = dynamic_cast<Attribute*>(f.pointer());
            component(attr);
        }
    }
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f.pointer())) {
            stub(func, attr);
        } 
    }
}

void
CodeExpander::functor(Class* clazz) {
    // Generate the @call method for the functor, which contains a switch on
    // the type of the arugment passed to @call method.
    Function::Ptr func = clazz->function(env_->name("@call"));
    Location loc;
    Statement::Ptr stmt;
    String::Ptr fn = func->formals()->next()->name();
    IdentifierRef::Ptr guard(new IdentifierRef(loc, env_->name(""), fn));
    Expression::Ptr arg0(new IdentifierRef(loc, env_->name(""), env_->name("self")));
    Expression::Ptr arg1(new IdentifierRef(loc, env_->name(""), fn));
    arg0->type(func->formals()->type());
    arg1->type(func->formals()->next()->type());

    Expression::Ptr arg = 0;
    arg = append(arg.pointer(), arg0.pointer());
    arg = append(arg.pointer(), arg1.pointer());

    for (Feature::Ptr feat = clazz->features(); feat; feat = feat->next()) {
        if (Function* func = dynamic_cast<Function*>(feat.pointer())) {
            String* nm = func->name();
            if (nm->string().find("@case") == 0) {
                // This is a functor case, so generate a branch for it.  Each 
                // branch looks like this: self.@case_Type(obj)
                Type::Ptr type = func->formals()->next()->type();
                IdentifierRef::Ptr id(new IdentifierRef(loc, env_->name(""), nm));
                Call::Ptr expr(new Call(loc, id, arg));
                expr->function(func);
                Statement::Ptr yes(new Simple(loc, expr));
                Is::Ptr is(new Is(loc, guard, type));
                stmt = new Conditional(loc, is, yes, stmt);
            }
        }
    }
    func->block(new Block(loc, env_->string(""), stmt));  
}

