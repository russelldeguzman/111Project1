#include "utils.h"

int main(int argc, char **argv){
	//isOperator Test 
	char test[]="|"; //put in a test here
	if(isOperator(test)) printf("This is an operator!! \n");
	else printf("This is NOT an operator!! \n");

	int precedence_test = precedence(test);
	printf("%i\n", precedence_test );

}