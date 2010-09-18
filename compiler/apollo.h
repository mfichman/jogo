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

#ifndef APOLLO_H
#define APOLLO_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#define APSYMBOL_TYPE_FUNC 0
#define APSYMBOL_TYPE_UNIT 1
#define APSYMBOL_TYPE_VAR 2

typedef struct apdef apdef_t;
typedef struct apexpr apexpr_t;
typedef struct aphash aphash_t;
typedef struct apimport apimport_t;
typedef struct apparser apparser_t;
typedef struct apchecker apchecker_t;
typedef struct apstmt apstmt_t;
typedef struct apsymtab apsymtab_t;
typedef struct aptype aptype_t;
typedef struct apunit apunit_t;
typedef struct apvar apvar_t;
typedef struct apfunc apfunc_t;
typedef struct aploc aploc_t;
typedef struct apcgen apcgen_t;

typedef int apsymbol_t;

typedef uint32_t apuint_t;
typedef int32_t apint_t;
typedef uint16_t apushort_t;
typedef int16_t apshort_t;
typedef uint8_t apubyte_t;
typedef int8_t apbyte_t;
typedef uint64_t apulong_t;
typedef int64_t aplong_t; 


static inline char *strdup(const char* str) {
	char *copy = malloc(strlen(str) + 1);
	strcpy(copy, str);
	return copy;
}

#endif
