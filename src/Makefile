CC = gcc
CFLAGS = -O2 -Wall
EXEC = test
SRC = ast.c test.c
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: all
	$(EXEC)

clean:
	rm $(EXEC) *.o
