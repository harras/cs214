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
	int malloc_count = 0;
	while(malloc_count < 150){
		int r = rand()%2;
		if(r == 0){
			ptrs[count++] = mymalloc(1, __FILE__, __LINE__);
			malloc_count++;
		}
		else  if(r == 1 && count < 0){
			myfree(ptrs[count--], __FILE__, __LINE__);
		}
		else{
			myfree(ptrs[0], __FILE__, __LINE__);
		}
	} // will this work idk lol
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
	printf("Test C...time elapsed:\t%fms\n", elapsed);


	// E
	gettimeofday(&start, NULL);
	int * int_ptrs[100];
	i = 0;
	while(i < 100){
		int_ptrs[i] = mymalloc(4, __FILE__, __LINE__);
		i++;
	}
	i = 0;
	while(i < 100){
		int r = rand()%100+1;
		int_ptrs[i] = &r;
		i++;
	}
	i = 0;
	while(i < 100){ // To be commented out in final version, an option to test things
		printf("value at ptrs[%d]:\t%d\n", i, *int_ptrs[i]);
		i++;	
	}
	i = 0;
	while(i < 100){
		myfree(int_ptrs[i], __FILE__, __LINE__);
		i++;
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
	printf("Test C...time elapsed:\t%fms\n", elapsed);

	return 0;
}
