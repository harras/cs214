/** mymalloc.c: framework to simulate memory allocation and allow
 * for improved malloc and free functions
 *
 *
 * block headers are 4 byte linked lists with a 2 byte "next" 
 * pointer (an offset from front of the block) and 2 byte memory 
 * size field
 * 
 *  block structure:
 *       [][]                         [][]                            [][][]][][][][].....
 *  index of next header    # of mem bytes in this block             bytes of mem allocated
 **/


//the amount of memory available before the next pointer: pointer-4+(next-memory)
#include "mymalloc.h"
static char myblock[5000];

// dummy function used to debug random things: to be removed before submitting
void trythis(){
	char* blerptr=&myblock[0];
	long blerp=(long) blerptr;
	printf("\narray starts at %ld\n",blerp);
}

// improvement on malloc
// Returns a void* pointer for valid requests or "NULL" for invalid requests. Prints calling file and line of invalid request
void* mymalloc(int memRequest, char* filename, int line){
	
	// The 4 bytes at the 0 index are a dummy header with a pointer to the 4 index and 0 memory allocated
	if((short) myblock[0]==0){
		short firstblock=4;
		myblock[0]=firstblock;
		short blankmem=0;
		myblock[2]=blankmem;
	}
	
	// if the request exceeds maximum available memory, does not try to allocate
	if((memRequest+4)>5000){
		printf("\nExcessive memory request error at %s, line %d", filename, line);
		return NULL;
	}
	
	// myblock indices used to iterate
	short currindex=0;
	short nextindex=0;
	
	// Finds first chunk of memory large enough
	// either gets to the end of allocated memory or there is sufficient free memory before the next to squeeze in a block
	while(myblock[currindex]!=0 && (myblock[currindex]-(currindex+4+memRequest))<(memRequest+4) ){
		currindex=myblock[currindex];
	}
	
	// Case: "next" field is 0 because it is the last block of memory and the request couldn't be accomodated anywhere earlier
	// This is the only failure case for a request under 4996 bytes
	// currindex+4=index of memory start, memRequest+4=amount of memory requested+header size
	if(currindex+4+(memRequest+4)>5000){
		printf("\nOut of memory error at %s, line %d",filename, line);
		return NULL;
	}

	// Case: valid request, no blocks after this point in simulated memory. (if it exceeded memory, would have returned above)
	// myblock[currindex+2] is the amount of memory in this block, myblock[currindex] is the next pointer
	if(myblock[currindex]==0){
		// sets current next pointer to refer to new block
		nextindex=currindex+4+myblock[currindex+2];
		myblock[currindex]=nextindex;
		// next field in new block is set to 0 to indicate that it is the last one
		myblock[nextindex]=0;
		myblock[nextindex+2]=memRequest;
		return (void*) &myblock[nextindex+4];
	}

	// Case: valid request (no need for conditional as alternatives already returned)
	nextindex=currindex+4+myblock[currindex+2];
	// sets next field for new block
	myblock[nextindex]=myblock[currindex];
	// sets memory field for new block
	myblock[nextindex+2]=memRequest; 
	myblock[currindex]=nextindex;
	
	return (void*) &myblock[nextindex+4];
}

// improved version of free that prevents some user errors (freeing pointers that were not allocated and freeing non-pointers)
void myfree(void* memptr, char* filename, int line){
	long memptrVal=(long) memptr;
	long memStart=(long) &myblock[0];
	long freeIndex = memptrVal-memStart;
	short index=0;
	short prev=0;

	// Case: not a valid pointer to our dynamic memory
	if(freeIndex<0 || freeIndex>5000){
		printf("Attempts to free a type other than pointer at %s line %d",filename, line);
		return NULL;
	}

	// iterates through to find the correct chunk of memory
	while(freeIndex>(short) myblock[index] && freeIndex!=index+4 && myblock[prev]!=0){
		prev=index;
		index=myblock[index];		
	}

	// Case: pointer was an address between two valid pointers (index has passed the pointer). Not a valid operation
	if(index>freeIndex || myblock[prev]==0){
		printf("Attempts to free invalid pointer at %s line %d",filename, line);
		return NULL;
	}

	if(freeIndex==index+4){
		// effectively previous.next=current.next
		myblock[prev]=myblock[index];

		// Could set the other indices to 0, but not actually necessary since they are no longer recognized as pointers
		// myblock[index]=(short) 0;
		// myblock[index+2]=(short) 0;
	}
}




