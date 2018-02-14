all: memgrind
memgrind: memgrind.c
	gcc -Werror -Wall -g -fsanitize=address mymalloc.c memgrind.c -o memgrind

clean:
	rm -rf memgrind
