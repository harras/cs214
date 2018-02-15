
#include "mymalloc.h"

int main(int argc, char* argv[]){
	trythis();
	char* blerp=(char*) malloc(8*sizeof(char));
	char* mem1= (char*) malloc(16*sizeof(char));
	printf("memlocation %ld\n",(long) blerp);
	printf("memlocation 2 %ld\n",(long) mem1);
	free(blerp);
	free((void*) 5);
	free(blerp+500); 
	//printf("shorts are of size %lu\n",sizeof(short));
	return(0);
}
