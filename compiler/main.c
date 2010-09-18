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
	apunit_t *queue = apunit_alloc(argv[1]);
	apunit_t *units = 0;

	apparser_t *parser = apparser_alloc();
	apchecker_t *checker = apchecker_alloc();
	apcgen_t *cgen = apcgen_alloc();
	
	for (apunit_t *unit = queue; unit; unit = queue) {
		queue = queue->next;	

		if (apparser_parse(parser, unit)) {
			/* Parsing succeeded */
			unit->next = units;
			units = unit;	
	
			/* Add the translation unit to the queue for parsing */
			for (apimport_t *im = unit->imports; im; im = im->next) {	
				apunit_t *unit = aphash_get(types, im->name);
				if (!unit) {
					unit = apunit_alloc(im->name);	
					unit->next = queue;
					queue = unit;
					aphash_put(types, im->name, unit);
				}
			}

		} else {
			/* Parsing failed */
			unit->next = 0;
			apunit_free(unit);
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

#if 0
	apimport_t *queue; /* Queue of files to parse */
int apparser_parse(apparser_t *self, const char *filename) {
	apparser_unit(self, 0, filename);

	while (self->queue) {
		apimport_t *import = self->queue;
		self->queue = self->queue->next;

		char *filename = malloc(strlen(import->type->name) + strlen(".ap") + 1);
		char *in = import->type->name;
		char *out = filename;
		while (*in) {
			if (*in == ':') {
				*out++ = '/';
				in += 2;
			} else {
				*out++ = *in++;
			}
		}
		*out = 0;
		strcat(out, ".ap");

		apparser_unit(self, import, filename);
        import->next = 0;
		apimport_free(import);
		free(filename);
	}

	for (apunit_t *unit = self->units; unit; unit = unit->next) {
		if (apchecker_check(self->checker, unit, self->types)) {
			self->error++;
		}
	}

	return self->error;
}


	/* Check semantics for the parsed translation units */
	for (apimport_t *import = unit->imports; import; import = import->next) {
		if (!aphash_get(self->types, import->type->name)) {
			apimport_t *next = apimport_clone(import);
			next->next = self->queue;
			self->queue = next;
		}
	}

#endif
