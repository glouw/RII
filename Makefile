CFLAGS = -Wall -Wextra -Wpedantic -g -fsanitize=address -fsanitize=undefined

BIN = rr

CC = gcc -std=c99

all:
	$(CC) src/main.c $(CFLAGS) -I ctl/ctl -o $(BIN) && ./$(BIN)

clean:
	rm -f $(BIN)
