#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

typedef struct wordNode wordNode;
typedef struct fileNode fileNode;
//typedef struct dirNode dirNode;

int processDir(char* dirString);
int processFile(char* inName);
void insertWord(char* insertStr,char* fileName);
void insertFile(char* fileName,wordNode* wnp);
int saveInvertedIndex(char* outName);



struct wordNode{
	char* nodeString;
	struct wordNode* nextWord;
	struct fileNode* nextFile; 
};

struct fileNode{
	char* nodeFile;
	int count;
	struct fileNode* nextFile;
};

/*struct dirNode{
	char* 
};*/
