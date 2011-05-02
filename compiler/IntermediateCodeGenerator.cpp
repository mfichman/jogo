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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "IntermediateCodeGenerator.hpp"

IntermediateCodeGenerator::IntermediateCodeGenerator(Environment* env) :
    environment_(env),
    temporary_(0) {
}

typedef Pointer<IntermediateCodeGenerator> Ptr; 

void IntermediateCodeGenerator::operator()(Class* unit) {
}

void IntermediateCodeGenerator::operator()(Interface* unit) {
}

void IntermediateCodeGenerator::operator()(Structure* unit) {
}

void IntermediateCodeGenerator::operator()(Module* unit) {
}

void IntermediateCodeGenerator::operator()(StringLiteral* expression) {
}

void IntermediateCodeGenerator::operator()(IntegerLiteral* expression) {
}

void IntermediateCodeGenerator::operator()(Binary* expression) {
}

void IntermediateCodeGenerator::operator()(Assignment* expression) {
}

void IntermediateCodeGenerator::operator()(Unary* expression) {
}

void IntermediateCodeGenerator::operator()(Call* expression) {
}

void IntermediateCodeGenerator::operator()(Dispatch* expression) {
}

void IntermediateCodeGenerator::operator()(Identifier* expression) {
}

void IntermediateCodeGenerator::operator()(Member* expression) {
}

void IntermediateCodeGenerator::operator()(Block* statment) {
}

void IntermediateCodeGenerator::operator()(Simple* statment) {
}

void IntermediateCodeGenerator::operator()(While* statment) {
}

void IntermediateCodeGenerator::operator()(For* statment) {
}

void IntermediateCodeGenerator::operator()(Conditional* statment) {
}

void IntermediateCodeGenerator::operator()(Variable* statment) {
}

void IntermediateCodeGenerator::operator()(Return* statment) {
}

void IntermediateCodeGenerator::operator()(When* statment) {
}

void IntermediateCodeGenerator::operator()(Case* statment) {
}

void IntermediateCodeGenerator::operator()(Function* feature) {
}

void IntermediateCodeGenerator::operator()(Define* feature) {
}

void IntermediateCodeGenerator::operator()(Attribute* feature) {
}

void IntermediateCodeGenerator::operator()(Import* feature) {
}

