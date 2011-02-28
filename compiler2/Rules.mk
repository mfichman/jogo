sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

OBJS_$(d) := $(d)/Parser.o $(d)/Grammar.o $(d)/Lexer.o \
    $(d)/Unit.o $(d)/Type.o $(d)/Environment.o $(d)/TypeChecker.o \
    $(d)/Printer.o $(d)/Location.o

DEPS_$(d) := $(OBJS_$(d):%=%.d)

CLEAN := $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d)) $(d)/main $(d)/Grammar.cpp \
	$(d)/Lexer.cpp $(d)/Grammar.output $(d)/Grammar.hpp $(d)/*.o

$(OBJS_$(d)): CFLOCAL := -I$(d)

TARGETS := $(TARGETS) $(BINARIES)/apollo $(BINARIES)/test
GRAMMAR := $(d)/Grammar.hpp

$(BINARIES)/apollo: $(d)/Main.o $(OBJS_$(d))
	$(CPP) -g -o $@ $^

$(BINARIES)/test: $(d)/Test.o $(OBJS_$(d))
	$(CPP) -g -o $@ $^

$(d)/Grammar.cpp: $(d)/Grammar.y
	bison -v --defines=$(GRAMMAR) --output=$@ $^

$(d)/Lexer.cpp: $(d)/Lexer.l
	flex --outfile=$@ $^

d := $(dirstack_$(sp))
sp := $(basename $(sp))
