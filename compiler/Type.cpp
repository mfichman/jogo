/*
 * Copyright (c) 2011 Matt Fichman
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

#include "Type.hpp"
#include "Feature.hpp"
#include "Environment.hpp"
#include <cassert>

Type::Type(Location loc, String* qn, Generic* gen, File* file, Environment* env) :
    TreeNode(loc),
    generics_(gen),
    file_(file),
    environment_(env),
    class_(0),
    qualified_name_(qn) {

    
    if (qn->string()[0] != ':') {
       // Compute the scope and name of the type by splitting on the '::' token.
       size_t scope_end = qn->string().find_last_of(':');
       if (scope_end == std::string::npos) {
           name_ = qn;
           scope_ = environment_->name("");
       } else {
           name_ = env->name(qn->string().substr(scope_end + 1));
           scope_ = env->name(qn->string().substr(0, scope_end - 1));
       }
    } else {
        name_ = qn;
        scope_ = environment_->name("");
    }

    // Add an implicit (qualified) import if it doesn't already exist
    if (!scope_->string().empty() && !is_variable()) {
        file_->feature(new Import(loc, scope_, true));
    }
}

bool Type::equals(Type* other) const {
    /* Make sure the classes are equal */
    if (clazz() != other->clazz()) {
        return false;
    }
    if (other->name() != name()) {
        return false;
    }

    /* Make sure the generic parameters are the same */
    Generic* g1 = generics();
    Generic* g2 = other->generics();
    while (g1 && g2) {
        if (g1->type()->equals(g2->type())) {
            return false;
        }
        
        g1 = g1->next();
        g2 = g2->next();
    }
    return true;
}

bool Type::subtype(Type* other) const {
    if (this == environment_->no_type()) {
        return true;
    }
    if (other == environment_->no_type()) {
        return true;
    }
    if (this->equals(other)) {
        return true;
    }
    if (!clazz() || !other->clazz()) {
        return false;
    }
    if (!clazz()->subtype(other->clazz())) {
        return false;
    }
    /* TODO: Need to fill out template parameters */
    return true;
}

bool Type::is_variable() const {
    return !name_->string().empty() && name_->string()[0] == ':';
}

bool Type::is_primitive() const {
    return is_int() || is_float() || is_bool() || is_char();
}

bool Type::is_interface() const {
    return clazz()->is_interface();
}

bool Type::is_object() const {
    return clazz()->is_object();
}

bool Type::is_value() const {
    return clazz()->is_value();
}

bool Type::is_char() const {
    return this->equals(environment_->char_type());
}

bool Type::is_bool() const {
    return this->equals(environment_->bool_type());
}

bool Type::is_self() const {
    return this->equals(environment_->self_type());
}

bool Type::is_int() const {
    return this->equals(environment_->int_type());
}

bool Type::is_float() const {
    return this->equals(environment_->float_type());
}

bool Type::is_boolifiable() const {
    if (this->equals(environment_->no_type())) {
        return true;
    } else {
        return !is_value() || is_bool() || is_int();
    }
}

bool Type::is_void() const {
    return this->equals(environment_->void_type());
}

Class* Type::clazz() const {
    if (class_) {
        return class_;
    }
    Type* self = const_cast<Type*>(this);
    self->class_ = self->file_->clazz(scope_, name_);
    return self->class_;
}


Stream::Ptr operator<<(Stream::Ptr out, const Type* type) {
    if (!type) {
        return out << "<<notype>>";
    }   
    if (type->scope()->string().empty()) {
        out << type->name();
    } else {
        out << type->scope() << "::" << type->name();
    }
    if (type->generics()) {
        out << '[';
        for (Generic::Ptr g = type->generics(); g; g = g->next()) {
            out << g->type();
            if (g->next()) {
                out << ',';
            }
        }
        out << ']';
    } 
    return out;
}
