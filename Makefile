# Need to set correct flag depending on the os
# -lll for lex on Os X

LFLAGS= -ly -lfl		    # Lex linking flags
CFLAGS= -g -Wall -Iincludes -Ibuild    # C Flags
C_COMPILER = gcc		    # C compiler
EXS = bin/parser bin/expr bin/calc bin/potatoc # Final output files

DIR = build bin			    # Folders to create
COMMON_DEPS = $(SOURCES) $(HEADERS)

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard includes/*.h)
OBJS = $(SOURCES:src/%.c=bin/%.o)

# Main

all: $(DIR) $(OBJS) $(EXS)

# Directories

$(DIR):
	mkdir -p $(DIR)

# C Library 

bin/%.o: src/%.c
	$(C_COMPILER) -o $@ -c $< $(CFLAGS)

# Parser

bin/parser: yacc/parser.y lex/parser.l $(OBJS)
	yacc -v -d yacc/parser.y -o build/y.parser.c
	flex -o build/parser.yy.c lex/parser.l
	$(C_COMPILER) $(CFLAGS) build/y.parser.c build/parser.yy.c $(OBJS) $(LFLAGS) -o $@

# Expr

bin/expr: yacc/expr.y lex/expr.l $(OBJS)
	yacc -v -d yacc/expr.y -o build/y.expr.c
	flex -o build/expr.yy.c lex/expr.l
	$(C_COMPILER) $(CFLAGS) build/y.expr.c build/expr.yy.c $(OBJS) $(LFLAGS) -o $@

# Calc

bin/calc: yacc/calc.y lex/calc.l $(OBJS)
	yacc -v -d yacc/calc.y -o build/y.calc.c
	flex -o build/calc.yy.c lex/calc.l
	$(C_COMPILER) $(CFLAGS) build/y.calc.c build/calc.yy.c $(OBJS) $(LFLAGS) -o $@

# PotatoC

bin/potatoc: yacc/potatoc.y lex/potatoc.l $(OBJS)
	yacc -v -d yacc/potatoc.y -o build/y.potatoc.c
	flex -o build/potatoc.yy.c lex/potatoc.l
	$(C_COMPILER) $(CFLAGS) build/y.potatoc.c build/potatoc.yy.c $(OBJS) $(LFLAGS) -o $@


clean:
	rm -rif *.o ./bin/*.o y.*.c y.*.o y.*.h *.yy.c a.out $(EXS) y.*.output build

test: all
	./bin/potatoc