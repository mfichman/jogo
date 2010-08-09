#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <parser.h>



int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stdout, "Illegal argument\n");
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	parser_t *parser = parser_alloc();
	parser_parse(parser, fd);
	parser_free(parser);

	printf("File parsed.\n");
}
