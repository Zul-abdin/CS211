all:	first

first:	first.c
	gcc -Wall -Werror -fsanitize=address -std=gnu99 first.c -lm -o first
clean:
	rm -rf first
