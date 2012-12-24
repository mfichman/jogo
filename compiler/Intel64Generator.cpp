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

#include "Intel64Generator.hpp"

Intel64Generator::Intel64Generator(Environment* env) :
    env_(env),
    machine_(Machine::intel64()) {
}

void
Intel64Generator::operator()(File* file) {
    // Output machine code for a single translation unit.
    if (env_->errors()) { return; }

    // FIXME: Output constants
    // FIXME: Output strings

    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this);
    }

    format_->out(out_);
}

void 
Intel64Generator::operator()(Class* feature){
}

void 
Intel64Generator::operator()(Module* feature){
}

void 
Intel64Generator::operator()(Function* feature){
}

void 
Intel64Generator::operator()(IrBlock* block){
}

