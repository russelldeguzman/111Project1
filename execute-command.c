// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */
   
//TODO: FIX execute the subshell command
//TODO: ALSO need to impelement I/O stuff for subshelll (may have to change read-command)
void execute_subshell(command_t c, int time_travel){
	printf("executing subshell\n");
	execute_command(c->u.subshell_command,time_travel);
	c->status = c->u.subshell_command->status;
	printf("executed subshell\n");
}
//simplest case: exectue the command.
//TODO: Implement I/O redirection "<", ">"
void execute_simple(command_t c, int time_travel){
	int in;
	int out;
	
	if (c->input != NULL) { // I/O redirection "<", ">"
		in = open(c->input, O_RDONLY);
		if (in < 0) {
			error(5,0, "Error opening input file");
		}
		dup2(in, 0);
		close(in);
	}
	
	if (c->input != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open("out", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening input file");
		}
		dup2(out, 1);
		close(out);
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
			c->status = WEXITSTATUS(status); 
	}
}
//OR COMMAND
void execute_or(command_t c, int time_travel){
	execute_command(c->u.command[0],time_travel); //execute command 1
	if(c->u.command[0]->status == 0){
		c->status = c->u.command[0]->status;
		return;
	}
	execute_command(c->u.command[1],time_travel); //execute command 2
	
	c->u.command[1]->status == 0
	c->status = c->u.command[1]->status;
}
//AND COMMAND
void execute_and(command_t c, int time_travel){
	execute_command(c->u.command[0],time_travel); //execute command 1
	if(c->u.command[0]->status != 0){
		c->status = c->u.command[0]->status;
		return;
	}
	else{
		execute_command(c->u.command[1],time_travel); //execute command 2
		c->status = c->u.command[1]->status; 
	}
}
//TODO: FIX PIPE COMMAND
void execute_pipe(command_t c, int time_travel){
		int pfd[2];
		if (pipe(pfd) == -1) { 
		error(1, 0 , "Pipe system call error!");
		}
	
		dup2(pfd[0],0);
		dup2(pfd[1],1);
		
}
//SEQUENCE_COMMAND
void execute_sequence(command_t c, int time_travel){
	execute_command(c->u.command[0],time_travel); //execute command 1
	execute_command(c->u.command[1],time_travel); //execute command 2;
	c->status = c->u.command[1]->status; // right tree is the sequence command's status
}
    
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
		execute_and(c,time_travel);
	}
	else if(c->type == SEQUENCE_COMMAND){
		execute_sequence(c, time_travel);
	}
	else if(c->type == OR_COMMAND){
		execute_or(c, time_travel);
	}
	else if(c->type == PIPE_COMMAND){
		execute_pipe(c, time_travel);
	}
	else if(c->type == SIMPLE_COMMAND){
		execute_simple(c, time_travel);
	}
	else if(c->type == SUBSHELL_COMMAND){
		execute_subshell(c, time_travel);
	}
	else{
		error(1,0, "Invalid command type!");
	}
}
