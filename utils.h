/*This file will contain commonly used utility/helper 
functions throughout our program*/
#ifndef utils
#define utils

#include <stdbool.h>

//Check if a character is one of the special operators.
bool isOperator(const char *c){
	if(c == ';' || c == '|' || c == '&&' || c == '||'){
		return true;
	}
	else return false;
}

//Determines the precedence of the Operator and returns an integer value
//Input: op -- input operator, multi is for the special case double characers
//such as  && or || 
//NOTE/TODO: This implmentation doesn't have to be final, as I know it will depend
//heavily on how we tokenize the operators, but for now, its pretty intuitive
//as a starting point
int precedence(char * op){
	switch(op){
		case ';'://5ith highest
			return 1;
		case '||':
			return 2;//4th highest for ||
		case '&&'://4th highest
			return 2;
		case '|'://highest
			return 3;
		default:
			//THROW ERROR
			printf("Invalid! Exiting...\n");
			exit(-1);		
	}
	return -1;
}

#endif
