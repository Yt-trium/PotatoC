# Need to set correct flag depending on the os
# -lll for lex on Os X

LFLAGS= -ly -lfl
CFLAGS= -Wall -Iincludes -Ibuild -Isrc -Iyacc -Ilex
C_COMPILER = gcc
DIR = build bin yacc lex includes
EXS = bin/parser

all: $(DIR) $(EXS)

$(DIR):
	mkdir -p $(DIR)

bin/parser: build/y.parser.o build/lex.parser.o
	$(C_COMPILER) $(CFLAGS) $^ $(LFLAGS) -o $@

build/y.parser.o: yacc/parser.y
	yacc -v -d yacc/parser.y -o build/y.parser.c
	$(C_COMPILER) $(CFLAGS) -c build/y.parser.c -o $@

build/lex.parser.o: lex/parser.l build/y.parser.h
	flex -o build/parser.yy.c lex/parser.l
	$(C_COMPILER) $(CFLAGS) -c build/parser.yy.c $(LFLAGS) -o $@

clean:
	rm -rif *.o y.*.c y.*.o y.*.h *.yy.c a.out $(EXS) y.*.output build
