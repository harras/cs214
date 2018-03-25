typedef struct wordNode wordNode;
typedef struct fileNode fileNode;
typedef struct dirNode dirNode;

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


struct dirNode{
	DIR* directory;
	char* dirName;
	dirNode* prevDirNode;
	dirNode* nextDirNode;
};

int processDir(char* dirString);
<<<<<<< HEAD
int processFile(char* pathName, char* inName);
=======
int processFile(char* inName);
>>>>>>> c63b2f7fad2a891bb5ed63507c9d12e4a2619fe5
void insertWord(char* insertStr,char* fileName);
void insertFile(char* fileName,wordNode* wnp);
int saveInvertedIndex(char* outName);
wordNode* makeWordNode(char* inStr,char* fileString);

