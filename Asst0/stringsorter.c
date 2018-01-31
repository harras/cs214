#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node {
	char *str;
	struct Node *next;
};

struct Node *head = NULL;

char * lower_all(char *str){
	for(int i = 0; str[i]; i++){
  		str[i] = tolower(str[i]);
	}
}

// Returns 1 if a string goes before -- or is the same string as -- another alphabetically.
// BUG: 'hello, hell' returns a 1. 
int goes_before(char *str1, char *str2){
	while(*str1 != '\0' && *str2 != '\0'){
		if((int) *str1 < (int) *str2){
			return 1;
		}
		else if((int) *str1 > (int) *str2){
			return 0;
		}
		else if((int) *str1 == (int) *str2){
			str1++;
			str2++;
		}
		else{
			break;
		}
	}
	return 1;
}

void insert(char *word){

	struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
	struct Node *temp = head;

	new_node->str = malloc(strlen(word) + 1);
	strcpy(new_node->str, word);
	
	new_node->next = NULL;

	if (head == NULL){
		head = new_node;
		return;
	}

	if(goes_before(new_node->str, head->str)){
		new_node->next = head;
		head = new_node;
		return;
	}

	while(temp->next != NULL){
		if(!goes_before(new_node->str, temp->str) && 
			(goes_before(new_node->str, temp->next->str))){
			new_node->next = temp->next;
			temp->next = new_node;
			return;
		}
		temp = temp->next;
	}

	if(goes_before(temp->str, new_node->str)){
		temp->next = new_node;
		return;
	}
}

void print_list(){
	struct Node *n = head;

	while(n != NULL){
		printf("%s\n", n->str);
		n = n->next;
	}
}

int main(int argc, char *argv[]) {
	// Catches invalid inputs and prints the input format
	if(argc != 2){
	    printf("Program must take one string parameter. Like so:\n");
		printf("./stringsorter \"Example string here\"\n");
		return(1);
	}

    // Declares and itializes pointers that scan the input string
	char *front;
    char *end;
    char word[strlen(argv[1])];
	front = argv[1];
	end = argv[1];

	// Main loop of the program, iterates over the input string and sorts each scanned word
	// NOTE TO SELF: Revise this, it's clunky
	while(*end != '\0'){
		while(!isalpha(*end) && *end != '\0'){
			if(isalpha(*front)){
				memset(word, '\0', sizeof(word));
				memcpy(word, front, (end-front));		
				insert(word);
				front = end;
			}
			front++;
			end++;
		}
		if(*end != '\0'){
			end++;
		}
	}
	if(isalpha(*front)){
		memset(word, '\0', sizeof(word));
		memcpy(word, front, (end-front));
		insert(word);
	}

	print_list();

	return(0);
}
