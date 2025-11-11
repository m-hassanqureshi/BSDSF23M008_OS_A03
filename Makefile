CC = gcc
CFLAGS = -Iinclude -Wall
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin/HQshell
LDFLAGS = -lreadline

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f src/*.o $(BIN)
