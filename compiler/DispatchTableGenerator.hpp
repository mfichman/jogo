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
#include "Environment.hpp"
#include <vector>

typedef std::vector<Function::Ptr> JumpBucket;

/* Generates the dispatch table for dynamic dispatch */
class DispatchTableGenerator : public TreeNode::Functor {
public:
    DispatchTableGenerator(Class* clazz);
    void operator()(Class* clazz);  
    void operator()(Function* func);

private:
    std::vector<JumpBucket> bucket_;
    Class::Ptr class_;
};

/* Counts the number of functions in a class */
class FunctionCounter : public TreeNode::Functor {
public:
    FunctionCounter(Class* clazz) : class_(clazz), count_(0) {}
    void operator()(Class* clazz);  
    void operator()(Function* func);
    int count() const { return count_; }
private:
    Class::Ptr class_;
    int count_;
};

