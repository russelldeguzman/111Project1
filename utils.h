/*This file will contain commonly used utility/helper 
functions throughout our program*/
#ifndef utils
#define utils

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Determines the precedence of the Operator and returns an integer value
//Input: op -- input operator, multi is for the special case double characers
//such as  && or || 
//NOTE/TODO: This implmentation doesn't have to be final, as I know it will depend
//heavily on how we tokenize the operators, but for now, its pretty intuitive
//as a starting point
int precedence(char * op){
	if(strncmp(op,";",1) == 0){
		return 1;
	}
	else if(strncmp(op,"||",2) == 0){
		return 2;
	}
	else if(strncmp(op,"&&",2) == 0){
		return 2;
	}
	else if(strncmp(op,"|",1) == 0){
		return 3;
	}
	else{
		printf("Error!\n");
		return -1;
	}
}

#endif
