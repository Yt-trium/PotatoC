# Need to set correct flag depending on the os
# -lll for lex on Os X

LFLAGS= -ly -lfl		    # Lex linking flags
CFLAGS= -Wall -Iincludes -Ibuild    # C Flags
C_COMPILER = gcc		    # C compiler
EXS = bin/parser bin/expr bin/calc  # Final output files

DIR = build bin			    # Folders to create
COMMON_DEPS = $(SOURCES) $(HEADERS)

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard includes/*.h)

# Main

all: $(DIR) build/cpotato.o $(EXS)

# Directories

$(DIR):
	mkdir -p $(DIR)

# C Library 

build/cpotato.o: $(SOURCES) $(HEADERS)
	$(C_COMPILER) $(CFLAGS) -c $(SOURCES) -o $@

# Parser

bin/parser: build/cpotato.o yacc/parser.y lex/parser.l
	yacc -v -d yacc/parser.y -o build/y.parser.c
	flex -o build/parser.yy.c lex/parser.l
	$(C_COMPILER) $(CFLAGS) build/y.parser.c build/parser.yy.c build/cpotato.o $(LFLAGS) -o $@

# Expr

bin/expr: build/cpotato.o yacc/expr.y lex/expr.l
	yacc -v -d yacc/expr.y -o build/y.expr.c
	flex -o build/expr.yy.c lex/expr.l
	$(C_COMPILER) $(CFLAGS) build/y.expr.c build/expr.yy.c build/cpotato.o $(LFLAGS) -o $@

# Calc

bin/calc: build/cpotato.o yacc/calc.y lex/calc.l
	yacc -v -d yacc/calc.y -o build/y.calc.c
	flex -o build/calc.yy.c lex/calc.l
	$(C_COMPILER) $(CFLAGS) build/y.calc.c build/calc.yy.c build/cpotato.o $(LFLAGS) -o $@

clean:
	rm -rif *.o y.*.c y.*.o y.*.h *.yy.c a.out $(EXS) y.*.output build
