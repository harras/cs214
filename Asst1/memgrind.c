#include "mymalloc.h"


int main(int argc, char* argv[]){
	int i,x;
	struct timeval start;
	struct timeval end;
	double elapsed;
	char * ptrs[150];
	srand(time(NULL));


	// A
	gettimeofday(&start, NULL);
	for(x=0;x<100;x++){
		for(i = 0; i < 150; i++){
			void * ptr = malloc(1);
			free(ptr);
		}
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_usec - start.tv_usec);
	printf("Test A...Average time per workload:\t%.0fus\n", elapsed/100);


	// B
	gettimeofday(&start, NULL);
	for(x=0;x<100;x++){
		for(i = 0; i < 150; i++){
			ptrs[i] = malloc(1);
		}
		for(i = 0; i < 150; i++){
			free(ptrs[i]);
		}
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_usec - start.tv_usec); 
	printf("Test B...Average time per workload:\t%.0fus\n", elapsed/100);


	// C
	gettimeofday(&start, NULL);
	for(x=0;x<100;x++){
		int totalPointers = 0;
		int numFreed=0;
		int r=0;
		while(numFreed<150){
			r = rand()%2;
			//if remainder is 0 and we havent hit the cap or there are no pointers to free, allocates another pointer
			if((r == 0 && totalPointers<150) || numFreed==totalPointers){
				ptrs[totalPointers] = malloc(1);
				totalPointers++;
			}
			//remainder is 1 and there are more pointers allocated than freed
			else{
				free(ptrs[numFreed]);
				numFreed++;
			}
		} 
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_usec - start.tv_usec); 
	printf("Test C...Average time per workload:\t%.0fus\n", elapsed/100);

	//D
	gettimeofday(&start, NULL);
	for(x=0;x<100;x++){
		int totalPointers = 0;
		int numFreed=0;
		int r=0;
		//maxes out at 73 to guarantee the array won't fill (5000/68=73.5)
		while(numFreed<150){
			//printf("numFreed=%d\n",numFreed);
			r = rand()%2;
			//if remainder is 0 and we havent hit the cap or there are no pointers to free, allocates another pointer
			if((r == 0 && totalPointers<100) || numFreed==totalPointers){
				ptrs[totalPointers]=NULL;
				while(ptrs[totalPointers]==NULL){
					ptrs[totalPointers] = malloc((rand()%64)+1);
				}
				totalPointers++;
			}
			//remainder is 1 and there are more pointers allocated than freed
			else{
				free(ptrs[numFreed]);
				numFreed++;
			}
		} 
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_usec - start.tv_usec); 

	printf("Test D... Average time per workload:\t%.0fus\n", elapsed/100);

	// E: demonstrates memory is useable
	gettimeofday(&start, NULL);
	for(x=0;x<100;x++){
		int** grid=(int**) malloc(sizeof(int*)*15);
		for(i=1;i<15;i++){
			grid[i]=(int*) malloc(sizeof(int)*2);
			grid[i][0]=i;
			grid[i][1]=i;
		}
		for(i=1;i<15;i++){
			grid[i][1]=i+1;
		}
		for(i=1;i<15;i++){
			free(grid[i]);
		}
		free(grid);
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_usec - start.tv_usec);
	printf("Test E... Average time per workload:\t%.0fus\n", elapsed/100); 
	
	// F: demonstrates the allocation of large blocks of memory


    return 0;
}
