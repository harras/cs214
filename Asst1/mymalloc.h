#ifndef MYMALLOC
#define MYMALLOC

#include <stdio.h>
#include <stdlib.h>
#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)
void* mymalloc(int memRequest, char* filenmane, int line);
void myfree(void* memptr, char* filename, int line);
void trythis();

#endif //end of MYMALLOC
