
CFLAGS := -std=c99 -c -g -Wall -Werror -pedantic
CC := clang
CPPFLAGS := -c -g -Wall -Werror -Wno-sign-compare -pedantic
CPP := g++

# Standard things
.SUFFIXES:
.SUFFIXES: .cc .c .o .l.cc .y.cc
all: targets

# Subdirectories
#dir=compiler
#include $(dir)/rules.mk 
#dir=runtime
#include $(dir)/rules.mk
dir=compiler2
include $(dir)/rules.mk

# General rules
%.o: %.c
	$(CC) $(CFLAGS) $(CFLOCAL) -o $@ $^

%.o: %.cc
	$(CPP) $(CPPFLAGS) $(CFLOCAL) -o $@ $^ 

.PHONY: targets
targets: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(CLEAN)

