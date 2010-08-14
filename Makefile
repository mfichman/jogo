OBJS=main.o type.o var.o expr.o func.o stmt.o parser.o unit.o op.o \
	 import.o def.o hash.o symtab.o grammar.y.o grammar.l.o
CFLAGS=-std=c99 -c -g -I. -Wall -Werror -pedantic


apollo: $(OBJS)
	$(CC) -g -o $@ $^ 

clean:
	rm -rf *.o apollo grammar.y.c grammar.l.c


grammar.y.c: grammar.y
	bison --defines=grammar.h --output=$@ $^

grammar.l.c: grammar.l
	flex --outfile=$@ $^


%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^ 
