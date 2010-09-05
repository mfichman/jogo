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
#include <apparser.h>
#include <apcgen.h>



int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Illegal argument\n");
		return 1;
	}

	FILE *fd = fopen(argv[1], "r");
	apparser_t *parser = apparser_alloc();
	if (!apparser_parse(parser, argv[1], fd)) {

		apcgen_t *cgen = apcgen_alloc();
		apcgen_gen_unit(cgen, parser->units, stdout);
		apcgen_free(cgen);
	}
	apparser_free(parser);

	fprintf(stderr, "File parsed.\n");
}
