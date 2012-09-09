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

class Object;
class TreeNode; 
class Expression;
class StringLiteral;
class NilLiteral;
class IntegerLiteral;
class Cast;
class Box;
class FloatLiteral;
class BooleanLiteral;
class Member;
class File;
class Is;
class Location;
class LocationAnchor;
class Fork;
class Yield;
class Formal;
class String;
class Binary;
class Unary;
class Call;
class HashLiteral;
class ArrayLiteral;
class Construct;
class Identifier;
class ConstantIdentifier;
class Case;
class Simple;
class Block;
class Match;
class Assignment;
class Until;
class While;
class Conditional;
class Return;
class Class;
class Constant;
class Module;
class Feature;
class Function;
class Attribute;
class Import;
class ParseError;
class Environment;
class Type;
class Generic;
class Empty;
class Let;
class LivenessAnalyzer;
class Closure;

#include <Pointer.hpp>
#include <map>
#include <sstream>
#include <string>

template <typename K, typename V>
V query(const std::map<K, V>& map, typename K::Value* str) {
    typename std::map<K, V>::const_iterator i = map.find(str);
    if (i == map.end()) {
        return V();
    } else {
        return i->second;
    }
}

template <typename T>
std::string stringify(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
