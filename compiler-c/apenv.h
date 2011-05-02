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

#ifndef APENV_H
#define APENV_H

#include <apollo.h>

/* Structure for holding context about the current compilation environment */
struct apenv {
    apunit_t *units; /* List of compilation unit structures */
    aphash_t *types; /* Hash of typenames to compilation unit structures */
    aphash_t *idents; /* Hash of identifiers */
    aphash_t *integers; /* Hash of integer values */
    aphash_t *strings; /* Hash of strings */
    char *root;
};

apenv_t *apenv_alloc(const char *root);
char *apenv_ident(apenv_t *self, const char *ident);
char *apenv_integer(apenv_t *self, const char *integer);
char *apenv_string(apenv_t *self, const char *string);
void apenv_unit(apenv_t *self, apunit_t *unit);
void apenv_free(apenv_t *self);

#endif
