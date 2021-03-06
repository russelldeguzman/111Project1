// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <stdio.h>
#include <error.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define initialSize 5
typedef struct graphNode *graphNode_t;
typedef struct graph *graph_t;
/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

struct graphNode {
	pid_t pid; //PID (initialze me as -1)
	int id; // integer ID (give me an unique INT ID upon initalization)
	command_t cmd; //associated command
	int *dependancy_list; //list of node ID's that the current node depends on
	int dependancies;
	int listSize;
	graphNode_t nextNode; //so we can link the nodes in the graph
};

struct graph {
	graphNode_t dependancies; //list of nodes with dependancies
	graphNode_t no_dependancies; //list of nodes w/o dependancies
	int no_dep_count;
	int dep_count;
}

//initialize graph
void init_graph(graph_t g){
	g->dependancies = NULL;
	g->no_dependancies = NULL;
	g->no_dep_count = 0;
	g->dep_count = 0;
	return;
}

//iniitialize the graph node
void init_graph_node(graphNode_t node, command_t cmd){
	node = (graphNode_t)malloc(sizeof(struct graphNode));

	//TODO
	return;
}

//add the node to the graph
void add_Node(graph_t graph, graphNode_t node){
	//TODO
	return;
}

//creates the graph out of a command stream
graph_t create_graph(command_stream_t s){
	//initialize graph
	graph_t g = (graph_t)malloc(sizeof(struct graph));
	init_graph(g);

	command_node_t curr = s->head;
	while(curr != NULL){
		graphNode_t currGraphNode = (graphNode_t) malloc(sizeof(struct graphNode));
		init_graph_node(currGraphNode, curr->root);
		add_Node(g,currGraphNode);
		curr = curr->nextNode;
	}
	return g;
}
void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
void clearNodeDependancies(int id, graphNode_t t){
	graphNode_t list = t; 
	while(list != NULL){
		int i;
		for(i = 0; i < list->dependancies; i++){
			if(list->dependancies > 0 && list->dependancy_list[i] == id){
				swap(list->dependancy_list[i], list->dependancy_list[dependancies-1]);
				dependancies--;
				continue;
			}
		}
		list = list->nextNode;
	}
	return;
}
//remove Node from the dependancies list.
void removeNode(graph_t g,int nodeID){
	graphNode_t it = g->dependancies;
	if(it == NULL) return;
	if(it->id == nodeID){
		g->dependancies = it->nextNode;
		g->dep_count--;
		return;
	}
	graphNode_t prev = it; 
	it = it->nextNode;
	while(it != NULL){
		if(it->id == nodeID){
			prev->nextNode = it->nextNode;
			g->dep_count--;
			return;
		}
		prev = it;
		it = it->nextNode;
	}
	return;
}

//execute graph, this needs to go below execute_command.
void execute_graph(graph_t g){
	/*execute all the non-dependant processes first*/
	int i;
	graphNode_t curr = g->no_dependancies;
	for(i = 0; i < g->no_dep_count; i++){
		fork();
		execute_command(curr->cmd);
		curr = curr->nextNode;
	}
	int status;

	for(i = 0; i < g->no_dep_count; i++){
		waitpid(-1,&status,0);
		if(!WIFEXITED(status)){
			error(5,0, "Error in child process");
		}
	}
	curr = g->no_dependancies; //reset iterator
	/*clear all of the dependancies on the first list*/
	for(i = 0; i< g->no_dep_count; i++){
		clearNodeDependancies(curr->id, g->dependancies);
		curr = curr->nextNode;
	}

	/*Execute the dependant processes now*/
	while(g->dep_count > 0){
		graphNode_t temp = g->dependancies;
		while(temp != NULL){
			if(temp->dependancies <= 0){
				pid_t pid=fork();
				int status;
				if(pid == 0){//child
					execute_command(temp->cmd);
				}
				else{ //parent
					waitpid(pid,&status,0);
					if(!WIFEXITED(status)){
						error(5,0,"Error in child process.");
					}
					else{
						clearNodeDependancies(temp->id, g->dependancies);
						removeNode(g,temp->id);
					}
				}
			}
			temp = temp -> nextNode;
		}
	}
	return;
}
//TODO: FIX execute the subshell command
//TODO: ALSO need to impelement I/O stuff for subshelll (may have to change read-command)
void execute_subshell(command_t c, int time_travel){
	int in;
	int out;
	if (c->input != NULL) { // I/O redirection "<", ">"
		//c->u.subshell_command->input = c->input;
		in = open(c->input, O_RDONLY);
		if (in < 0) {
			error(5,0, "Error opening input file");
		}
		dup2(in, 0);
		close(in);
	}
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola

		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
		}
		dup2(out, 1);
		close(out);
	}
	
	execute_command(c->u.subshell_command,time_travel);
	c->status = c->u.subshell_command->status;
}
//simplest case: execute the command.
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
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
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
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
		}
		dup2(out, 1);
		close(out);
	}
	
	execute_command(c->u.command[0],time_travel); //execute command 1
	if(c->u.command[0]->status == 0){
		c->status = c->u.command[0]->status;
		return;
	}
	execute_command(c->u.command[1],time_travel); //execute command 2
	
	c->u.command[1]->status == 0;
	c->status = c->u.command[1]->status;
}
//AND COMMAND
void execute_and(command_t c, int time_travel){
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
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
		}
		dup2(out, 1);
		close(out);
	}
	
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
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
		}
		dup2(out, 1);
		close(out);
	}
	
		int child_status;
		pid_t first_pid, second_pid, return_pid;
		int pipe_fd[2];
		
		int pipe_error = pipe(pipe_fd);
		if(pipe_error == -1) error(1, 0, "Pipe Error");
		first_pid = fork();
		if(first_pid == 0){ //1st child
			close(pipe_fd[0]);
			dup2(pipe_fd[1],1);
			close(pipe_fd[1]);
			execute_command(c->u.command[0],time_travel);
		}
		else{
			//parent
			second_pid = fork();
			if(second_pid == 0){ //second child
				close(pipe_fd[1]);
				dup2(pipe_fd[0],0);
				close(pipe_fd[0]);
				execute_command(c->u.command[1], time_travel);
			}
			else{ //parent again
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				return_pid = waitpid(-1, &child_status,0);
				if(return_pid == first_pid){
					waitpid(second_pid, &child_status, 0);
					c->status = WEXITSTATUS(child_status);
				}
				else{
					waitpid(first_pid,&child_status,0);
					c->status = WEXITSTATUS(child_status);
				}
			}
		}
		
}
//SEQUENCE_COMMAND
void execute_sequence(command_t c, int time_travel){
	int in;
	int out;
	int pid;
	if (c->input != NULL) { // I/O redirection "<", ">"
		in = open(c->input, O_RDONLY);
		if (in < 0) {
			error(5,0, "Error opening input file");
		}
		dup2(in, 0);
		close(in);
	}
	
	if (c->output != NULL) { // Parameter wall comes from lecture notes from a 702 lecture at loyola
		out = open(c->output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (out < 0) {
			error(5,0, "Error opening output file");
		}
		dup2(out, 1);
		close(out);
	}
	
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
