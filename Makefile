CC=gcc
CFLAGS=-Iinclude -Wall
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
BIN=bin/HQshell

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f src/*.o $(BIN)
