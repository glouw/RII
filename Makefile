CFLAGS = -Wall -Wextra -Wpedantic -g #-fsanitize=address -fsanitize=undefined

BIN = rr

CC = gcc -std=c99

all:
	$(CC) rr.c $(CFLAGS) -I ctl/ctl -o $(BIN)

test: all
	./$(BIN) tests/entry.rr
	./$(BIN) examples/fact.rr

clean:
	rm -f $(BIN)
