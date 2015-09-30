#include "stack.h"

//Routine to test & debug the Stack datastructure.
int main(int argc, char **argv){
	int val;
	stack intStack; //create struct
	newStack(&intStack, sizeof(int)); //initialize stack

	for(val = 0; val < 6; val++){
		pushStack(&intStack, &val); //push values 0 - 6
	}	

	while (!StackisEmpty(&intStack)){
		topStack(&intStack, &val); //test top function
		printf("The top of the stack is: %d\n", val);
		popStack(&intStack, &val); // test pop function
		printf("This just popped: %d\n", val);
		if(!StackisEmpty(&intStack)){ 
			printf("This stack is NOT empty!\n");
		}
		else printf("This stack is empty!\n");
	}

	destroyStack(&intStack); //destroy stack
}
