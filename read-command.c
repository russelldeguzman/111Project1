// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define initialSize 5 

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

typedef struct symbol *symbol_t;
typedef struct symbol_header *symbol_header_t;

typedef enum {
	COMMAND_SYMBOL,		// Regular commands
	OR_SYMBOL,		// ||
	AND_SYMBOL,		// &&
	PIPE_SYMBOL,		// |
	LBRACKET_SYMBOL,	// (
	RBRACKET_SYMBOL,	// )
	SEQUENCE_SYMBOL,	// ;
	NEWCOMMAND_SYMBOL	// \n
} symbol_type;

struct symbol {
	symbol_type type;
	char *simple_command;
	struct symbol *next;
};

typedef struct {
	void *data; //data pointer
	int dataSize; //the size of the particular data (e.g. sizeof(int))
	int length; //number of elements in the container
	int allocLength; //the length of memory reserved for this container 
} stack;

  //Command Node
struct command_node {
	command_t root;
	command_node_t next;
};

struct command_stream{
	command_node_t head;
	command_node_t tail;
};

symbol_t newSymbol() {
	struct symbol *new = (struct symbol*)malloc(sizeof(struct symbol));
	new->simple_command = NULL;
	new->next = NULL;
	return new;
}

int precedence(symbol_type op){
	if(op == SEQUENCE_SYMBOL){
		return 1;
	}
	else if(op == OR_SYMBOL || op == AND_SYMBOL){
		return 2;
	}
	else if(op == PIPE_SYMBOL){
		return 3;
	}
	else{
		printf("Error!\n");
		return -1;
	}
}

//command_node initalize
void node_init(command_node_t n, command_t command){
	n->root=command;
	n->next=NULL;
}
//get tail of list
command_node_t getTail(command_node_t head){
	command_node_t tail=head;
	while(tail->next != NULL) tail=tail->next;
	return tail;
}

//add to list 
void append(command_node_t tail, command_node_t newNode){
	tail->next=newNode;
	newNode->next=NULL;
	tail=tail->next;	
}

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

	destination = (char *)st->data + st->length * st->dataSize;
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
	src = (const char *) st->data + (st->length - 1) * st->dataSize;
	memcpy(top, src, st->dataSize);
}

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
void commandStreamInit(command_stream_t stream){
  stream->head = NULL;
  stream->tail = NULL;
}

// goes through an idetnified simple command and checks for input and output redirection and puts it inside the command
void parseSimpCommand(char * parserOutput, char **input, char **output, char ***word){
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
    *word =(char **) malloc(sizeof(char *)); 
    **word = malloc(sizeof(parserOutput));
    strcpy(**word , parserOutput);
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
    *output = (char* )malloc(sizeof(output_word));
    *word =(char **) malloc(sizeof(char *));
    **word = malloc(sizeof(parserOutput));    strcpy(*output, output_word);
    strcpy(**word, result_word);

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

    *input = (char* )malloc(sizeof(input_word));
    *word =(char **) malloc(sizeof(char *));
    **word = malloc(sizeof(parserOutput));
    *output = 0; 
    strcpy(**word,result_word);
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

    *input = (char* )malloc(sizeof(input_word));
    *word =(char **) malloc(sizeof(char *));
    **word = malloc(sizeof(parserOutput));    
    *output = (char* )malloc(sizeof(output_word)); 
    strcpy(**word,result_word);
    strcpy(*input,input_word);
    strcpy(*output, output_word);
    return;
  }
  printf("No events triggered! Error!\n");
}

//breaks up the simple command from the parser and constructs command
void constructSimpleCommand(command_t com, char * parserOutput){
  com->type = SIMPLE_COMMAND; 
  com->status =  -1; //TODO: EDIT THIS IN LAB 1B
  parseSimpCommand(parserOutput, &(com->input), &(com->output), &(com->u.word));/*TODO: Need to check this*/
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
void combine_helper(stack *opStack, stack *cmdStack, symbol_type *tempOp){
   struct command r;
   struct command l;
   struct command result;
   popStack(opStack, tempOp);
   popStack(cmdStack, &l);
   popStack(cmdStack, &r);
   combine_commands(&r,&l, &result, *tempOp);
   pushStack(cmdStack, &result);
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

//Remember to fix & and *s
void createSymbol(symbol_t *sym, symbol_type type) {
	(*sym)->type = type;
	symbol_t tempSymbol = newSymbol();
	(*sym)->next = tempSymbol;
	(*sym) = tempSymbol;
	(*sym)->next = NULL;
}

//Helper for turning simple commands into tokens
//Pointer bug, fix later.
void createSimpCommand(symbol_t *sym, int *len, int *maxLen, char **data, int *empty){
	if (*len == *maxLen) {
		*maxLen += 1;
		*data = (char*)realloc(*data, (*maxLen)*sizeof(char));
	}
	(*data)[*len] = '\0';
	(*sym)->simple_command = *data;
	createSymbol(sym, COMMAND_SYMBOL);

	*len = 0;
	*maxLen = initialSize;
	*data = (char*)malloc(initialSize * sizeof(char));
	*empty = 0;
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
	char *simpleCommand = (char*)malloc(initialSize * sizeof(char));
	int empty = 0; // Tracking whether the last simple command was empty.
				   // 0 = empty, 1 = not empty
	int skip = 0;  // Skips the character read at the end of each
				   // iteration of while: ugly workaround to
				   // distinguishing | and ||

	symbol_t currentSymbol = newSymbol();
	symbol_t headSymbol = currentSymbol;
	symbol_t tempSymbol;
	while (currentChar != EOF) { // Parsing
	switch (currentChar) {
			case ';':
				assert(empty == 1); // Assert that there is a non-null
									// command prior, or the operation is
									// invalid.
				createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
				currentSymbol->type = SEQUENCE_SYMBOL;
				createSymbol(&currentSymbol, SEQUENCE_SYMBOL);
				break;
			case '|':
				assert(empty == 1);
				createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
					// Check the next character. If it's also a pipe, we
					// have an or operator. If it's anything else, it's
					// just a pipe.
				currentChar = get_next_byte(get_next_byte_argument);
				if (currentChar == '|') {
					createSymbol(&currentSymbol, OR_SYMBOL);
				} else {
					createSymbol(&currentSymbol, PIPE_SYMBOL);
					skip = 1;
				}
				break;
			case '&':
				assert(empty == 1);
				createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
				assert (get_next_byte(get_next_byte_argument) == '&');
					// If the & character isn't followed by another one,
					// the operator is invalid.
				createSymbol(&currentSymbol, AND_SYMBOL);
				break;
			case '(':
				if (empty == 1) { // There does not necessarily need to
								  // be a simple command before a '('
				createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
				}

				createSymbol(&currentSymbol, LBRACKET_SYMBOL);
				break;
			case ')':
				assert(empty == 1);
				createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
				createSymbol(&currentSymbol, RBRACKET_SYMBOL);
				break;
			case '#': // Advance to end of line. No break becase # is
					  // an effective newline
				while (currentChar != '\n') {
					currentChar = get_next_byte(get_next_byte_argument);
				}
			case '\n':
				if (empty == 1) {
					createSimpCommand(&currentSymbol, &commandLength, &allocLength, &simpleCommand, &empty);
					// Check the next byte. if it's also a newline, we have a new command.
					currentChar = get_next_byte(get_next_byte_argument);
					if (currentChar == '\n' || currentChar == '#') {
						createSymbol(&currentSymbol, NEWCOMMAND_SYMBOL);
						while (currentChar == '\n') {
							currentChar = get_next_byte(get_next_byte_argument);
							if (currentChar =='#') {
								while (currentChar != '\n') {
									currentChar = get_next_byte(get_next_byte_argument);
								}
							}
						}
					} else if (currentChar == EOF) {
						break;
					} else { //otherwise, we just have a sequence command.
						createSymbol(&currentSymbol, SEQUENCE_SYMBOL);
					}
					skip = 1;
				}
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
				simpleCommand[commandLength++] = currentChar;
				break;
		}
		if (skip == 0) {
			currentChar = get_next_byte(get_next_byte_argument);
		} else {
			skip = 0;
		}
	}

	//error (1, 0, "command reading not yet implemented");
	//return 0;

  //TODO: initialize a command_stream linked list

  //initialize a command_stream linked list
     command_stream_t stream = (command_stream_t)malloc(sizeof(struct command_stream));
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
    command_node_t currCommandNode = (command_node_t) malloc(sizeof(struct command_node));// init first command node
    stream->head = currCommandNode;
    stream->tail = currCommandNode; //attach to the stream

    newStack(&commandStack,sizeof(struct command));
    newStack(&operatorStack,sizeof(symbol_type)); //enums symbols are ints
    while(currentSymbol != NULL)
    {
      if(currentSymbol->type == NEWCOMMAND_SYMBOL){
          //create a new command node, hoook the last one to the tail  of the stream
      	  struct command currCommand;
      	  popStack(&commandStack, &currCommand);
          command_t temp = (command_t) malloc(sizeof(struct command));
          command_node_t tempCommandNode = (command_node_t)malloc(sizeof(struct command_node));
          stream->tail->next = currCommandNode;
          currCommandNode->root = &currCommand;
          currCommandNode->next = NULL;
	  	  currCommandNode = tempCommandNode;
	  	  destroyStack(&operatorStack);
	  	  destroyStack(&commandStack);
	  	  newStack(&commandStack,sizeof(struct command));
    	  newStack(&operatorStack,sizeof(symbol_type)); //enums symbols are ints

      }
      //check if this symbol is a newline and the next symbol is a newline

      //a)If a simple command, push to a command stack
      if(currentSymbol->type == COMMAND_SYMBOL){
        struct command simpCommand;
        constructSimpleCommand(simpCommand, currentSymbol->simple_command);
        pushStack(&commandStack, &simpCommand);
      }

      //b)If it is a "(", push it onto an operator-stack
      if(currentSymbol->type == LBRACKET_SYMBOL){
        pushStack(&operatorStack, currentSymbol->type);
      }

      //c)If it is an Operator and operator stack is empty
      //  1)push the operator onto the operator stack
      if(currentSymbol->type != COMMAND_SYMBOL && StackisEmpty(&operatorStack)){
        pushStack(&operatorStack, currentSymbol->type);
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
        topStack(&operatorStack, &tempOp);
        while(!StackisEmpty(&operatorStack) && precedence(tempOp)>=precedence(currentSymbol->type) && tempOp != LBRACKET_SYMBOL){
          combine_helper(&operatorStack, &commandStack, &tempOp);
        }
        pushStack(&operatorStack, &(currentSymbol->type));
      }

     // e)If encounter ")", pop operators off the stack
    //(for each operator, pop two commands, combine, push back on command stack) 
    //until you find a matching "(". then create a subshell command by popping 
    //out 1 command from command stack.
      if(currentSymbol->type == RBRACKET_SYMBOL){
        symbol_type tempOp;
        topStack(&operatorStack, &tempOp);
        while(!StackisEmpty(&operatorStack) && tempOp != LBRACKET_SYMBOL){
          combine_helper(&operatorStack,&commandStack,&tempOp);
        }
        struct command subshellCommand;
        struct command topCommand;
        popStack(&commandStack, &topCommand);
        popStack(&operatorStack, &tempOp);
        createSubshell(topCommand, &subshellCommand);
        pushStack(&commandStack, &subshellCommand);
      }
      //f) Advance to next word (simple command, and, or) go to a)
      currentSymbol=currentSymbol->next;
    }
    
    //g)When all words are gone, pop each operator and 
    //combine them with 2 commands similar to d)
    symbol_type tempOp;
    topStack(&operatorStack, &tempOp);
    while(!StackisEmpty(&operatorStack)){
        combine_helper(&operatorStack,&commandStack,&tempOp);
    }
    
    //rootNode is now ready to add to the tree.
    struct command rootNode;
    popStack(&commandStack, &rootNode); 

    //TODO: add the rootNode to a command_Node
    currCommandNode->root = &rootNode;

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
