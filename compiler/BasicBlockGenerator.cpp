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

#include "BasicBlockGenerator.hpp"

BasicBlockGenerator::BasicBlockGenerator(Environment* env) :
    environment_(env),
    temporary_(0) {
}

typedef Pointer<BasicBlockGenerator> Ptr; 

void BasicBlockGenerator::operator()(Class* unit) {
}

void BasicBlockGenerator::operator()(Module* unit) {
}

void BasicBlockGenerator::operator()(StringLiteral* expression) {
}

void BasicBlockGenerator::operator()(IntegerLiteral* expression) {
}

void BasicBlockGenerator::operator()(Binary* expression) {
}

void BasicBlockGenerator::operator()(Assignment* expression) {
}

void BasicBlockGenerator::operator()(Unary* expression) {
}

void BasicBlockGenerator::operator()(Call* expression) {
}

void BasicBlockGenerator::operator()(Dispatch* expression) {
}

void BasicBlockGenerator::operator()(Identifier* expression) {
}

void BasicBlockGenerator::operator()(Member* expression) {
}

void BasicBlockGenerator::operator()(Block* statment) {
}

void BasicBlockGenerator::operator()(Simple* statment) {
}

void BasicBlockGenerator::operator()(While* statment) {
}

void BasicBlockGenerator::operator()(For* statment) {
}

void BasicBlockGenerator::operator()(Conditional* statment) {
}

void BasicBlockGenerator::operator()(Variable* statment) {
}

void BasicBlockGenerator::operator()(Return* statment) {
}

void BasicBlockGenerator::operator()(When* statment) {
}

void BasicBlockGenerator::operator()(Case* statment) {
}

void BasicBlockGenerator::operator()(Function* feature) {
}

void BasicBlockGenerator::operator()(Attribute* feature) {
}

void BasicBlockGenerator::operator()(Import* feature) {
}

