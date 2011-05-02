
CFLAGS := -std=c99 -c -g -Wall -Werror -pedantic
CC := clang
CPPFLAGS := -c -g -Wall -Werror -Wno-sign-compare -pedantic
CPP := g++
BINARIES=binaries
TESTS := $(wildcard tests/*.ap)

# Standard things
.SUFFIXES:
.SUFFIXES: .cpp .c .o
all: targets test

# Subdirectories
#dir=compiler
#include $(dir)/rules.mk 
#dir=runtime
#include $(dir)/rules.mk
dir=compiler
include $(dir)/Rules.mk

# General rules
%.o: %.c
	$(CC) $(CFLAGS) $(CFLOCAL) -o $@ $^

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(CFLOCAL) -o $@ $^ 

.PHONY: test
test: $(TESTS)
	scripts/test $(TESTS)
	binaries/test samples/BinaryTree.ap

.PHONY: targets
targets: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(CLEAN)

