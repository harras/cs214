#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node {
	char *str;
	struct Node *next;
};

char * lower_all(char *str){
	for(int i = 0; str[i]; i++){
  		str[i] = tolower(str[i]);
	}
}

int goes_before(char *str1, char *str2){
	printf("RUNNING: goes_before\n");
	printf("str1: %s, str2: %s\n", str1, str2);
	
	while(*str1 != '\0' && *str2 != '\0'){
		printf("str1: %c, str2: %c\n", *str1, *str2);
		if(atoi(str1) < atoi(str2)){
			printf("RETURNING...\n");
			return 1;
		}
		else if(atoi(str1) == atoi(str2)){
			str1++;
			str2++;
		}
		else{
			printf("I don't know what went wrong, fam\n");
			break;
		}
	}
	printf("RETURNING...\n");
	return 0;
}

void insert(struct Node **n_ptr, char *word){
	printf("RUNNING: insert\n");

	struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
	struct Node *temp = *n_ptr;
	
	new_node->str = word;
	new_node->next = NULL;

	if (*n_ptr == NULL){
		printf("1.\n");
		*n_ptr = new_node;
		printf("RETURNING...\n");
		return;
	}

	printf("n_ptr: %s\n", (*n_ptr)->str);

	if(goes_before(new_node->str, (*n_ptr)->str)){
		printf("2.\n");
		new_node->next = (*n_ptr);
		(*n_ptr) = new_node;
		printf("RETURNING...\n");
		return;
	}

	while(temp->next != NULL){
		printf("3.\n");
		if(!goes_before(new_node->str, temp->str) && 
			(goes_before(new_node->str, temp->next->str))){
			new_node->next = temp->next;
			temp->next = new_node;
			printf("RETURNING...\n");
			return;
		}
		temp = temp->next;
	}

	printf("RETURNING UNEXPECTEDLY...\n");
}

void print_list(struct Node *n){
	printf("RUNNING: print_list\n");
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

	// Declares Linked List pointers
	struct Node *head = NULL;

	// Main loop of the program, iterates over the input string and sorts each scanned word
	// NOTE TO SELF: Revise this, it's clunky
	while(*end != '\0'){
		//printf("''''''''''''''''\n");
		//print_list(head);
		//printf("................\n");
		while(!isalpha(*end) && *end != '\0'){
			if(isalpha(*front)){
				memset(word, '\0', sizeof(word));
				memcpy(word, front, (end-front));		
				printf("%s\n", word);
				printf("-----------\n");
				insert(&head, word);
				printf("head: %s", head->str);
				front = end;
			}
			front++;
			end++;
		}
		if(*end != '\0')
			end++;
	}
	if(isalpha(*front)){
		memset(word, '\0', sizeof(word));
		memcpy(word, front, (end-front));
		printf("%s\n", word);
		printf("===========\n");
		insert(&head, word);
	}

	print_list(head);

	return(0);
}
