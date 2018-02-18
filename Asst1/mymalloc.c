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
    //makes a short pointer to the same block for the purpose of grabbing headers
    //myshortblock indices are worth 2 of myblock indices
    unsigned short* myShortBlock=(unsigned short*) &myblock[0];
    //converts memrequest to a short type, asks for half as many shorts as bytes
    unsigned short memReqShort=(unsigned short) memRequest;
    //rounds memory request to an even number (tempPtr indices are equal to 2 bytes, headers can't start on odd indices
    if(memReqShort%2==1)
        memReqShort++;
    memReqShort=memReqShort/2;
    // The 4 bytes at the 0 index are a dummy header with a pointer to the 4 index and 0 memory allocated
    if(myShortBlock[0]==0){
        myShortBlock[0]=2;
        myShortBlock[1]=0;
    }
    if(memRequest==0){
        return NULL;
    }
    
    // if the request exceeds maximum available memory, does not try to allocate
    if((memReqShort+2)>2500){
        printf("\nExcessive memory request error at %s, line %d\n", filename, line);
        return NULL;
    }
    
    // myblock indices used to iterate
    unsigned short currindex=0;
    unsigned short nextindex=0;
    
    // Finds first chunk of memory large enough
    // either gets to the end of allocated memory or there is sufficient free memory before the next to squeeze in a block
    while(myShortBlock[currindex]!=0 && (myShortBlock[currindex]-(currindex+2+myShortBlock[currindex+1]))<(memReqShort+2) ){
        currindex=myShortBlock[currindex];
    }
    
    // Case: "next" field is 0 because it is the last block of memory and the request couldn't be accomodated anywhere earlier
    // This is the only failure case for a request under 4996 bytes
    // currindex+4=index of memory start, memRequest+4=amount of memory requested+header size
    if(currindex+2+(memReqShort+2)>2500){
        printf("\nOut of memory error at %s, line %d\n",filename, line);
        return NULL;
    }

    // Case: valid request, no blocks after this point in simulated memory. (if it exceeded memory, would have returned above)
    // myblock[currindex+2] is the amount of memory in this block, myblock[currindex] is the next pointer
    if(myShortBlock[currindex]==0 && myShortBlock[currindex+1]!=0){
        // sets current next pointer to refer to new block
        nextindex=currindex+2+myShortBlock[currindex+1];
        myShortBlock[currindex]=nextindex;
        if(currindex==nextindex){
            printf("frogged up line 78");
        }
        // next field in new block is set to 0 to indicate that it is the last one
        myShortBlock[nextindex]=0;
        myShortBlock[nextindex+1]=memReqShort;
        return (void*) &myShortBlock[nextindex+2];
    }
    //edge case: first real node
    if(myShortBlock[currindex]==0 && myShortBlock[currindex+1]==0){
        myShortBlock[currindex+1]=memReqShort;
        return (void*) &myShortBlock[currindex+2];
    }

    // Case: valid request (no need for conditional as alternatives already returned)
    nextindex=currindex+2+myShortBlock[currindex+1];
    if(myShortBlock[currindex]==myShortBlock[nextindex] && myShortBlock[nextindex]!=0){
        printf("error line 96");
    }
    // sets next field for new block
    myShortBlock[nextindex]=myShortBlock[currindex];
    // sets memory field for new block
    myShortBlock[nextindex+1]=memReqShort; 
    myShortBlock[currindex]=nextindex;
    if(currindex==nextindex && currindex!=0){
        printf("error line 102");
    }
    return (void*) &myShortBlock[nextindex+2];
}

// improved version of free that prevents some user errors (freeing pointers that were not allocated and freeing non-pointers)
void myfree(void* memptr, char* filename, int line){
    unsigned short* myShortBlock=(unsigned short*) &myblock[0];
    long memptrVal=(long) memptr;
    long memStart=(long) &myShortBlock[0];
    unsigned short freeIndex = (memptrVal-memStart)/2;
    unsigned short index=0;
    unsigned short prev=0;

    // Case: not a valid pointer to our dynamic memory
    if(freeIndex<0 || freeIndex>2500){
        printf("Attempts to free a type other than pointer at %s line %d\n",filename, line);
        return;
    }

    // iterates through to find the correct chunk of memory
    while(freeIndex>myShortBlock[index] && freeIndex!=index+2 &&myShortBlock[prev]!=0){
        prev=index;
        index=myShortBlock[index];      
    }

    // Case: pointer was an address between two valid pointers (index has passed the pointer). Not a valid operation
    if(index>freeIndex || myShortBlock[prev]==0){
        printf("Attempts to free invalid pointer at %s line %d\n",filename, line);
        return;
    }

    if(freeIndex==index+2){
        //case: delete head, nothing else is in the list
        if(prev==0 && myShortBlock[index]==0){
            myShortBlock[index]=0;
            myShortBlock[index+1]=0;
            myShortBlock[0]=2;
            return;
        }
        // effectively previous.next=current.next
        if(myShortBlock[prev]==myShortBlock[index] && myShortBlock[index]!=0){
            printf("you goofed");
        }
        myShortBlock[prev]=myShortBlock[index];
        myShortBlock[index]=0;
        myShortBlock[index+1]=0;
        // Could set the other indices to 0, but not actually necessary since they are no longer recognized as pointers
    }
}




