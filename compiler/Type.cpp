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

Type::Type(Location loc, String* qn, Generic* gen, Environment* env) :
    TreeNode(loc),
    generics_(gen),
    environment_(env),
    class_(0),
    qualified_name_(qn),
    is_no_type_(false) {

    
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
    if (!scope_->string().empty() && !is_generic()) {
        file()->feature(new Import(loc, env, scope_, true));
    }
}

Type* Type::generic(String* name) const {
    // Returns the generic with type 'name'.  This method works by looking
    // through the generic class definition and returning the type that is
    // bound to generic 'name' in the current instantiation of the type.
    Generic::Ptr gen1 = generics();
    Generic::Ptr gen2 = clazz()->type()->generics();

    while (gen1 && gen2) {
        if (gen2->type()->name() == name) {
            return gen1->type();
        }
        gen1 = gen1->next();
        gen2 = gen2->next();
    }
    assert(!"not found");
    return 0;
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
    if (other->is_bottom_type() || is_bottom_type()) {
        return false;
    }
    if (other->is_any_type() || is_any_type()) {
        return true;
    }
    if (is_no_type()) {
        return true;
    }
    if (other->is_no_type()) {
        return true;
    }
    if (!other->is_value() && is_nil_type()) {
        return true;
    }
    if (other->is_nil_type() && !is_value()) {
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

bool Type::is_proto() const {

    std::string qn = qualified_name()->string();
    if (qn == "Object") {
        return true;
    }
    if (qn == "Value") {
        return true;
    }
    if (qn == "Interface") {
        return true;
    }
    if (qn == "Mixin") {
        return true;
    }
    if (qn == "Enum") {
        return true;
    }
    return false;
}

bool Type::is_no_type() const {
    return is_no_type_ || this == environment_->no_type();
}

bool Type::is_bottom_type() const {
    return this == environment_->bottom_type();
}

bool Type::is_nil_type() const {
    return this == environment_->nil_type();
}

bool Type::is_any_type() const {
    return this->equals(environment_->any_type());
}

bool Type::is_alt_type() const {
    return is_any_type() || (clazz() && clazz()->alternates());
}

bool Type::is_generic() const {
    return !name_->string().empty() && name_->string()[0] == ':';
}

bool Type::is_primitive() const {
    if (is_int() || is_float() || is_bool() || is_char() || is_byte()) {
        return true;
    }
    return is_enum();
}

bool Type::is_interface() const {
    std::string qn = qualified_name()->string();
    if (qn == "Interface") {
        return true;
    }
    return clazz() && clazz()->is_interface();
}

bool Type::is_object() const {
    std::string qn = qualified_name()->string();
    if (qn == "Object") {
        return true;
    }
    return clazz() && clazz()->is_object();
}

bool Type::is_value() const {
    std::string qn = qualified_name()->string();
    if (qn == "Value") {
        return true;
    }
    return !is_generic() && clazz() && clazz()->is_value();
}

bool Type::is_enum() const {
    std::string qn = qualified_name()->string();
    if (qn == "Enum") {
        return true;
    }
    return clazz() && clazz()->is_enum();
}

bool Type::is_byte() const {
    return this->equals(environment_->byte_type());
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
    if (is_no_type()) {
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
    self->class_ = self->file()->clazz(scope_, name_);
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
