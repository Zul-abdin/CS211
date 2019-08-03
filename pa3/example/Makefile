all:	example

example:	example.c
	gcc -Wall -Werror -fsanitize=address -std=gnu99 example.c -lm -o example
clean:
	rm -rf example
