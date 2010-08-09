#include <parser.h>
#include <unistd.h>


parser_t *parser_alloc() {
	parser_t *self = malloc(sizeof(parser_t));

	self->units = 0;
	yylex_init(&self->scanner);
	yyset_extra(self, self->scanner);

	return self;
}

void parser_parse(parser_t *self, int fd) {
	self->fd = fd;
	yyparse(self, self->scanner);
}

int parser_read(parser_t *self, char *buffer, size_t length) {
	return read(self->fd, buffer, length);
}

void parser_class(parser_t *self, unit_t *unit) {
}

void parser_interface(parser_t *self, unit_t *unit) {
}

void parser_struct(parser_t *self, unit_t *unit) {
}

void parser_module(parser_t *self, unit_t *unit) {
}

void parser_free(parser_t *self) {
	if (self) {
		yylex_destroy(self->scanner);
		free(self);
	}
}

