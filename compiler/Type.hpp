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
#include <vector>
#include <map>
#include <iostream>

/* Type object */
class Type : public TreeNode {
public:
    Type(Location loc, String* qn, Generic* gen, Environment* env);
    Generic* generics() const { return generics_; }
    String* qualified_name() const { return qualified_name_; }
    String* name() const { return name_; }
    String* scope() const { return scope_; }
    Type* next() const { return next_; }
    Type* last() const { return last_; }
    Type* generic(String* name) const;
    Class* clazz() const;
    bool equals(Type* other) const;
    bool subtype(Type* other) const;
    bool is_union() const;
    bool is_any() const;
    bool is_bottom() const;
    bool is_nil() const;
    bool is_top() const;
    bool is_void() const;
    bool is_byte() const;
    bool is_char() const;
    bool is_bool() const;
    bool is_self() const;
    bool is_int() const;
    bool is_float() const;
    bool is_number() const;
    bool is_interface() const;
    bool is_object() const;
    bool is_enum() const;
    bool is_value() const;
    bool is_ref() const;
    bool is_compound() const;
    bool is_boolifiable() const;
    bool is_generic() const;
    bool is_proto() const;
    bool is_primitive() const;
    bool is_alt() const;
    bool is_single() const;
    bool is_interface_proto() const;
    bool is_value_proto() const;
    bool is_enum_proto() const;
    bool is_object_proto() const;
    bool is_union_proto() const;
    void is_top(bool flag) { is_top_ = flag; }
    void next(Type* next) { next_ = next; }
    void last(Type* last) { last_ = last; }
    void operator()(Functor* functor) { functor->operator()(this); }
    typedef Pointer<Type> Ptr;

private:
    Pointer<Generic> generics_;
    File* file_;
    Environment* env_;
    String::Ptr scope_;
    String::Ptr name_;
    Type::Ptr next_;
    Type::Ptr last_;
    Class* class_;
    String::Ptr qualified_name_;
    bool is_top_;
};

/* Holder for a generics type parameter */
class Generic : public Object {
public: 
    Generic(Type* type) :
        type_(type) {
    }

    Type* type() const { return type_; }
    Generic* next() const { return next_; }
    Generic* last() const { return last_; }
    void next(Generic* next) { next_ = next; }
    void last(Generic* last) { last_ = last; }
    typedef Pointer<Generic> Ptr;

private:
    Type::Ptr type_;
    Generic::Ptr next_;
    Generic::Ptr last_;
};

Stream::Ptr operator<<(Stream::Ptr out, const Type* type);


