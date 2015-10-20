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
//TODO: MAJOR TESTING.
void
execute_command (command_t c, int time_travel) //I think time_travel is implemented in 1c, won't touch for now
{
	if(c->type == AND_COMMAND){
		//AND
		execute_command(c->u.command[0],time_travel); //execute command 1
		if (c->u.command[0]->status == 0) { // It is defined that the second half of && only runs if the first half is true
			execute_command(c->u.command[1],time_travel); //execute command 2
		}
		c->status = (c->u.command[0]->status) & (c->u.command[1]->status); //status is the & result of those two ops
	}
	else if(c->type == SEQUENCE_COMMAND){ //very likely more complicated than this
		execute_command(c->u.command[0],time_travel);
		execute_command(c->u.command[1],time_travel);
	}
	else if(c->type == OR_COMMAND){
		//OR
		execute_command(c->u.command[0],time_travel); //execute command 1
		if (c->u.command[0]->status == 0) {
			execute_command(c->u.command[1],time_travel); //execute command 2
		}
		c->status = (c->u.command[0]->status) | (c->u.command[1]->status); //status is the "or" result of those two ops
	}
	else if(c->type == PIPE_COMMAND){
		//TODO: IMPLEMENT PIPE COMMAND
	}
	else if(c->type == SIMPLE_COMMAND){
		//simplest case: execute the command.
		if (c->input != NULL) { //TODO: Implement I/O redirection "<", ">"

		}

		if (c->input != NULL) {

		}
		int status;
		pid_t pid = fork();
		if(pid == 0){ //call execute in the child process
			execvp(c->u.word[0],c->u.word);
			//if execvp returns, there has been an error.
			error(1,0, "Unknown command!");
		}
		else{
			waitpid(pid, &(status), 0);
			c->status = status;
		}
	}
	else if(c->type == SUBSHELL_COMMAND){
		//execute the subshell command
		execute_command(c->u.subshell_command,time_travel);
		c->status = c->u.subshell_command->status;
	}
	else{
		error(1,0, "Invalid command type!");
	}
}
