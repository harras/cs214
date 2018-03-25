#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include "invertedIndex.h"



char wordHolder[100];
char buffer[100];
char buffer2[100];
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
	DIR* checkDir=opendir(inputString);
	//Case: not a directory, must be a file
	if(checkDir==NULL){
		valid=processFile(inputString,inputString);
	}
	else{
		closedir(checkDir);
		processDir(inputString);
	}
	//prints output
	if(valid!=0){
		response=saveInvertedIndex(outputFile);
		if(response==0){
			return(0);
		}
	}
	else{
		printf("\nerror: invalid input file\n");
	}
	return(0);
}
//custom version of builtin strcmp. Returns 0 if equal, -1 if str1 comes first, 1 is str2 comes first
int customStrCmp(char* str1, char* str2){
	if(str1==NULL || str2==NULL){
		//not sure what to do here but I needed some form of error handling
		return 0;
	}
	int strIndex=0;
	while(str1[strIndex]!='\0' && str2[strIndex]!='\0'){
		if(str1[strIndex]==str2[strIndex]){
			++strIndex;
		}
		else{
			//case: str1[index] is a letter
			if(str1[strIndex]>='a' && str1[strIndex]<='z'){
				if(str2[strIndex]>='a' && str2[strIndex]<='z'){
					if(str1[strIndex]>str2[strIndex]){
						return 1;
					}
					//case: str1[index] is a character that comes before str2[index]
					else return -1;
				}
				if((str2[strIndex]>='0' && str2[strIndex]<='9') || str2[strIndex]=='.')
					return -1;
			}
			//case: str1 is a number
			else if(str1[strIndex]>='0' && str1[strIndex]<='9'){
				//case: both are numbers, compare
				if(str2[strIndex]>='0' && str2[strIndex]<='9'){
					if(str1[strIndex]>str2[strIndex]){
						return 1;
					}
					//case: str1[index] is a character that comes before str2[index]
					else return -1;
				}
				//case: str2[index] is a letter, str1[index] comes before
				if(str2[strIndex]>='a' && str2[strIndex]<='z'){
					return 1;
				}
				//case: str2[index] is a '.', str1[index] comes before
				if(str2[strIndex]=='.'){
					return -1;
				}				
			}
			//case
			else if(str1[strIndex]=='.'){
				//case: str2[index] is a letter or number, str1 must then come before
				if((str2[strIndex]>='0' && str2[strIndex]<='9')||(str2[strIndex]>='a' && str2[strIndex]<='z')){
					return 1;
				}
			}
			//case: str1[index] or str2[index] is an arbitray character
			else{
				return(strcmp(str1,str2));
			}
		}
	}
	//Case: strings are equal and terminate at the same time
	if(str1[strIndex]==str2[strIndex]){
		return 0;
	}
	//Case: string one ended first
	else if(str1[strIndex]=='\0'){
		return -1;
	}
	//Case: string 2 ended first
	return 1;
}

//processes the directories using a depth first search of each subtree
int processDir(char* dirString){
	DIR* dirFD=opendir(dirString);
	struct dirent* currItem;
	//return Val: number of files successfully processed and dumped into the output file
	int returnVal=0;
	int holdVal=0;
	dirNode* currDir=(dirNode*) malloc(sizeof(dirNode));
	char* dirNameHolder;
	char* fileNameHolder;
	currDir->directory=dirFD;
	int sz=strlen(dirString)+1;
	currDir->dirName=(char*) malloc(sz);
	memcpy(currDir->dirName,dirString,sz);
	while(currDir!=NULL){
		//gets next item
		currItem=readdir(currDir->directory);
		//case: exhausted this branch of the folder tree
		if(currItem==NULL){
			closedir(currDir->directory);
			free(currDir->dirName);
			//Case: not done with the entire stack, needs to continue cleaning up
			if(currDir->prevDirNode!=NULL){
				currDir=currDir->prevDirNode;
				free(currDir->nextDirNode);
				currDir->nextDirNode=NULL;				
			}
			//Case: done with entire stack (previous node is null)
			else{
				free(currDir);
				currDir=NULL;
			}
		}
		//Case: something else to process in this branch
		else if(strcmp(currItem->d_name,".")!=0 && strcmp(currItem->d_name,"..")!=0){
			//case: found a nested subdirectory
			if(currItem->d_type==DT_DIR){
				dirNameHolder=(char*) malloc(strlen(currDir->dirName)+strlen(currItem->d_name)+2);
				strcpy(dirNameHolder,currDir->dirName);
				strcat(dirNameHolder,"/");
				strcat(dirNameHolder,currItem->d_name);
				//opens the path name, adds the node to the stack
				currDir->nextDirNode=(dirNode*) malloc(sizeof(dirNode));
				currDir->nextDirNode->prevDirNode=currDir;
				currDir->nextDirNode->directory=opendir(dirNameHolder);
				currDir->nextDirNode->dirName=dirNameHolder;
				currDir=currDir->nextDirNode;
			}
			//case: found a text file to process
			else if(currItem->d_type==DT_REG){
				//makes full file path
				fileNameHolder=(char*) malloc(strlen(currDir->dirName)+2+strlen(currItem->d_name));
				memcpy(fileNameHolder,currDir->dirName,strlen(currDir->dirName)+1);
				strcat(fileNameHolder,"/");
				strcat(fileNameHolder,currItem->d_name);

				holdVal=processFile(fileNameHolder,currItem->d_name);
				returnVal=returnVal+holdVal;
			}
		}
	}
	return(returnVal);
}

int processFile(char* pathName, char* inName){
	char* currChar=(char*) malloc(3*sizeof(char));
	memset(wordHolder,0,50);
	int readVal;
	char tempChar;
	int inFD=open(pathName, O_RDONLY);
	if(inFD<0){
		return(0);
	}
	int notBlank=0;
	while(1){
		int tokenLength=0;
		while(1){
			readVal=read(inFD,currChar,1);	
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
	free(currChar);
	return(notBlank);
}

//function creates and allocates a new word node if none currently exists
wordNode* makeWordNode(char* wordIn,char* fileString){
		wordNode* wordPtr=(wordNode*) malloc(sizeof(wordNode));
		//makes new strings and saves them so I can free the ones I passed (for cases where new nodes aren't being made)
		wordPtr->nodeString=(char*) malloc(strlen(wordIn)+1);
		memcpy(wordPtr->nodeString,wordIn,strlen(wordIn)+1);
		//initializes a branched off file node, sets the count to 1
		wordPtr->nextFile=(fileNode*) malloc(sizeof(fileNode));
		wordPtr->nextFile->count=1;
		//Makes a new string for the file name so I can free the parameter and not run out of RAM
		wordPtr->nextFile->nodeFile=(char*) malloc(strlen(fileString)+1);
		memcpy(wordPtr->nextFile->nodeFile,fileString,strlen(fileString)+1);
		return wordPtr;
}

//Inserts a node corresponding to a token into the hash table
void insertWord(char* insertStr,char* fileName){
	int cmp;
	int key=insertStr[0]-'a';
	wordNode* tempWordPtr=NULL;
	wordNode* wordNodePtr=invIndex[key];
	wordNode* prevWordPtr=invIndex[key];
	//Case: new linkedlist in an empty bucket
	if(wordNodePtr==NULL){
		invIndex[key]=makeWordNode(insertStr,fileName);
		return;
	}
	cmp=customStrCmp(insertStr,wordNodePtr->nodeString);
	//Case: first node matches
	if(cmp==0){
		insertFile(fileName, wordNodePtr);
		return;
	}
	//Case: word comes before first node
	if(cmp==-1){
		tempWordPtr=makeWordNode(insertStr,fileName);
		tempWordPtr->nextWord=wordNodePtr;
		invIndex[key]=tempWordPtr;
		return;
	}
	//Otherwise: finds the node or the spot where the node should be
	while(wordNodePtr->nextWord!=NULL && customStrCmp(insertStr,wordNodePtr->nodeString)==1){
		prevWordPtr=wordNodePtr;
		wordNodePtr=wordNodePtr->nextWord;
	}	
	//Case: found the relevant node
	if(customStrCmp(insertStr,wordNodePtr->nodeString)==0){
		insertFile(fileName,wordNodePtr);
		return;
	}
	//Case: hit the last node but the inserted node comes after
	else if(wordNodePtr->nextWord==NULL){
		wordNodePtr->nextWord=makeWordNode(insertStr,fileName);
		return;
	}
	//Case: inserted at the end of the file
	tempWordPtr=makeWordNode(insertStr,fileName);
	tempWordPtr->nextWord=wordNodePtr;
	prevWordPtr->nextWord=tempWordPtr;
	return;
}

//searches for the file node off the relevant word node, inserts or increments accordingly
void insertFile(char* fileName,wordNode* wnp){
	fileNode* fileNodePtr;	
	fileNode* prevFilePtr;
	fileNode* tempFilePtr;
	fileNodePtr=wnp->nextFile;
	prevFilePtr=NULL;
	//finds the relevant file
	while(fileNodePtr->nextFile!=NULL && customStrCmp(fileName,fileNodePtr->nodeFile)!=0){
		prevFilePtr=fileNodePtr;
		fileNodePtr=fileNodePtr->nextFile;
	}
	//increments the relevant file, adjusts if the counts are rearranged
	if(customStrCmp(fileName,fileNodePtr->nodeFile)==0){
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
	int outFD=open(outName, O_RDONLY, 00666);
	if(outFD!=-1){
		printf("Are you sure you want to overwrite the file %s?\nEnter 1 to proceed, or 0 to terminate\n",outName);
		scanf("%d",&response);
		if(response==0){
			printf("\nterminating execution");
			return(0);
		}
	}
	close(outFD);
	outFD=open(outName, O_RDWR | O_CREAT,00666);
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
				write(outFD,fileNodePtr->nodeFile,strlen(fileNodePtr->nodeFile));
				write(outFD,"\">",2);
				sprintf(buffer,"%d",fileNodePtr->count);
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


