// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, int time_travel) //I think time_travel is implemented in 1c, won't touch for now
{
	if(c->type == AND_COMMAND){
		//do something
	}
	else if(c->type == SEQUENCE_COMMAND){
		//do something
	}
	else if(c->type == OR_COMMAND){
		// do something
	}
	else if(c->type == PIPE_COMMAND){
		//do something
	}
	else if(c->type == SIMPLE_COMMAND){
		//simplest case: exectue the command.
		pid_t pid = fork();
		if(pid == 0){ //call execute in the child process
		execvp(c->u.word[0],c->u.word);
		//if execvp returns, there has been an error.
		error(1,0, "Unknown command!");
		}
		else{
			waitpid(pid, &(c->status), 0);
			printf("%d\n",WEXITSTATUS(c->status)); 
		}
	}
	else if(c->type == SUBSHELL_COMMAND){
		//execute the subshell command
		pid_t pid = fork();
		if(pid == 0){
			execute_command(c->u.subshell_command,time_travel);
		}
		else{
			waitpid(pid, &(c->status),0);
			printf("%d\n",WEXITSTATUS(c->status));
		}
	}
	else{
		error(1,0, "Invalid command type!");
	}
}
