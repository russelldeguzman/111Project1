
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
void newStack(stack *st, int dataSize){
	assert(dataSize > 0);
	st->dataSize = dataSize;
	st->length = 0;
	st->allocLength = initialSize;
	st->data = malloc(initialSize * dataSize);
	assert(st->data != NULL);
} 

//free stack
void destroyStack(stack *st){
	free(st->data);
} 

//isempty
bool StackisEmpty(const stack *st){
	if(st->length == 0) return true;
	return false;
} 

 //push
void pushStack(stack *st, const void* add){
	void *destination;
	if(st->length == st->allocLength){
		st->allocLength *= 2;
		st->data = realloc(st->data, st->allocLength * st->dataSize);
		assert(st->data != NULL);
	}

	destination = (char *)st->data + st->length * st->dataSize;
	memcpy(destination,add,st->dataSize);
	st->length++;
}

//pop
void popStack(stack *st, void* top){
	const void *src;
	assert(!StackisEmpty(st));
	st->length--;
	src = (const char *) st->data + st->length * st->dataSize;
	memcpy(top, src, st->dataSize);
} 

//view top element (doesnt affect stack)
void topStack(stack *st, void* top){
	const void *src;
	assert(!StackisEmpty(st));
	src = (const char *) st->data + (st->length - 1) * st->dataSize;
	memcpy(top, src, st->dataSize);
}

#endif
