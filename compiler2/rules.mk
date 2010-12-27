sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

OBJS_$(d) := $(d)/Main.o $(d)/Parser.o $(d)/Grammar.o $(d)/Lexer.o
DEPS_$(d) := $(OBJS_$(d):%=%.d)

CLEAN := $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d)) $(d)/apollo $(d)/Grammar.cc \
	$(d)/Grammar.cc $(d)/Grammar.output $(d)/Grammar.h

$(OBJS_$(d)): CFLOCAL := -I$(d)

TARGETS := $(TARGETS) $(d)/apollo
GRAMMAR := $(d)/Grammar.h

$(d)/apollo: $(OBJS_$(d))
	$(CPP) -g -o $@ $^

$(d)/Grammar.cc: $(d)/Grammar.y
	bison -v --defines=$(GRAMMAR) --output=$@ $^

$(d)/Lexer.cc: $(d)/Lexer.l
	flex --outfile=$@ $^

d := $(dirstack_$(sp))
sp := $(basename $(sp))
