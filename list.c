#include "list.h"

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
