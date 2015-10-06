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

#define initialSize 5;

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
void combine_commands(command_t right,command_t left ,command_t result, symbol_type op){

  if(op == AND_SYMBOL) result->type = AND_COMMAND;
  else if(op ==SEQUENCE_SYMBOL) result->type = SEQUENCE_COMMAND;
  else if(op == OR_SYMBOL) result->type = OR_COMMAND;
  else result->type = PIPE_COMMAND; 

  result->status = -1; //TODO: for part 1B ( dont worry for 1A)
  result->input = NULL;
  result->output = NULL;
  result->u.command[0] = left;
  result->u.command[1] = right;
}

//combine_helper: see d) (1-3) in the psuedocode below in make_command_stream
void combine_helper(stack &opStack, stack &cmdStack, symbol_type tempOp){
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

//Helper for turning simple commands into tokens
void createSimpCommand(symbol_t &sym, int &len, int &maxLen, char *&data){
	if (len == maxLen) {
		maxLen += 1;
		data = realloc(data, maxLen*sizeof(char));
	}
	data[len] = '\0';
	sym->simple_command = data;
	createSymbol(sym, COMMAND_SYMBOL);

	int len = 0;
	int maxLen = initialSize;
    char *data = malloc(initialSize * sizeof(char));
}

void createSymbol(symbol_t &sym, symbol_type type) {
	sym->type = type;
	symbol_t tempSymbol = newSymbol();
	sym->next = tempSymbol;
	sym = tempSymbol;
	sym->next = NULL;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
	char currentChar = get_next_byte(get_next_byte_argument);

	int commandLength = 0;
	int allocLength = initialSize;
	char *simpleCommand = malloc(initialSize * sizeof(char));

	int empty = 0; // Tracking whether the last simple command was empty.
				   // 0 = empty, 1 = not empty
	int skip = 0;  // Skips the character read at the end of each
				   // iteration of while: ugly workaround to
				   // distinguishing | and ||

	symbol_t currentSymbol = newSymbol();
	symbol_t headSymbol = currentSymbol;
	symbol_t tempSymbol;

	while (currentChar != EOF) { // Parsing
		switch (current) {
			case ';':
				assert(empty == 1); // Assert that there is a non-null
									// command prior, or the operation is
									// invalid.
				createSimpCommand(currentSym, commandLength,
									allocLength, simpleCommand);
				currentSymbol->type = SEQUENCE_SYMBOL;
				createSymbol(sym, SEQUENCE_SYMBOL);
				break;
			case '|':
				assert(empty == 1);
				createSimpCommand(currentSym, commandLength,
									allocLength, simpleCommand);
					// Check the next character. If it's also a pipe, we
					// have an or operator. If it's anything else, it's
					// just a pipe.
				currentChar = get_next_byte(get_next_byte_argument);
				if (currentChar == '|') {
					createSymbol(sym, OR_SYMBOL);
				} else {
					createSymbol(sym, PIPE_SYMBOL);
				}
				skip = 1;
				break;
			case '&':
				assert(empty == 1);
				createSimpCommand(currentSym, commandLength,
									allocLength, simpleCommand);
				assert (get_next_byte(get_next_byte_argument) == '&');
					// If the & character isn't followed by another one,
					// the operator is invalid.
				createSymbol(sym, AND_SYMBOL);
				break;
			case '(':
				if (empty == 1) { // There does not necessarily need to
								  // be a simple command before a '('
				createSimpCommand(currentSym, commandLength,
									allocLength, simpleCommand);
				}
				
				createSymbol(sym, LBRACKET_SYMBOL);
				break;
			case ')':
				assert(empty == 1);
				createSimpCommand(currentSym, commandLength,
									allocLength, simpleCommand);
				
				createSymbol(sym, RBRACKET_SYMBOL);
				break;
			case '#': // Advance to end of line. No break becase # is
					  // an effective newline
				while (currentChar != '\n') {
					get_next_byte(get_next_byte_argument);
				}
			case '\n':
				if (empty == 1) {
				createSimpCommand(currentSym, commandLength, allocLength, simpleCommand);
				}

				// Check the next byte. if it's also a newline, we have a new command.
				currentChar = get_next_byte(get_next_byte_argument);
				if (currentChar == '\n') {
					createSymbol(NEWCOMMAND_SYMBOL);
					while (currentChar == '\n') {
						currentChar = get_next_byte(get_next_byte_argument);
					}
				} else { //otherwise, we just have a sequence command.
					createSymbol(SEQUENCE_SYMBOL);
				}
				skip = 1;
				break;
			default: // Making the dangerous assumption that all other
					 // characters are safe
				if (empty == 0 && currentChar != ' ') {
					empty = 1;
				}
				if (commandLength == allocLength) {
					allocLength *= 2;
					simpleCommand = realloc(simpleCommand, allocLength *
									sizeof(char));
				}
				simpleCommand[commandLength++] = current;
				break;
		}
		if (skip == 0) {
			current = get_next_byte(get_next_byte_argument);
		} else {
			skip = 0;
		}
	}

	//error (1, 0, "command reading not yet implemented");
	//return 0;

  //TODO: initialize a command_stream linked list

  //initialize a command_stream linked list
     command_stream_t stream;
     commandStreamInit(stream);

  /*TODO: Parser from left to right. Create command_node every time
  There's a new command
  */
  // The output from this function I will denote as parserOutput for now
  //  char * parserOutput; //TODO:
  // Formerly parserOutput, now we use currentSymbol to denote the
  // current output node.
	currentSymbol = headSymbol;

  //INIT STACKS
    stack operatorStack;
    stack commandStack;
    command_t currCommand = (command_t) malloc(sizeof(command)); //init first command
    command_node_t currCommandNode = (command_node_t) malloc(sizeof(command_node));// init first command node
    stream->head = currCommandNode;
    stream->tail = currCommandNode; //attach to the stream

    newStack(&commandStack,sizeof(command_t));
    newstack(&operatorStack,sizeof(int)); //enums symbols are ints
    while(currentSymbol != NULL)
    {

      if(currentSymbol->symbol_type == NEWCOMMAND_SYMBOL){
          //create a new command node, hoook the last one to the tail  of the stream
          command_t temp = (command_t) malloc(sizeof(command));
          command_node_t = (command_node_t) = malloc(sizeof(command_node));
          stream->tail->next = currCommandNode;
          currCommandNode->root = currCommand; 
          currCommandNode->next = NULL;
          currCommand = temp; 
      }
      //check if this symbol is a newline and the next symbol is a newline

      //a)If a simple command, push to a command stack
      if(currentSymbol->type = COMMAND_SYMBOL){
        command_t simpCommand;
        constructSimpleCommand(simpCommand, currentSymbol->simple_command);
        pushStack(&commandStack, simpCommand);
      }

      //b)If it is a "(", push it onto an operator-stack
      if(strcmp(currentSymbol->simple_command,"(",1) == 0){
        pushStack(&operatorStack, currentSymbol->simple_command);
      }

      //c)If it is an Operator and operator stack is empty
      //  1)push the operator onto the operator stack
      if(currentSymbol->type != COMMAND_SYMBOL && StackisEmpty(&operatorStack)){
        pushStack(&operatorStack, currentSymbol->simple_command);
      }

     /*
      d)If it is an operator and the operator stack is NOT empty
        1)Pop all operators with greator or equal precedence off the operator stack
          For each popped off Operator, Pop 2 commands  off command stack
          combine them into a new command
        2)Stop when you reach an operator with lower precedence or a "("
        3)Push the operator onto the stack
      */
      if(currentSymbol->type != COMMAND_SYMBOL && !StackisEmpty(&operatorStack)){
        symbol_type tempOp;
        topStack(&operatorStack, tempOp);
        while(!StackisEmpty(&operatorStack) && precedence(tempOp)>=precedence(currentSymbol->type) && tempOp == LBRACKET_SYMBOL){
          combine_helper(&operatorStack, &commandStack, tempOp);
        }
        pushStack(&operatorStack, currentSymbol->type);
      }

     // e)If encounter ")", pop operators off the stack
    //(for each operator, pop two commands, combine, push back on command stack) 
    //until you find a matching "(". then create a subshell command by popping 
    //out 1 command from command stack.
      if(currentSymbol->symbol_type == RBRACKET_SYMBOL){
        symbol_type tempOp;
        topStack(&operatorStack, tempOp);
        while(!StackisEmpty(&operatorStack) && currentSymbol -> symbol_type != LBRACKET_SYMBOL){
          combine_helper(&operatorStack,&commandStack,tempOp);
        }
        command_t subshellCommand;
        command_t topCommand;
        popStack(&commandStack, topCommand);
        popStack(&operatorStack,tempOp);
        createSubshell(topCommand, subshellCommand);
        pushStack(&commandStack, subshellCommand);
      }
      //f) Advance to next word (simple command, and, or) go to a)
      currentSymbol=currentSymbol->next;
    }
    
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
    currCommandNode = rootNode;

    //TODO: add the command_Node to the linked list
    stream->tail->next = currCommandNode; //ATTACHES THE LAST COMMAND
    currCommandNode->next = NULL; //MARKS THE END
    

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
