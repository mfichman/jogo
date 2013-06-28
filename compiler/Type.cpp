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
#include "SubtypeEval.hpp"
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

bool Type::equals(Type const* other) const {
    // Make sure the classes are equal 
    if (clazz() != other->clazz()) {
        return false;
    }
    if (other->name() != name()) {
        return false;
    }

    // Make sure the generic parameters are the same 
    Generic* g1 = generics();
    Generic* g2 = other->generics();
    while (g1 && g2) {
        if (!g1->type()->equals(g2->type())) {
            return false;
        }
        g1 = g1->next();
        g2 = g2->next();
    }
    return true;
}

bool Type::subtype(Type const* other) const {
    // Returns true if 'this' is a subtype of other.
    return SubtypeEval(this, other);
}

bool Type::is_generic() const {
    return !name_->string().empty() && name_->string()[0] == ':';
}

bool Type::is_proto() const {
    return is_object_proto()
        || is_interface_proto()
        || is_enum_proto()
        || is_value_proto()
        || is_union_proto()
        || is_functor_proto();
}

bool Type::is_functor_proto() const {
    return equals(env_->functor_type());
}

bool Type::is_object_proto() const {
    return equals(env_->object_type());
}

bool Type::is_interface_proto() const {
    return equals(env_->interface_type());
}

bool Type::is_enum_proto() const {
    return equals(env_->enum_type());
}

bool Type::is_value_proto() const {
    return equals(env_->value_type());
}

bool Type::is_union_proto() const {
    return equals(env_->union_type());
}

bool Type::is_object() const {
    Class* cls = clazz();
    return is_object_proto() || is_functor()
        || (cls && cls->proto()->is_object_proto()); 
}

bool Type::is_functor() const {
    Class* cls = clazz();
    return is_functor_proto() || (cls && cls->proto()->is_functor_proto());
}

bool Type::is_interface() const {
    Class* cls = clazz();
    return is_interface_proto() || (cls && cls->proto()->is_interface_proto()); 
}

bool Type::is_enum() const {
    Class* cls = clazz();
    return is_enum_proto() || (cls && cls->proto()->is_enum_proto());
}

bool Type::is_value() const {
    Class* cls = clazz();
    return is_primitive() || is_value_proto() 
        || (cls && cls->proto()->is_value_proto()); 
}

bool Type::is_union() const {
    Class* cls = clazz();
    return is_union_proto() || (cls && cls->proto()->is_union_proto());
}

bool Type::is_alt() const {
    return is_union() || is_any();
}

bool Type::is_primitive() const {
    return is_number() || is_bool() || is_char() || is_enum();
}

bool Type::is_ref() const {
    return is_interface() || is_object() || is_alt() || is_generic() || is_nil();
}   

bool Type::is_compound() const {
    return is_value() && !is_primitive();
}

bool Type::is_boolifiable() const {
    return is_top() || is_primitive() || is_ref();
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
    if (is_generic()) {
        class_ = env_->object_type()->clazz();
    } else {
        class_ = file()->clazz(scope_, name_);
    } 
    return class_;
}

bool Type::operator<(Type const& other) const {
    // Compares two types and returns true if this type is less than the other,
    // by doing a name comparison and a recursive comparison of the generics.
    if (qualified_name() != other.qualified_name()) {
        return qualified_name()->string() < other.qualified_name()->string();
    }
    Generic* g1 = generics();
    Generic* g2 = other.generics();
    while (true) {
        if (!g1 && g2) { return true; }
        if (g1 && !g2) { return false; }
        if (!g1 && !g2) { return false; }
        if (!g1->type()->equals(g2->type())) { return *g1->type() < *g2->type(); }
        g1 = g1->next();
        g2 = g2->next();
    }
    return false;
}
//    if (qualified_name()->string() < other.qualified_name()->string()) {
//        return true;
//    }
//    Generic* g1 = generics();
//    Generic* g2 = other.generics();
//    while (g1 || g2) {
//        if (g1 && !g2) { return true; }
//        if (!g1 && g2) { return false; }
//        if (*g1->type() < *g2->type()) { return true; }
//        g1 = g1->next();
//        g2 = g2->next();
//    }
//    return false;
//}

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
