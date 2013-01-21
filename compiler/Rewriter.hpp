/*
 * Copyright (c) 2011 Matt Fichman
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


#include "Jogo.hpp"
#include "Environment.hpp"

/* Records the location of something that must be replaced */
class Rewrite {
    std::string replacement;
    Location location;
};

/* Rewrites the source file */
class Rewriter : public TreeNode::Functor {
public:
    Rewriter(Environment* env);
    void name(std::string const& name) { name_ = name; }
    void replacement(std::string const& repl) { replacement_ = repl; }
    void rewrite();
    typedef Pointer<Rewriter> Ptr;
private:
    Environment::Ptr env_;
    std::string name_; // Name of the variable/class/to replace
    std::string replacement_;
    std::vector<Rewrite> loc_;
    TreeNode::Ptr node_;
     
};
