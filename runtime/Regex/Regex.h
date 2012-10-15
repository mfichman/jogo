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

#ifndef APOLLO_REGEX_REGEX_H
#define APOLLO_REGEX_REGEX_H

#include "Primitives.h"

typedef enum InstrType InstrType;
enum InstrType { CHAR, MATCH, JUMP, SPLIT, ANY };

typedef struct Regex_Instr* Regex_Instr;
struct Regex_Instr {
    enum InstrType type;
    Int target;
    Int gen;
};

typedef struct Regex_Regex* Regex_Regex;
struct Regex_Regex {
    Ptr _vtable;
    U64 _refcount;
    Char* next;
    Int gen;
    Int length;
    Int capacity;
    struct Regex_Instr instr[];    
};

typedef struct Regex_Thread* Regex_Thread;
struct Regex_Thread {
    Regex_Instr pc;
};

typedef struct Regex_ThreadList* Regex_ThreadList;
struct Regex_ThreadList {
    Regex_Regex regex;
    Int length;
    struct Regex_Thread thread[]; 
};

typedef Object Regex_Match;

Regex_Match Regex_Match__init();
Regex_Regex Regex_Regex_alloc(Int capacity);
Regex_Regex Regex_Regex__init(String str);
Regex_Match Regex_Regex__match(Regex_Regex self, String str);
Regex_Regex Regex_Regex_append(Regex_Regex self, Regex_Instr instr);
Regex_Regex Regex_Regex_parse_char(Regex_Regex self);
Regex_Regex Regex_Regex_parse_concat(Regex_Regex self);
Regex_Regex Regex_Regex_parse_closure(Regex_Regex self);
Regex_Regex Regex_Regex_parse_opt(Regex_Regex self);
Regex_Regex Regex_Regex_parse_class(Regex_Regex self);
Regex_Regex Regex_Regex_parse_alt(Regex_Regex self);
Regex_Regex Regex_Regex_parse_plus(Regex_Regex self);
Regex_Regex Regex_Regex_parse_char(Regex_Regex self);
extern Int Regex_Regex__vtable[];


#endif
