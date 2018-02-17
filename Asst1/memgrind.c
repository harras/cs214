
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"


int main(int argc, char* argv[]){
	// A
	struct timeval start, end;
	gettimeofday(&start, NULL);
	void * ptrs[150];
	for(int i = 0; i < 150; i++){
		if(i < 101){	
			printf("\n%d...\n\n", i);
			ptrs[i] = mymalloc(1, __FILE__, __LINE__);
		}
	}
	for(int i = 0; i < 150; i++){
		//printf("%d...\n", i);
		myfree(ptrs[i], __FILE__, __LINE__);
	}
	gettimeofday(&end, NULL);
	printf("Time elapsed: %ldms\n", (end.tv_sec - start.tv_sec));

	return(0);
}
