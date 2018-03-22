all: invertedIndex
invertedIndex: invertedIndex.c
	gcc -Werror -Wall -g -fsanitize=address invertedIndex.c -o invertedIndex
clean:
	rm -rf invertedIndex
