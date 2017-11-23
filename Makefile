# Need to set correct flag depending on the os
# -lll for lex on Os X

LFLAGS= -ly -lfl
CFLAGS= -Wall -Iincludes -Ibuild -Isrc -Iyacc -Ilex
C_COMPILER = gcc
DIR = build bin yacc lex includes
EXS = bin/parser bin/expr bin/calc
CFILES = src/symbol.c

all: $(DIR) $(EXS)

$(DIR):
	mkdir -p $(DIR)

# Parser
bin/parser: build/y.parser.o build/lex.parser.o
	$(C_COMPILER) $(CFLAGS) $^ $(LFLAGS) -o $@

build/y.parser.o: yacc/parser.y
	yacc -v -d yacc/parser.y -o build/y.parser.c
	$(C_COMPILER) $(CFLAGS) -c build/y.parser.c -o $@

build/lex.parser.o: lex/parser.l build/y.parser.h
	flex -o build/parser.yy.c lex/parser.l
	$(C_COMPILER) $(CFLAGS) -c build/parser.yy.c $(LFLAGS) -o $@

# Expr
bin/expr: build/y.expr.o build/lex.expr.o
	$(C_COMPILER) $(CFLAGS) $^ $(LFLAGS) -o $@

build/y.expr.o: yacc/expr.y
	yacc -v -d yacc/expr.y -o build/y.expr.c
	$(C_COMPILER) $(CFLAGS) -c build/y.expr.c -o $@

build/lex.expr.o: lex/expr.l build/y.expr.h
	flex -o build/expr.yy.c lex/expr.l
	$(C_COMPILER) $(CFLAGS) -c build/expr.yy.c $(LFLAGS) -o $@

# Calc
bin/calc: build/y.calc.o build/lex.calc.o
	$(C_COMPILER) $(CFLAGS) $^ $(CFILES) $(LFLAGS) -o $@

build/y.calc.o: yacc/calc.y
	yacc -v -d yacc/calc.y -o build/y.calc.c
	$(C_COMPILER) $(CFLAGS) -c build/y.calc.c  -o $@

build/lex.calc.o: lex/calc.l build/y.calc.h
	flex -o build/calc.yy.c lex/calc.l
	$(C_COMPILER) $(CFLAGS) -c build/calc.yy.c $(LFLAGS) -o $@

clean:
	rm -rif *.o y.*.c y.*.o y.*.h *.yy.c a.out $(EXS) y.*.output build
