SANITIZE = 0

CFLAGS = -Wall -Wextra -Wpedantic -g

ifeq (1, $(SANITIZE))
CFLAGS += -fsanitize=address -fsanitize=undefined
endif

BIN = rr

CC = gcc -std=c99

SRC = rr.c

INC = ctl/ctl

test: all
	./$(BIN) tests/for.rr
	./$(BIN) tests/while.rr
	./$(BIN) tests/entry.rr
	./$(BIN) tests/ref.rr
	./$(BIN) tests/fun.rr
	./$(BIN) tests/if.rr
	./$(BIN) tests/break.rr
	./$(BIN) tests/continue.rr
	./$(BIN) tests/argv.rr testing cmd params
	./$(BIN) tests/continue-bad.rr; test "$$?" -eq 1
	./$(BIN) tests/break-bad.rr; test "$$?" -eq 1
	./$(BIN) examples/fact.rr
	@echo ">> PASS"

all:
	$(CC) $(SRC) $(CFLAGS) -I $(INC) -o $(BIN)

clean:
	rm -f $(BIN)
