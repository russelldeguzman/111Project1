// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "symbol.h"

#include "utils.h"
#include "stack.h"
#include <error.h>
#include <string.h>
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

//According to Tuan, this is actually the linked list
//I put the command_node in command-internals
struct command_stream { 
	struct command_node_t head;
	struct command_node_t tail;
};


/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
void commandStreamInit(command_stream_t stream){
  stream->head = NULL;
  stream->tail = NULL;
}

//Determines whether or not the parser output is a simple command
bool is_simple_command(char * parserOutput){
  if(!isOperator(parserOutput) && strncmp(parserOutput,"\n",1) != 0) return true;
  return false;
}

// goes through an idetnified simple command and checks for input and output redirection and puts it inside the command
void parseSimpCommand(char * parserOutput, char **input, char **output, char **word){
  int pos = 0;
  int hasInput = -1; //represents the position of '<'
  int hasOutput = -1; //represents the position of '>'
  while(parserOutput[pos] != '\0'){ //TODO: remember to end strings with '\0'
    if(parserOutput[pos] == '<') hasInput = pos; 
    if(parserOutput[pos] == '>') hasOutput = pos; 
    pos = pos + 1;
  }
  if(hasOutput == -1 && hasInput == -1){ //CASE 1: NO I/O REDIRECTION
    *input = NULL;
    *output = NULL; 
    *word = (char *) malloc(sizeof(parserOutput));
    strcpy(*word , parserOutput);
    return;
  }

  if(hasOutput != -1 && hasInput == -1){//CASE 2: Output but no Input
    char result_word[pos];
    char output_word[pos];
    int i;

    for(i = 0; i < hasOutput; i++){
      result_word[i] = parserOutput[i]; //get result word
    }
    
    if(isspace(result_word[hasOutput-1])){ //elim whitespace before > if there is one
      result_word[hasOutput - 1] = '\0';
    }

    else{
      result_word[hasOutput] = '\0';
    }

    if(isspace(parserOutput[hasOutput + 1])){
      i = hasOutput + 2; //elim whitespace after > if there is any
    }

    else{
      i = hasOutput + 1;
    }
    int offset = i;
    for(i = offset; i < pos; i++){
      output_word[i - offset] = parserOutput[i]; //get output 
    }
    output_word[pos - offset] = '\0';

    *input = NULL;
    *output = (char *) malloc(sizeof(output_word));
    *word = (char *)malloc(sizeof(result_word));
    strcpy(*output, output_word);
    strcpy(*word, result_word);

    return;
    
  } 
  if(hasOutput == -1 && hasInput != -1){//CASE 3: Input but no Output
    char result_word[pos];
    char input_word[pos];
    int i;

    for(i = 0; i<hasInput; i++){
      result_word[i] = parserOutput[i]; //get result word 
    }

    if(isspace(result_word[hasInput-1])){ //elim whitespace before < if there is one
      result_word[hasInput - 1] = '\0';
    }
    else{
      result_word[hasInput] = '\0';
    }

    if(isspace(parserOutput[hasInput + 1])){
      i = hasInput + 2; //elim whitespace after < if there is any
    }
    else{
      i = hasInput + 1;
    }
    int offset = i;

    for(i = offset; i < pos; i++){
      input_word[i - offset] = parserOutput[i]; //get output 
    }
    input_word[pos - offset] = '\0';

    *input = (char *)malloc(sizeof(input_word));
    *word = (char *)malloc(sizeof(result_word));
    *output = NULL; 
    strcpy(*word,result_word);
    strcpy(*input,input_word);
    return;
  }
  if(hasInput != -1 && hasOutput != -1){//CASE 4: has both I/O
    //In test cases, Input always comes before Output, so I'm going to assume that's 
    //the correct syntax. 
    char result_word[pos];
    char input_word[pos];
    char output_word[pos];
    int i;
    for(i = 0; i < hasInput; i++){
      result_word[i] = parserOutput[i]; //get result file word
    }
    if(isspace(result_word[hasInput-1])){ //elim whitespace before < if there is one
      result_word[hasInput - 1] = '\0';
    }
    else{
      result_word[hasInput] = '\0';
    }

    if(isspace(parserOutput[hasInput + 1])){
      i = hasInput + 2; //elim whitespace after < if there is any
    }
    else{
      i = hasInput + 1;
    }
    int offset = i;

    for(i = offset; i < hasOutput; i++){
      input_word[i - offset] = parserOutput[i]; //get output 
    }

    if(isspace(result_word[hasOutput-1])){ //elim whitespace before > if there is one
      input_word[hasOutput - offset -1] = '\0';
    }
    else{
      input_word[hasOutput - offset] = '\0';
    }

    if(isspace(parserOutput[hasOutput + 1])){
      i = hasOutput + 2; //elim whitespace after > if there is any
    }
    else{
      i = hasOutput + 1;
    }
    offset = i;
    for(i = offset; i < pos; i++){
      output_word[i - offset] = parserOutput[i]; //get output 
    }
    output_word[pos - offset] = '\0';

    *input = (char *)malloc(sizeof(input_word));
    *word = (char *)malloc(sizeof(result_word));
    *output = (char *)malloc(sizeof(output_word)); 
    strcpy(*word,result_word);
    strcpy(*input,input_word);
    strcpy(*output, output_word);
    return;
  }
  printf("No events triggered! Error!\n");
}

//breaks up the simple command from the parser and constructs command
void constructSimpleCommand(command_t com, char * parserOutput){
  com = (command_t) malloc(sizeof(struct command));
  com->type = SIMPLE_COMMAND; 
  com->status =  -1; //TODO: EDIT THIS IN LAB 1B
  parseSimpCommand(parserOutput, com->&input, com->&output, com->u.word/*TODO: Need to check this*/);

}

//combines two commands into result
void combine_commands(command_t right,command_t left ,command_t result, char * op){

  if(strncmp(op,"&&",2) == 0) result->type = AND_COMMAND;
  else if(strncmp(op,";",1) == 0) result->type = SEQUENCE_COMMAND;
  else if(strncmp(op,"||",2) == 0) result->type = OR_COMMAND;
  else result->type = PIPE_COMMAND; 
  
  result->status = -1; //TODO: for part 1B ( dont worry for 1A)
  result->input = NULL;
  result->output = NULL;
  result->u.command[0] = left;
  result->u.command[1] = right;
}

//combine_helper: see d) (1-3) in the psuedocode below in make_command_stream
void combine_helper(stack &opStack, stack &cmdStack, char * tempOp){
   command_t r;
   command_t l;
   command_t result = (command_t) malloc(sizeof(struct command));
   popStack(opStack, tempOp);
   popStack(cmdStack, l);
   popStack(cmdStack, r);
   combine_commands(r,l,result, tempOp);
   pushStack(cmdStack, result);
   topStack(opStack, tempOp);  
}

//creates a subshell
void createSubshell(command_t topCommand, command_t subshellCommand){
  subshellCommand = (command_t) malloc(sizeof(struct command));
  subshellCommand->type = SUBSHELL_COMMAND;
  subshellCommand->status = -1; //TODO: change this in 1B.
  subshellCommand->input = NULL;
  subshellCommand->output = NULL;
  subshellCommand->u.subshell_command = topCommand;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
	char current = get_next_byte(get_next_byte_argument);
	
	while (current != EOF) { // Parsing
		switch (current) {
			case ';':
				
				break;
			case '|':
				
				break;
			case '&':
				
				break;
			case '(':
				
				break;
			case ')':
				
				break;
			case '<':
				
				break;
			case '>':
				
				break;
			default: // Making the dangerous assumption that all other
					 // characters are safe
				
		}
	}
		

  //initialize a command_stream linked list
     command_stream_t stream;
     commandStreamInit(stream);

  /*TODO: Parser from left to right. Create command_node every time 
  There's a new command 
  */
  // The output from this function I will denote as parserOutput for now
    char * parserOutput; //TODO: 

  //INIT STACKS
    stack operatorStack;
    stack commandStack;
    newStack(&commandStack,sizeof(command_t));
    newstack(&operatorStack,sizeof(char)* 3); //3 is the max length of a string for stuff like (&&\0) or (||\0)
    while(/*!EOF from input*/)
    {
      //a)If a simple command, push to a command stack
      if(is_simple_command(parserOutput)){
        command_t simpCommand;
        constructSimpleCommand(simpCommand, parserOutput);
        pushStack(&commandStack, simpCommand);
      }
      
      //b)If it is a "(", push it onto an operator-stack
      if(strcmp(parserOutput,"(",1) == 0){
        pushStack(&operatorStack, parserOutput);
      }
      
      //c)If it is an Operator and operator stack is empty
      //  1)push the operator onto the operator stack
      if(isOperator(parserOutput) && StackisEmpty(&operatorStack)){
        pushStack(&operatorStack, parserOutput);
      }

     /* 
      d)If it is an operator and the operator stack is NOT empty
        1)Pop all operators with greator or equal precedence off the operator stack
          For each popped off Operator, Pop 2 commands  off command stack
          combine them into a new command
        2)Stop when you reach an operator with lower precedence or a "("
        3)Push the operator onto the stack
      */
      if(isOperator(parserOutput) && !StackisEmpty(&operatorStack)){
        char * tempOp;
        topStack(&operatorStack, tempOp);
        while(!StackisEmpty(&operatorStack) && precedence(tempOp)>=precedence(parserOutput) && strcmp(tempOp,"(",1) != 0){
          combine_helper(&operatorStack, &commandStack, tempOp);
        }
        pushStack(&operatorStack, parserOutput);
      }
    
     // e)If encounter ")", pop operators off the stack 
    //(for each operator, pop two commands, combine, push back on command stack) 
    //until you find a matching "(". then create a subshell command by popping 
    //out 1 command from command stack.
      if(strcmp(parserOutput,")",1) == 0){
        char * tempOp;
        topStack(&operatorStack, tempOp);
        while(!StackisEmpty(&operatorStack) && strcmp(tempOp,"(",1) != 0){
          combine_helper(&operatorStack,&commandStack,tempOp);
        }
        command_t subshellCommand;
        command_t topCommand;
        popStack(&commandStack, topCommand);
        popStack(&operatorStack,tempOp);
        createSubshell(topCommand, subshellCommand);
        pushStack(&commandStack, subshellCommand);
      }
    }    //f) Advance to next word (simple command, and, or) go to a)
    
    //g)When all words are gone, pop each operator and 
    //combine them with 2 commands similar to d)
    char * tempOp;
    topStack(&operatorStack, tempOp);
    while(!StackisEmpty(&operatorStack)){
        combine_helper(&operatorStack,&commandStack,tempOp);
    }
    
    //rootNode is now ready to add to the tree.
    command_t rootNode;
    popStack(&commandStack, rootNode); 

    //TODO: add the rootNode to a command_Node

    //TODO: add the command_Node to the linked list
    append(stream.tail, /*Command_Node*/ ); 

    //return Command_stream linked list
    return stream;
}

command_t
read_command_stream (command_stream_t s)
{
  command_node_t current = s->head;
  s->head = current -> next;
  return current -> root; 
}
