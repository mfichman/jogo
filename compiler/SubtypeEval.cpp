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

#include "SubtypeEval.hpp"

SubtypeEval::SubtypeEval(Type const* self, Type const* sub) :
    env_(self->env()), 
    self_(self),
    sub_(sub) {
}

SubtypeEval::operator bool() {
    // Returns true if 'sub' is a subtype of 'self'
    if (self_->is_void() && sub_->is_void()) { return true; }
    // Void is a subtype of itself
    if (self_->is_void() || sub_->is_void()) { return false; }
    // Void is not a subtype of anything; nothing is a subtype of void
    if (sub_->is_bottom() || self_->is_bottom()) { return false; }
    // Bottom type is not assignable to anything
    if (sub_->is_union() && self_->is_any()) { return false; }
    // Union is not a subtype of Any
    if (sub_->is_any() || self_->is_any()) { return true; }
    // Any is a supertype of everything
    if (sub_->is_top() || self_->is_top()) { return true; }
    // Top is a supertype of everything
    if (!sub_->is_value() && self_->is_nil()) { return true; }
    // Nil is assignable to anything but a value type
	if (sub_->is_primitive() && self_->is_nil()) { return true; }
    // Nil is assignable to primitives
    if (sub_->clazz() == self_->clazz()) { 
        return self_->equals(sub_);
    }
    if (!sub_->clazz() || !sub_->clazz()) { return false; }
    // No classes available for these types; assume false
    return subtype();
}

bool SubtypeEval::subtype() {
    // If 'this' is a union type, then it will be a subtype of 'other' if
    // 'other' is listed as an alternate.  Likewise, 'this' will be a subtype
    // of 'other' if 'other' is an alternate and 'this' is listed as an
    // alternate of 'other'.
    Class const* self = self_->clazz();
    Class const* sub = sub_->clazz();
	assert(self&&sub&&"No types in subtype evaluation");
    SubtypeResult res = env_->subtype(self_, sub_);
    if (res == YES) { return true; }
    if (res == NO) { return false; }
    if (self->alternates()) {
        for (Type::Ptr alt = self->alternates(); alt; alt = alt->next()) {
            if (alt->clazz() == sub) { 
                env_->subtype(self_, sub_, YES);
                return true;
            }
        }
        env_->subtype(self_, sub_, NO);
        return false;
    }
    if (sub->alternates()) {
        for (Type::Ptr alt = sub->alternates(); alt; alt = alt->next()) {
            if (alt->clazz() == self) {
                env_->subtype(self_, sub_, YES);
                return true;
            }
        }
        env_->subtype(self_, sub_, NO);
        return false;
    }
    if (!sub->is_interface()) { return self == sub; }

    // Assume that this type is a subtype of 'other.'  This will break cycles
    // caused when checking function return values for covariance.
    env_->subtype(self_, sub_, YES);
    for (Feature* f = sub->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f)) {
            Function* mine = self->function(func->name());   
            if (!mine || !covariant(mine, func)) {
                // Now we know that the types are not compatible, so mark
                // 'other' as a disjoint type.
                env_->subtype(self_, sub_, NO);
                return false;
            }
        }
    }
    return true;
}

bool SubtypeEval::covariant(Function const* self, Function const* func) {
    // Returns true if this function is covariant, i.e., its return type is the
    // same as or a subtype of 'other.'  The arguments must match exactly.
    Formal* f1 = self->formals();
    Formal* f2 = func->formals();

    while (f1 && f2) {
        if (f1->is_self() && f2->is_self()) {
            /* pass */
        } else {
           Type* t1 = self_->generic(f1->type());
           Type* t2 = sub_->generic(f2->type());
           if (!t1->equals(t2)) {
               return false;
           }
        }
        f1 = f1->next();
        f2 = f2->next();
    }
    if (f1 || f2) {
        return false;
    }
    Type::Ptr ret1 = self->type()->canonical(self_);
    Type::Ptr ret2 = func->type()->canonical(sub_);
    if (!ret1->subtype(ret2)) {
        return false;
    } 
    return true;
}
