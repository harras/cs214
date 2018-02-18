
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"


int main(int argc, char* argv[]){
	int i;
	struct timeval start, end;
	double elapsed;
	srand(time(NULL));


	// A
	gettimeofday(&start, NULL);
	for(i = 0; i < 150; i++){
		void * ptr = mymalloc(1, __FILE__, __LINE__);
		myfree(ptr, __FILE__, __LINE__);
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
	elapsed += (end.tv_usec - start.tv_usec) / 1000.0; // idk why but this is always 0.
	printf("Test A...time elapsed:\t%fms\n", elapsed);


	// B
	gettimeofday(&start, NULL);
	void * ptrs[150];
	for(i = 0; i < 150; i++){
		//printf("malloc...%d...\n", i);
		ptrs[i] = mymalloc(1, __FILE__, __LINE__);
	}
	for(i = 0; i < 150; i++){
		//printf("free...%d...\n", i);
		myfree(ptrs[i], __FILE__, __LINE__);
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
	elapsed += (end.tv_usec - start.tv_usec) / 1000.0; 
	printf("Test B...time elapsed:\t%fms\n", elapsed);


	// C
	gettimeofday(&start, NULL);
	int count = 0;
	for(i = 0; i < 150; i++){
		int r = rand()%2;
		//if(r == 0){
		//	ptrs[count++] = mymalloc(1, __FILE__, __LINE__);
		//}
		
		void * hi = mymalloc(1, __FILE__, __LINE__);
		myfree(hi, __FILE__, __LINE__);
		myfree(hi, __FILE__, __LINE__);	//should have error pintout here

		/*else*/  if(r == 1 && count < 0){
			myfree(ptrs[count--], __FILE__, __LINE__);
		}
		else{
			myfree(ptrs[0], __FILE__, __LINE__);
		}
	} // will this work idk lol
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
	elapsed += (end.tv_usec - start.tv_usec) / 1000.0; 
	printf("Test C...time elapsed:\t%fms\n", elapsed);
	return 0;
}
