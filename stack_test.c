#include "stack.h"
struct data{
	int *x;
	int y;
};

void wrapper(stack * st, struct data * result){
	popStack(st, result);
}

//Routine to test & debug the Stack datastructure.
//INSIGHT: literally declared strings and char pointers have different sizes, be careful of this 
int main(int argc, char **argv){
	struct data *test = (struct data *) malloc(sizeof(struct data));
	test->x=NULL;
	test->y=5;
	stack dataStack; //create stack
	newStack(&dataStack, sizeof(struct data)); //initialize stack
	pushStack(&dataStack, test);

	struct data* res = (struct data *) malloc(sizeof(struct data));
	wrapper(&dataStack, res); 
	/**struct data* foo;
	foo = &res;
	struct data* bar;
	bar = foo;
	foo = NULL;*/
	printf("%i\n", res->y);
	destroyStack(&dataStack); //destroy stack
	free(test);
	free(res);
}
