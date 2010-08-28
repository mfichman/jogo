OBJS=main.o aptype.o apvar.o apexpr.o apfunc.o apstmt.o apparser.o apunit.o \
	 apimport.o apdef.o aphash.o apsymtab.o apgrammar.y.o apgrammar.l.o
CFLAGS=-std=c99 -c -g -I. -Wall -Werror -pedantic
CC=clang


apollo: $(OBJS)
	$(CC) -g -o $@ $^ 

clean:
	rm -rf *.o apollo apgrammar.h apgrammar.y.c apgrammar.l.c


apgrammar.y.c: grammar.y
	bison --defines=apgrammar.h --output=$@ $^

apgrammar.l.c: grammar.l
	flex --outfile=$@ $^


%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^ 
