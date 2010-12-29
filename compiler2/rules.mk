sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

OBJS_$(d) := $(d)/main.o $(d)/parser.o $(d)/grammar.o $(d)/lexer.o
DEPS_$(d) := $(OBJS_$(d):%=%.d)

CLEAN := $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d)) $(d)/apollo $(d)/grammar.cpp \
	$(d)/lexer.cpp $(d)/grammar.output $(d)/grammar.hpp

$(OBJS_$(d)): CFLOCAL := -I$(d)

TARGETS := $(TARGETS) $(d)/apollo
GRAMMAR := $(d)/grammar.hpp

$(d)/apollo: $(OBJS_$(d))
	$(CPP) -g -o $@ $^

$(d)/grammar.cpp: $(d)/grammar.y
	bison -v --defines=$(GRAMMAR) --output=$@ $^

$(d)/lexer.cpp: $(d)/lexer.l
	flex --outfile=$@ $^

d := $(dirstack_$(sp))
sp := $(basename $(sp))
