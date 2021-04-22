all: 
	gcc main.c -Wall -Wextra -Wpedantic -g -fsanitize=address -fsanitize=undefined -I ctl/ctl
