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

	aphash_compfn_t comp = (aphash_compfn_t)&strcmp;
	aphash_hashfn_t hash = (aphash_hashfn_t)&aphash_string;
	aphash_t *types = aphash_alloc(comp, hash);
	apunit_t *queue = apunit_alloc(strdup(argv[1]));
	apunit_t *units = 0;

	apparser_t *parser = apparser_alloc();
	apchecker_t *checker = apchecker_alloc();
	apcgen_t *cgen = apcgen_alloc();
	
	/* Parsing/semantic analysis phase */
	for (apunit_t *unit = queue; unit; unit = queue) {
		queue = queue->next;	

		/* Run the parser to create the translation unit */
		if (apparser_parse(parser, unit)) {
			unit->next = 0;
			apunit_free(unit);
			return 1;
		}
		unit->next = units;
		units = unit;	
	
		/* Add the translation unit to the queue for parsing */
		for (apimport_t *im = unit->imports; im; im = im->next) {	
			apunit_t *unit = aphash_get(types, im->name);
			if (!unit) {
				unit = apunit_alloc(strdup(im->name));	
				unit->next = queue;
				queue = unit;
				aphash_put(types, im->name, unit);
			}
		}
	}

	/* Semantic analysis/code generation phase */
	for (apunit_t *unit = units; unit; unit = unit->next) {
		if (apchecker_check(checker, unit, types)) {
			break;
		}
		if (apcgen_gen(cgen, unit)) {
			break;
		}	
	}

	aphash_free(types);
	apunit_free(units);
	apunit_free(queue);
	apparser_free(parser);
	apchecker_free(checker);
	apcgen_free(cgen);
	
	return 0;
}
