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
    env_(env),
    class_(0),
    qualified_name_(qn),
    is_top_(false) {

    if (qn->string()[0] != ':') {
       // Compute the scope and name of the type by splitting on the '::' token.
       size_t scope_end = qn->string().find_last_of(':');
       if (scope_end == std::string::npos) {
           name_ = qn;
           scope_ = env_->name("");
       } else {
           name_ = env->name(qn->string().substr(scope_end + 1));
           scope_ = env->name(qn->string().substr(0, scope_end - 1));
       }
    } else {
        name_ = qn;
        scope_ = env_->name("");
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
    // Returns true if other is a subtype of 'this'
    if (!other) { 
        return false;
    }
    if (other->is_bottom() || is_bottom()) {
        return false;
    }
    if (other->is_any() || is_any()) {
        return true;
    }
    if (other->is_top() || is_top()) {
        return true;
    }
    if (!other->is_value() && is_nil()) {
        return true;
    }
    if (other->is_nil() && !is_value()) {
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
    return is_object_proto()
        || is_interface_proto()
        || is_enum_proto()
        || is_value_proto()
        || is_alt_proto();
}

bool Type::is_interface_proto() const {
    return equals(env_->interface_type());
}

bool Type::is_enum_proto() const {
    return equals(env_->enum_type());
}

bool Type::is_object_proto() const {
    return equals(env_->object_type());
}

bool Type::is_value_proto() const {
    return equals(env_->value_type());
}

bool Type::is_alt_proto() const {
    return equals(env_->alt_type());
}

bool Type::is_generic() const {
    return !name_->string().empty() && name_->string()[0] == ':';
}

bool Type::is_alt() const {
    return is_alt_proto() || (clazz() && clazz()->is_alt());
}

bool Type::is_interface() const {
    return is_interface_proto() || (clazz() && clazz()->is_interface()); 
}

bool Type::is_object() const {
    return is_object_proto() || (clazz() && clazz()->is_object()); 
}

bool Type::is_value() const {
    return is_value_proto() || (clazz() && clazz()->is_value()); 
}

bool Type::is_enum() const {
    return is_enum_proto() || (clazz() && clazz()->is_enum());
}

bool Type::is_primitive() const {
    return is_number() || is_bool() || is_char() || is_enum();
}

bool Type::is_boolifiable() const {
    return is_top() || !is_value() || is_bool() || is_int();
}

bool Type::is_any() const {
    return equals(env_->any_type()); 
}

bool Type::is_bottom() const { 
    return equals(env_->bottom_type());
}

bool Type::is_nil() const {
    return equals(env_->nil_type()); 
}

bool Type::is_top() const { 
    return is_top_ || equals(env_->top_type()); 
}

bool Type::is_void() const { 
    return equals(env_->void_type());
}

bool Type::is_byte() const {
    return equals(env_->byte_type());
}

bool Type::is_char() const {
    return equals(env_->char_type());
}

bool Type::is_bool() const {
    return equals(env_->bool_type());
}

bool Type::is_self() const {
    return equals(env_->self_type());
}

bool Type::is_int() const {
    return equals(env_->int_type());
}

bool Type::is_float() const {
    return equals(env_->float_type());
}

bool Type::is_number() const { 
    return is_float() || is_int() || is_byte(); 
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
    // Outputs the fully-qualified type for 'type', including all generics, and
    // recursively outputs the fully-qualified name for any generic parameters,
    // or type variables if used.
    if (!type) {
        return out << "<<null>>";
    }   
    
    if (type->scope()->string().empty()) {
        Class* clazz = type->file()->clazz(0, type->name());
        if (clazz) {
            out << clazz->qualified_name();
        } else {
            out << type->name();
        }
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
