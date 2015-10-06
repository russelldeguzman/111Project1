#include "utils.h"

int main(int argc, char **argv){
	//isOperator Test 
	char test[]="|"; //put in a test here

	int precedence_test = precedence(test);
	printf("%i\n", precedence_test );

}
