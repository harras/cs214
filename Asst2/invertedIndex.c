#include "invertedIndex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

char wordHolder[50];
char buffer[20];
char buffer2[20];
wordNode** invIndex; 

int main(int argc, char* argv[]){
	int response;
	if(argc!=3){
		printf("\ninvalid arguments\n");
		return(0);
	}
	char* outputFile=argv[1];
	char* inputString=argv[2];
	int valid=1;

	//initializes Hash Table: current scheme is one bucket for each letter of the alphabet
	//Array of node pointers
	invIndex=(wordNode**) calloc(26,sizeof(wordNode*));
	//Case: directory
	if(inputString[0]=='.' || inputString[0]=='/'){
		valid=processDir(inputString);
	}
	//case: single file
	else{
		valid=processFile(inputString);
	}
	//prints output
	if(valid==1){
		response=saveInvertedIndex(outputFile);
		if(response==0){
			return(0);
		}
	}
	else{
		printf("error");
	}
	return(0);
}

/*int processDir(char* dirString){
	DIR* dirFD=opendir(dirString);
	return(0);
}*/

int processFile(char* inName){
	char* currChar=(char*) malloc(3*sizeof(char));
	memset(wordHolder,0,50);
	ssize_t readVal;
	size_t bytesRead=sizeof(char);
	char tempChar;
	int inFD=open(inName, O_RDONLY);
	if(inFD<0){
		return(0);
	}
	int notBlank=0;
	while(1){
		int tokenLength=0;
		while(1){
			readVal=read(inFD,&currChar,bytesRead);	
			if(readVal>=0){
				if(notBlank==0){
					notBlank=1;
				}
			}
			else{
				break;
			}
			//ensures it is a lowercase letter or if not the first character, a number
			if(currChar[0]>='a' && currChar[0]<='z'){
				tempChar=currChar[0];
				wordHolder[tokenLength]=tempChar;
				++tokenLength;
			}
			else if(currChar[0]>='A' && currChar[0]<='Z'){
				tempChar=currChar[0];
				tempChar=tempChar-'A'+'a';
				wordHolder[tokenLength]=tempChar;
				++tokenLength;
			}
			else if(currChar[0]>='0' && currChar[0]<='9' && tokenLength>0){
				wordHolder[tokenLength]=currChar[0];
				++tokenLength;
			}
			else{
				break;
			}		
		}
		//if the token isn't empty, sets the terminal character and inserts
		if(tokenLength>0){
			wordHolder[tokenLength]='\0';
			insertWord(wordHolder,inName);
			memset(wordHolder,0,50);
		}
		//breaks when it finishes reading the file
		if(readVal<=0){
			break;
		}
	}
	close(inFD);	
	return(notBlank);
}
void insertWord(char* insertStr,char* fileName){
	int cmp;
	wordNode* tempWordPtr;
	int key=insertStr[0]-'a';
	wordNode* wordNodePtr=invIndex[key];
	wordNode* prevWordPtr=NULL;
	//Case: new linkedlist in an empty bucket
	if(wordNodePtr==NULL){
		invIndex[key]=(wordNode*) malloc(sizeof(wordNode));
		//makes new strings and saves them so I can free the ones I passed (for cases where new nodes aren't being made)
		invIndex[key]->nodeString=(char*) malloc(strlen(insertStr)+1);
		memcpy(invIndex[key]->nodeString,insertStr,strlen(insertStr)+1);
		//initializes a branched off file node, sets the count to 1
		invIndex[key]->nextFile=(fileNode*) malloc(sizeof(fileNode));
		invIndex[key]->nextFile->count=1;
		//Makes a new string for the file name so I can free the parameter and not run out of RAM
		invIndex[key]->nextFile->nodeFile=(char*) malloc(strlen(fileName)+1);
		memcpy(invIndex[key]->nextFile->nodeFile,fileName,strlen(fileName)+1);
		return;
	}
	cmp=strcmp(insertStr,wordNodePtr->nodeString);
	//Case: first node matches
	if(cmp==0){
		insertFile(fileName, wordNodePtr);
		return;
	}
	//Case: word comes before first node
	if(cmp==-1){
		tempWordPtr=(wordNode*) malloc(sizeof(wordNode));
		tempWordPtr->nextWord=wordNodePtr;
		tempWordPtr->nodeString=(char*) malloc(strlen(insertStr)+1);
		memcpy(tempWordPtr->nodeString,insertStr,strlen(insertStr)+1);
		//Makes a new file node and set the count to 1
		tempWordPtr->nextFile=(fileNode*) malloc(sizeof(fileNode));
		tempWordPtr->nextFile->count=1;
		tempWordPtr->nextFile->nodeFile=(char*) malloc(strlen(fileName)+1);
		memcpy(tempWordPtr->nextFile->nodeFile,fileName,strlen(fileName)+1);
		invIndex[key]=tempWordPtr;
	}
	//Otherwise: finds the node or the spot where the node should be
	while(wordNodePtr->nextWord!=NULL && strcmp(insertStr,wordNodePtr->nodeString)==1){
		prevWordPtr=wordNodePtr;
		wordNodePtr=wordNodePtr->nextWord;
	}	
	//Case: found the relevant node
	if(strcmp(insertStr,wordNodePtr->nodeString)==0){
		insertFile(fileName,wordNodePtr);
		return;
	}
	//Case: need to insert a new word node between two others
	tempWordPtr=(wordNode*) malloc(sizeof(wordNode));
	tempWordPtr->nextWord=wordNodePtr;
	prevWordPtr->nextWord=tempWordPtr;
	//makes a new string and saves the word entered
	tempWordPtr->nodeString=(char*) malloc(strlen(insertStr)+1);
	memcpy(tempWordPtr->nodeString,insertStr,strlen(insertStr)+1);
	//Makes a new file node, save a copy of the string and set the count to 1
	tempWordPtr->nextFile=(fileNode*) malloc(sizeof(fileNode));
	tempWordPtr->nextFile->count=1;
	tempWordPtr->nextFile->nodeFile=(char*) malloc(strlen(fileName)+1);
	memcpy(tempWordPtr->nextFile->nodeFile,fileName,strlen(fileName)+1);
	return;
}

void insertFile(char* fileName,wordNode* wnp){
	fileNode* fileNodePtr;	
	fileNode* prevFilePtr;
	fileNode* tempFilePtr;
	fileNodePtr=wnp->nextFile;
	prevFilePtr=NULL;
	//finds the relevant file
	while(fileNodePtr->nextFile!=NULL && strcmp(fileName,fileNodePtr->nodeFile)!=0){
		prevFilePtr=fileNodePtr;
		fileNodePtr=fileNodePtr->nextFile;
	}
	//increments the relevant file, adjusts if the counts are rearranged
	if(strcmp(fileName,fileNodePtr->nodeFile)==0){
		fileNodePtr->count=fileNodePtr->count+1;
		//can't just swap the two because you could have clumps of files with a shared count right before
		if(prevFilePtr!=NULL && prevFilePtr->count<fileNodePtr->count){
			tempFilePtr=wnp->nextFile;
			//finds node before where the incremented node should really be
			while(tempFilePtr->nextFile->count>fileNodePtr->count){
				tempFilePtr=tempFilePtr->nextFile;
			}
			//case: overtakes the first one
			if(wnp->nextFile==tempFilePtr && tempFilePtr->count < fileNodePtr->count){
				wnp->nextFile=fileNodePtr;
				prevFilePtr->nextFile=fileNodePtr->nextFile;
				fileNodePtr->nextFile=tempFilePtr;
			}
			//case: switch occurs in the middle
			else{	
				prevFilePtr->nextFile=fileNodePtr->nextFile;
				fileNodePtr->nextFile=tempFilePtr->nextFile;
				tempFilePtr->nextFile=fileNodePtr;
			}
		}
		return;
	}
	//if the file isn't there: inserts a new file node
	//else is redundant, but there for readability
	else{
		fileNodePtr->nextFile=(fileNode*) malloc(sizeof(fileNode));
		fileNodePtr->nextFile->count=1;
		//makes a new string, stores the file name
		fileNodePtr->nextFile->nodeFile=(char*) malloc(strlen(fileName)+1);
		memcpy(fileNodePtr->nextFile->nodeFile,fileName,strlen(fileName)+1);
	}
	return;
}

int saveInvertedIndex(char* outName){
	fileNode* fileNodePtr;
	wordNode* wordNodePtr;
	int bucket;
	int response;
	int outFD=open(outName, O_RDONLY);
	if(outFD!=-1){
		printf("Are you sure you want to overwrite the file %s?\nEnter 1 to proceed, or 0 to terminate",outName);
		scanf("%d",&response);
		if(response==0){
			printf("\nterminating execution");
			return(0);
		}
	}
	outFD=open(outName, O_RDWR | O_CREAT);
	write(outFD, "<?xml version=\"1.0\"encoding=\"UTF-8\"?>\nfileIndex\n",48);
	//cycles through each slot of the hash table
	for(bucket=0; bucket<26; bucket++){
		wordNodePtr=invIndex[bucket];
		//Goes through each word in the bucket
		while(wordNodePtr!=NULL){
			write(outFD,"\t<word text=\"",13);
			write(outFD,wordNodePtr->nodeString,strlen(wordNodePtr->nodeString));
			write(outFD,"\">\n",3);
			fileNodePtr=wordNodePtr->nextFile;
			//iterates through each branching file node and prints
			while(fileNodePtr!=NULL){
				write(outFD,"\t\t<file name=\"",14);
				itoa(fileNodePtr->count,buffer,10);
				write(outFD,fileNodePtr->nodeFile,strlen(fileNodePtr->nodeFile));
				write(outFD,"\">",2);
				itoa(fileNodePtr->count,buffer,10);
				write(outFD,buffer,strlen(buffer));
				write(outFD,"</file>\n",8);
				fileNodePtr=fileNodePtr->nextFile;
			}
			write(outFD,"\t</word>\n",9);
			wordNodePtr=wordNodePtr->nextWord;
		}
	}
	write(outFD,"</fileIndex>",12);
	close(outFD);
	return(1);
}


