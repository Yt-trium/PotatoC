EXEC=PotatoC

CC=gcc
LEX=lex
YAC=yacc -d
CFLAGS=-O2 -Wall
LDFLAGS=-ly -lfl

BIN_DIR=./bin
OBJ_DIR=./obj
SRC_DIR=./src
INC_DIR=./inc
LEX_DIR=./lex
YAC_DIR=./yacc

SRC=ast.c
OBJ=$(SRC:.c=.o)

all: yacc lex
	
yacc: $(YAC_DIR)/y.tab.c
	$(YACC) $(YAC_DIR)/$(EXEC).y
lex: $(LEX_DIR)/lex.yy.c
	$(LEX) $(EXEC).l

y.tab.c: $(EXEC).y
	$(YACC) $(EXEC).y

lex.yy.c: $(EXEC).l
	$(LEX) $(EXEC).l

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: all
	./bin./$(EXEC)