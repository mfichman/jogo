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

#ifndef APRUNTIME_H
#define APRUNTIME_H

#include <stdint.h>

typedef struct apobject apobject_t;
typedef struct apiface apiface_t;

typedef uint32_t apuint_t;
typedef int32_t apint_t;
typedef uint16_t apushort_t;
typedef int16_t apshort_t;
typedef uint8_t apubyte_t;
typedef int8_t apbyte_t;
typedef uint64_t apulong_t;
typedef int64_t aplong_t; 

#define apobject_release(object, dtor) {\
	if (object && --(object)->__refcount <= 0) {\
		dtor(object);\
	}\
}

#define apobject_retain(object) {\
	(object)->__refcount++;\
}

struct apobject {
	apushort_t __refcount;
};

typedef void (*apdtor_t)(apobject_t *object);

static inline void *apobject_assign(void **a, void *b, apdtor_t *dtor) {
	apobject_release((apobject_t *)*a);
	*a = b;	
	apobject_retain((apobject_t *)*a);
	return *a;
}

static inline void *apobject_init(void *object) {
	apobject_retain((apobject_t *)*object);
	return object;
}

#endif
