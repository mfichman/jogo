
CFLAGS := -std=c99 -c -g -Wall -Werror -pedantic
CC := clang

# Standard things
.SUFFIXES:
.SUFFIXES: .c .o
all: targets

# Subdirectories
dir=compiler
include $(dir)/rules.mk 
#dir=runtime
#include $(dir)/rules.mk

# General rules
%.o: %.c
	$(CC) $(CFLAGS) $(CFLOCAL) -o $@ $^

.PHONY: targets
targets:	$(TARGETS)

.PHONY: clean
clean:
	rm -f $(CLEAN)

.SECONDARY: clean
