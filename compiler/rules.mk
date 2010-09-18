sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

OBJS_$(d) := $(d)/main.o $(d)/aptype.o $(d)/apvar.o $(d)/apexpr.o \
	 $(d)/apfunc.o $(d)/apstmt.o $(d)/apparser.o $(d)/apunit.o \
	 $(d)/apimport.o $(d)/apdef.o $(d)/aphash.o $(d)/apsymtab.o \
	 $(d)/apgrammar.y.o $(d)/apgrammar.l.o $(d)/apcgen.o $(d)/aploc.o \
	 $(d)/apchecker.o
DEPS_$(d) := $(OBJS_$(d):%=%.d)

CLEAN := $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d)) $(d)/apollo $(d)/apgrammar.y.c \
	$(d)/apgrammar.l.c $(d)/apgrammar.y.output $(d)/apgrammar.h

$(OBJS_$(d)): CFLOCAL := -I$(d)

TARGETS := $(TARGETS) $(d)/apollo
APGRAMMAR := $(d)/apgrammar.h

$(d)/apollo: $(OBJS_$(d))
	$(CC) -g -o $@ $^

$(d)/apgrammar.y.c: $(d)/grammar.y
	bison -v --defines=$(APGRAMMAR) --output=$@ $^

$(d)/apgrammar.l.c: $(d)/grammar.l
	flex --outfile=$@ $^

d := $(dirstack_$(sp))
sp := $(basename $(sp))

