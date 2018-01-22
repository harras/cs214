#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_alpha(char c){
    if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
	    return 1;
	else
		return 0;
}

int main(int argc, char *argv[]) {
	// Catches invalid inputs and prints the input format
	if(argc != 2){
	    printf("Program must take one string parameter. Like so:\n");
		printf("./stringsorter \"Example string here\"\n");
		exit(1);
	}

    //Declares and itializes pointers that scan the input string
	char *front;
    char *end;
    char word[strlen(argv[1])];
	front = argv[1];
	end = argv[1];

	// Main loop of the program, iterates over the input string and sorts each scanned word
	// NOTE TO SELF: Revise this, it's clunky
	while(*end != '\0'){
		while(!is_alpha(*end) && *end != '\0'){
			if(is_alpha(*front)){
				memset(word, '\0', sizeof(word));
				memcpy(word, front, (end-front));		
				printf("%s\n", word);
				front = end;
			}
			front++;
			end++;
		}
		if(*end != '\0')
			end++;
	}
	if(is_alpha(*front)){
		memset(word, '\0', sizeof(word));
		memcpy(word, front, (end-front));
		printf("%s\n", word);
	}

	exit(0);
}
