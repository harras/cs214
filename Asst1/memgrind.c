
#include "mymalloc.h"

int main(int argc, char* argv[]){
	char* blerp=(char*) malloc(5);
	printf("memlocation %ld",(long) blerp);
	free(blerp);
	return(0);
}
