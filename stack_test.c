#include "stack.h"

void wrapper(stack *st, char * add){
	pushStack(st, add);
}
//Routine to test & debug the Stack datastructure.
//INSIGHT: literally declared strings and char pointers have different sizes, be careful of this 
int main(int argc, char **argv){
	char test[] = "hello world";
	stack charStack; //create struct
	newStack(&charStack, sizeof(char)*12); //initialize stack
	wrapper(&charStack,test); /
	char * temp;
	popStack(&charStack,temp);
	printf("%s\n", temp);
	destroyStack(&charStack); //destroy stack
}
