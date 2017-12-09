OS:=$(shell uname)
LFLAGS= -ly 	    # Lex linking flags
CFLAGS= -g -Wall -Iincludes -Ibuild    # C Flags
C_COMPILER = gcc		    # C compiler
EXS = bin/potatoc # Final output files

DIR = build bin			    # Folders to create
COMMON_DEPS = $(SOURCES) $(HEADERS)

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard includes/*.h)
OBJS = $(SOURCES:src/%.c=bin/%.o)

# OS Specific
$(info Running on $(OS))
ifeq ($(OS),Darwin)
    LFLAGS += -ll
endif

ifeq ($(OS),Linux)
    LFLAGS += -lfl
endif

# Main

all: $(DIR) $(OBJS) $(EXS)

# Directories

$(DIR):
	mkdir -p $(DIR)

# C Library 

bin/%.o: src/%.c
	$(C_COMPILER) -o $@ -c $< $(CFLAGS)

# PotatoC

bin/potatoc: yacc/potatoc.y lex/potatoc.l $(OBJS)
	yacc -v -d yacc/potatoc.y -o build/y.potatoc.c
	flex -o build/potatoc.yy.c lex/potatoc.l
	$(C_COMPILER) $(CFLAGS) build/y.potatoc.c build/potatoc.yy.c $(OBJS) $(LFLAGS) -o $@


clean:
	rm -rif *.o ./bin/*.o y.*.c y.*.o y.*.h *.yy.c a.out $(EXS) y.*.output build

test: all
	./bin/potatoc
