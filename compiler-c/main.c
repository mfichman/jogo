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

#include <stdio.h>
#include <stdlib.h>
#include <apollo.h>
#include <apenv.h>
#include <apparser.h>
#include <apchecker.h>
#include <apimport.h>
#include <aphash.h>
#include <apcgen.h>
#include <apunit.h>



int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Illegal argument\n");
		return 1;
	}

    apenv_t *env = apenv_alloc(argv[1]);
	apparser_t *parser = apparser_alloc();
	apchecker_t *checker = apchecker_alloc();
	apcgen_t *cgen = apcgen_alloc();

    if (!apparser_parse(parser, env)) {
        return 0;
    }
    if (!apchecker_check(checker, env)) {
        return 0;
    }
    if (apcgen_gen(cgen, env)) {
        return 0;
    }
	
	apparser_free(parser);
	apchecker_free(checker);
	apcgen_free(cgen);
    apenv_free(env);
	
	return 0;
}
