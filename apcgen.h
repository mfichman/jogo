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

#ifndef APCGEN_H
#define APCGEN_H

#include <apollo.h>
#include <stdio.h>

/* Code generator for the 'C' target */

apcgen_t *apcgen_alloc();
void apcgen_gen_unit(apcgen_t *self, apunit_t *unit, FILE *fd);
void apcgen_gen_func(apcgen_t *self, apfunc_t *func);
void apcgen_gen_stmt(apcgen_t *self, apstmt_t *stmt);
void apcgen_gen_expr(apcgen_t *self, apexpr_t *expr);
void apcgen_gen_var(apcgen_t *self, apvar_t *var);
void apcgen_print_type(apcgen_t *self, aptype_t *type);
void apcgen_print(apcgen_t *self, const char* fmt, ...);
void apcgen_free(apcgen_t *self);

#endif
