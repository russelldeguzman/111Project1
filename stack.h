
/*This implementation is of a GENERIC stack using void 
pointers to access memory regardless of the data type */
#ifndef stck
#define stck

#include "stdlib.h" 
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/*Intial number elements able to be held in stack
Dynamically doubles if needed*/
#define initialSize 5 

//Stack struct
typedef struct{
	void *data; //data pointer
	int dataSize; //the size of the particular data (e.g. sizeof(int))
	int length; //number of elements in the container
int allocLength; //the length of memory reserved for this container 
} stack;

// STACK OPERATIONS:
//create stack
void newStack(stack *st, int dataSize);

//free stack
void destroyStack(stack *st);

//isempty
bool StackisEmpty(const stack *st);

 //push
void pushStack(stack *st, const void* add);

//pop
void popStack(stack *st, void* top);

//view top element (doesnt affect stack)
void topStack(stack *st, void* top);

#endif
