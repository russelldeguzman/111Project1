// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
<<<<<<< HEAD
#include "symbol.h"

=======
#include "utils.h"
#include "stack.h"
>>>>>>> d13fd33ac2cfed383daec4e1eae1f32a534a7f18
#include <error.h>
#include "list.h"

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
  stream->head=NULL;
  stream->tail=NULL;
}

//Determines whether or not the parser output is a simple command
bool is_simple_command(char * parserOutput){
  //TODO
}

//breaks up the simple command from the parser and constructs command
void constructSimpleCommand(command_t com, char * parserOutput){
  //TODO
}

//combines two commands into result
void combine_commands(command_t right,command_t left ,command_t result, char * op){
  //TODO
  switch(op){
    case '&&':
      result->type=AND_COMMAND;
    break;
    case';':
      result->type=SEQUENCE_COMMAND;
    break;
    case'||':
      result->type=OR_COMMAND;
    break;
    default:
      result->type=PIPE_COMMAND; 
  }
  result->status=-1; //TODO: for part 1B ( dont worry for 1A)
  result->input=NULL;
  result->output=NULL;
  result->u.command[0]=left;
  result->u.command[1]=right;
  result->u.word=NULL; //TODO check this later with TA
  result->u.subshell_command=NULL;
}

//combine_helper: see d) (1-3) in the psuedocode below in make_command_stream
void combine_helper(stack &opStack, stack &cmdStack, command_t tempOp){
   command_t r;
   command_t l;
   command_t result;
   popStack(opStack, tempOp);
   popStack(cmdStack, l);
   popStack(cmdStack, r);
   combine_commands(r,l,result, tempOp);
   pushStack(cmdStack, result);
   topStack(opStack, tempOp);  
}

//creates a subshell
void createSubshell(command_t topCommand, command_t subshellCommand){
  //TODO
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
<<<<<<< HEAD
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
		
	error (1, 0, "command reading not yet implemented");
	return 0;
=======

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
    newstack(&operatorStack,sizeof(char *));
    while(/*!EOF from input*/)
    {
      //a)If a simple command, push to a command stack
      if(is_simple_command(parserOutput)){
        command_t simpCommand;
        constructSimpleCommand(simpCommand, parserOutput)
        pushStack(&commandStack, simpCommand);
      }
      
      //b)If it is a "(", push it onto an operator-stack
      if(parserOutput == '('){
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
        while(!StackisEmpty(&operatorStack) && precedence(tempOp)>=precedence(parserOutput) && tempOp != '('){
          combine_helper(&operatorStack, &commandStack, tempOp);
        }
        pushStack(&operatorStack, parserOutput);
      }
    
     // e)If encounter ")", pop operators off the stack 
    //(for each operator, pop two commands, combine, push back on command stack) 
    //until you find a matching "(". then create a subshell command by popping 
    //out 1 command from command stack.
      if(parserOutput = ')'){
        char * tempOp;
        topStack(&operatorStack, tempOp);
        while(!StackisEmpty(&operatorStack) && tempOp != '('){
          combine_helper(&operatorStack,&commandStack,tempOp);
        }
        command_t subshellCommand;
        command_t topCommand
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

    //TODO: return Command_stream linked list

  error (1, 0, "command reading not yet implemented");
  return 0;
>>>>>>> d13fd33ac2cfed383daec4e1eae1f32a534a7f18
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
	error (1, 0, "command reading not yet implemented");
	return 0;
}
