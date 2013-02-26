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

#include "Regex/Regex.h"
#include "Boot/Boot.h"
#include "Object.h"
#include "String.h"
#include <stdio.h>
#include <assert.h>

Regex_ThreadList Regex_ThreadList__init(Regex_Regex re, Int capacity) {
    // Initializes a thread list.  Used by the matcher to track the state of
    // the regex virtual machine.
    Int size = sizeof(struct Regex_Thread)*capacity;
    Regex_ThreadList ret = Boot_calloc(sizeof(struct Regex_ThreadList)+size); 
    ret->regex = re;
    return ret;
}

void Regex_ThreadList_thread(Regex_ThreadList self, Regex_Instr pc) {
    // Adds a thread to the list, but only if the thread isn't on the list
    // already.  This prevents the size of the thread list from becoming 
    // unbounded.
    if (pc->gen == self->regex->gen) {
        return;
    }
    assert(self->length < self->regex->length);
    pc->gen = self->regex->gen;
    self->thread[self->length++].pc = pc;
}


Regex_Regex Regex_Regex_alloc(Int capacity) {
    Int size = sizeof(struct Regex_Instr)*capacity;
    Regex_Regex ret = Boot_calloc(sizeof(struct Regex_Regex)+size);
    ret->_vtable = Regex_Regex__vtable;
    ret->_refcount = 1;
    ret->capacity = capacity;
    return ret;
}

Regex_Regex Regex_Regex__init(String str) {
    // Parses the regular expression, and compiles it into bytecode.
    Regex_Regex ret = Regex_Regex_alloc(512);
    struct Regex_Instr instr = {0};

    ret->next = str->data;
    ret = Regex_Regex_parse_alt(ret);
    instr.type = MATCH;
    instr.target = 0;
    ret = Regex_Regex_append(ret, &instr);
    ret->next = 0;
    assert(ret->instr->gen==0);
    return ret;
}

void Regex_Regex__destroy(Regex_Regex self) {
    Object__refcount_dec((Object)self->error);
    Boot_free(self);
}

Regex_Match Regex_Regex__match(Regex_Regex self, String str) {
    // Runs a mini-vm to determine if the string matches the compiled regular
    // expression.
    Regex_ThreadList cur = Regex_ThreadList__init(self, self->length);
    Regex_ThreadList next = Regex_ThreadList__init(self, self->length);
    Int i = 0;
    Byte* c = 0;
    self->gen++; 

    for (c = str->data;; c++) {
        Regex_ThreadList temp = 0;
        self->gen++; 
        Regex_ThreadList_thread(cur, self->instr);
        for (i = 0; i < cur->length; ++i) {
            Regex_Thread th = cur->thread+i;
            Regex_Instr pc = th->pc;
            switch (pc->type) {
            case MATCH:
                Boot_free(cur);
                Boot_free(next);
                return Regex_Match__init(); 
            case JUMP: 
                Regex_ThreadList_thread(cur, self->instr+pc->target);
                break;
            case SPLIT:
                Regex_ThreadList_thread(cur, self->instr+pc->target);
                Regex_ThreadList_thread(cur, pc+1);
                break;
            case START:
                if (c == str->data) {
                    Regex_ThreadList_thread(cur, pc+1);
                }
                break;
            case END:
                if (c >= str->data+str->length) {
                    Regex_ThreadList_thread(cur, pc+1);
                }
                break;
            case CHAR:
            case ANY:
                break;
            default:
                assert(!"Invalid opcode");
            }
        } 
        self->gen++; 
        for (i = 0; i < cur->length; ++i) {
            Regex_Thread th = cur->thread+i;
            Regex_Instr pc = th->pc;
            switch (pc->type) {
            case CHAR:
                if (*c == pc->target) {
                    Regex_ThreadList_thread(next, pc+1);
                }
                break;
            case ANY:
                Regex_ThreadList_thread(next, pc+1);
                break;
            default:
                break;
            }
        }
        if (!*c) { break; }
        temp = cur;
        cur = next;
        next = temp; 
        next->length = 0;
    }
    Boot_free(cur);
    Boot_free(next);
    return 0;
}

Regex_Regex Regex_Regex_parse_alt(Regex_Regex self) {
    // Parses an alternate expression, e.g., e1|e2, and emits regex interpreter
    // instructions for the expression.  May free 'self.'
    Int l1 = self->length;
    self = Regex_Regex_parse_concat(self);
    while (*self->next == '|') {
        // Modify the instruction sequence to fit instructions for the alt. The
        // instruction seq for e1 needs to be prepended with a SPLIT, and then
        // a JUMP moves from e2 to the end of the block.
        struct Regex_Instr instr = {0};
        Regex_Regex ret = 0;
        Int l2 = self->length;
        Int l3 = 0;
        Int i = 0;

        self->next++;
        self = Regex_Regex_parse_concat(self);
        l3 = self->length;
        ret = Regex_Regex_alloc(Int_max(self->capacity, (l3+2)*2));
        ret->next = self->next;

        // Append the first part of the regex
        for (i = 0; i < l1; ++i) {
            ret->instr[ret->length++] = self->instr[i];
        }

        // Insert a SPLIT instruction to execute the ALT
        instr.type = SPLIT;
        instr.target = l2+2;
        ret->instr[ret->length++] = instr;

        // Append e1, the first expression's instruction block
        for (i = l1; i < l2; ++i) {
            instr = self->instr[i];
            if (SPLIT == instr.type || JUMP == instr.type) {
                instr.target++;
            }
            ret->instr[ret->length++] = instr;
        }  

        // Insert a JUMP instruction
        instr.type = JUMP;
        instr.target = l3+2;
        ret->instr[ret->length++] = instr;

        // Append e2, the second expression's instruction block
        for (i = l2; i < l3; ++i) {
            instr = self->instr[i];
            if (SPLIT == instr.type || JUMP == instr.type) {
                instr.target += 2;
            } 
            ret->instr[ret->length++] = instr;
        }

        Boot_free(self);
        self = ret;
    }
    return self;
}

Regex_Regex Regex_Regex_parse_concat(Regex_Regex self) {
    // Parses a concat expression, e.g., e1e2
    Byte c = 0;
    self = Regex_Regex_parse_closure(self);
    c = *self->next;
    while (c && c != '|' && c != ')') {
        self = Regex_Regex_parse_closure(self);  
        c = *self->next;
    }
    return self;
}

Regex_Regex Regex_Regex_parse_closure(Regex_Regex self) {
    // Parses a closure expression, e.g., e*
    Int l1 = self->length;
    self = Regex_Regex_parse_plus(self);
    while (*self->next == '*') {
        // Modify the instruction seq to fit instructions for the closure.  A
        // SPLIT is prepended to the instruction seq and a JUMP is appended to
        // the end.
        struct Regex_Instr instr = {0}; 
        Regex_Regex ret = 0;
        Int l2 = self->length;
        Int i = 0;
        self->next++;

        ret = Regex_Regex_alloc(Int_max(self->capacity, 2*(l2+1))); 
        ret->next = self->next;

        // Append the first part of the program
        for (i = 0; i < l1; i++) {
            instr = self->instr[i];
            ret->instr[ret->length++] = instr;
        }  

        // Append a SPLIT instruction to execute the closure
        instr.type = SPLIT;
        instr.target = l2+2;
        ret->instr[ret->length++] = instr;

        // Append e1, the first expression's instruction block
        for (i = l1; i < l2; i++) {
            instr = self->instr[i];
            if (SPLIT == instr.type || JUMP == instr.type) {
                instr.target++;
            }
            ret->instr[ret->length++] = instr;
        }  

        // Append a JUMP instruction
        instr.type = JUMP;
        instr.target = l1;
        ret->instr[ret->length++] = instr;
    
        Boot_free(self);
        self = ret; 
    }
    return self;
}

Regex_Regex Regex_Regex_parse_plus(Regex_Regex self) {
    Int l1 = self->length;
    self = Regex_Regex_parse_opt(self);
    while (*self->next == '+') {
        // Modify the instruction seq to fit instructions for the closure.
        // A SPLIT is appended to the instruction seq.
        struct Regex_Instr instr = {0}; 
        // Append a SPLIT instruction to execute the plus
        instr.type = SPLIT;
        instr.target = l1;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    }
    return self;
}

Regex_Regex Regex_Regex_parse_opt(Regex_Regex self) {
    // Parses a optional expression, e.g., e?
    Int l1 = self->length;
    self = Regex_Regex_parse_char(self);
    if (*self->next == '?') {
        // Modify the instruction seq to fit instructions for the opt.
        struct Regex_Instr instr = {0};
        Regex_Regex ret = 0;
        Int l2 = self->length;  
        Int i = 0;
        self->next++;

        ret = Regex_Regex_alloc(Int_max(self->capacity, 2*(l2+1)));
        ret->next = self->next;
    
        // Append the first part of the program
        for (i = 0; i < l1; i++) {
            instr = self->instr[i];
            ret->instr[ret->length++] = instr;
        }

        // Append a SPLIT instruction to execute the closure
        instr.type = SPLIT;
        instr.target = l2+1;
        ret->instr[ret->length++] = instr;

        // Append e1, the first expression's instruction block
        for (i = l1; i < l2; i++) {
            instr = self->instr[i];
            if (SPLIT == instr.type || JUMP == instr.type) {
                instr.target++;
            }
            ret->instr[ret->length++] = instr;
        }  

        Boot_free(self);
        self = ret;
    }
    if (*self->next == '?') {
        static struct String err = String__static("Multiple option");
        err._refcount++;
        self->error = &err;
    }
    return self;
}

Regex_Regex Regex_Regex_parse_char(Regex_Regex self) {
    Byte c = *self->next;
    // Modify the instruction seq to fit instructions for the char.
    if (c == '\\') {
        struct Regex_Instr instr = {0}; 
        self->next++;
        c = *self->next;
        instr.type = CHAR;
        instr.target = c;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    } else if (c == '(') {
        static struct String err = String__static("Unbalanced parenthesis");
        self->next++;
        self = Regex_Regex_parse_alt(self);         
        if (*self->next == ')') {
            self->next++;
        } else {
            err._refcount++;
            self->error = &err;
        }
    } else if (c == ')' || c == '*' || c == '?' || c == '+') {
        // Do nothing
    } else if (c == '.') {
        struct Regex_Instr instr = {0};
        instr.type = ANY;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    } else if (c == '^') {
        struct Regex_Instr instr = {0};
        instr.type = START;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    } else if (c == '$') {
        struct Regex_Instr instr = {0};
        instr.type = END;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    } else if (c) {
        struct Regex_Instr instr = {0}; 
        instr.type = CHAR;
        instr.target = c;
        self = Regex_Regex_append(self, &instr);
        self->next++;
    }
    return self;
}

Regex_Regex Regex_Regex_parse_class(Regex_Regex self) {
    // Parses a character class expression, e.g., [e]
    assert(!"Not implemented");
    return 0;
}

Regex_Regex Regex_Regex_append(Regex_Regex self, Regex_Instr instr) {
    // Appends 'instr' and resizes 'self' if necessary.
    if (self->capacity <= self->length+1) {
        Int oldsize = sizeof(struct Regex_Instr)*self->capacity;
        Int size = sizeof(struct Regex_Instr)*self->capacity*2;
        Regex_Regex ret = Regex_Regex_alloc(sizeof(struct Regex_Regex)+size); 

        self->capacity *= 2;
        Boot_memcpy(ret, self, sizeof(struct Regex_Regex)+oldsize);
        Boot_free(self);
        self = ret;
    }

    // Append a CHAR instruction
    self->instr[self->length++] = *instr;
    return self;
}

String Regex_Regex_error__g(Regex_Regex self) {
    self->error->_refcount++;
    return self->error;
}

void Regex_Regex_dump(Regex_Regex self) {
    // Dump the regex bytecode
    Int i = 0;
    for (i = 0; i < self->length; i++) {
        struct Regex_Instr instr = self->instr[i];
#ifdef LINUX
        printf("%03lx ", i);
        switch (instr.type) {
        case SPLIT: printf("split %03lx\n", instr.target); break;
        case JUMP: printf("jump %03lx\n", instr.target); break;
#else
        printf("%03llx ", i);
        switch (instr.type) {
        case SPLIT: printf("split %03llx\n", instr.target); break;
        case JUMP: printf("jump %03llx\n", instr.target); break;
#endif
        case CHAR: printf("char %c\n", (Byte)instr.target); break;
        case MATCH: printf("match\n"); break;
        case ANY: printf("any\n"); break;
        case START: printf("start\n"); break;
        case END: printf("end\n"); break;
        default: assert(!"Invalid opcode"); break;
        }
    } 
    fflush(stdout);
}

Regex_Match Regex_Match__init() {
    // Initializes a match object on the heap.
    Regex_Match ret = Boot_calloc(sizeof(struct Regex_Match));
    return ret;
}

void Regex_Match_group(Regex_Match self, Int index, Regex_Span ret) {
    if (index < 0 || index > MAXGROUPS) {
        Regex_Span__init(ret);
    } else { 
        *ret = self->group[index];
    }
}
