/*
 * Copyright (c) 2010 Matt Fichman
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

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "apollo.hpp"
#include "object.hpp"
#include "unit.hpp"
#include "name.hpp"
#include <map>

/* Compilation environment; contains symbol table and compilation units */
class Environment : public Object {
public:
    Name* name(const std::string& str);
    Unit* units() const { return units_; }
    Unit* unit(Name *name) { return unit_[Name::Ptr(name)]; }
    void unit(Unit* unit);
    typedef Pointer<Environment> Ptr;

private:
    std::map<std::string, Name::Ptr> name_;
    std::map<Name::Ptr, Unit::Ptr> unit_;
    Unit::Ptr units_;
};

#endif
