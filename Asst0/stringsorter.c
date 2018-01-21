#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	//Catches invalid inputs and prints the input format
	if(agrc > 2){
	    printf("Program must take one string parameter. Like so:\n");
		printf("./stringsorter \"Example string here\"\n");
		exit(1);
	}

    //Copies the value at argv to variable s
	char s[strlen(argv[1])];
    char *iter = *s;
	strcpy(s, argv[1]);

	while(*iter != '\0'){
		while(/*Non-letter chars*/){
		}
	}

    printf("%s\n", s);
	exit(0);
}
