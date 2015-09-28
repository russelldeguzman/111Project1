#include "stack.h"

#define initialSize 10

//TODO: These functions are not tested yet. Will write a test routine later to do so.

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

	destination = (char *)s->data + st->length * st->dataSize;
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
	src = (const char *) st->data + st->length * st->dataSize;
	memcpy(top, src, st->dataSize);
}
