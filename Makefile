OBJS=node.o parser.o lexer.o input.o main.o
CFLAGS=-c -I. -Wall -Werror -pedantic
CC=g++

ifdef DEBUG
	CFLAGS += -g -fno-inline
endif

mc: $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -rf *.o mc

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $^ 
